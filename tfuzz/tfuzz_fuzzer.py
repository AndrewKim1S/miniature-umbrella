import logging
import shutil
import threading
import time
import os
import re

from fuzzer import Fuzzer as SFFuzzer
import angr

from .tprogram import TProgram
from .cov import DynamicTrace

logger = logging.getLogger("tfuzz.tfuzz")

class Fuzzer(object):

    def __init__(self, tprogram, seed_files, workdir, target_opts=None,
                 input_placeholder='@@', afl_opts=None):
        self.tprogram = tprogram
        self.workdir = workdir
        self.stat_file = os.path.join(self.workdir,
                                      tprogram.program_name, 'stat')
        self.stat = {}

        self.crashing_inputs = []
        self.tmout_inputs = []

        seeds = []
        for sf in seed_files:
            dtrace = DynamicTrace(tprogram.program_path, sf,
                                  target_opts=target_opts,
                                  input_placeholder=input_placeholder)

            if dtrace.crash():
                self.crashing_inputs.append(sf)
            elif dtrace.timeout():
                self.tmout_inputs.append(sf)
            else:
                seeds.append(file(sf).read())

        use_qemu = False
        if tprogram.is_cgc():
            use_qemu = True

        print("%s %s" % (self.tprogram.program_path, str(target_opts)))
        self._fuzzer = SFFuzzer(self.tprogram.program_path, self.workdir,
                                seeds=seeds, qemu=use_qemu,
                                create_dictionary=False,
                                target_opts=target_opts, extra_opts=afl_opts)

        self.save_crash_and_tmout_inputs()

    def __str__(self):
        return "<Fuzzer:(tprogram:%s, workdir:%s)>" % \
            (self.tprogram, self.workdir)

    def __repr__(self):
        return self.__str__()

    def start(self):
        self._fuzzer.start()

    def stop(self):
        self._fuzzer.kill()
        self.write_stat()

    def __find_generated_files(self, afl_instance, subdir):
        if not os.path.exists(os.path.join(self._fuzzer.out_dir,
                                           afl_instance, subdir)):
            raise ValueError("subdir: '%s' in afl instance '%s' does not exist"\
                             % (subdir, afl_instance))

        subdir = os.path.join(self._fuzzer.out_dir, afl_instance, subdir)
        generated_files = filter(lambda x: x.startswith('id:'),
                                 os.listdir(subdir))
        return map(lambda x: os.path.join(subdir, x), generated_files)

    def generated_inputs(self, afl_instance='fuzzer-master'):
        return self.__find_generated_files(afl_instance, 'queue')

    def crashes_found(self, afl_instance='fuzzer-master'):
        return self.__find_generated_files(afl_instance, 'crashes')

    def crash_seeds(self):
        cs_dir = os.path.join(self._fuzzer.job_dir, 'crashing_seeds')
        cs_files =  filter(lambda x: x.startswith('crash_seed'),
                                 os.listdir(cs_dir))

        return map(lambda x: os.path.join(cs_dir, x), cs_files)

    def failed_to_start(self):
        afl_log_file = os.path.join(self._fuzzer.job_dir, 'fuzzer-master.log')
        if not os.path.exists(afl_log_file):
            return True

        ansi_escape = re.compile(r'\x1b[^m]*m')

        with open(afl_log_file, 'r') as f:
            for l in f.readlines():
                l = l.strip()
                ansi_escape.sub('', l)
                if "[-] PROGRAM ABORT :" in l:
                    return True

        return False

    def save_crash_and_tmout_inputs(self, additional_crash_input_files=None,
                                    additional_tmout_input_files=None):
        ci_dir = os.path.join(self._fuzzer.job_dir, 'crashing_seeds')
        tmout_dir = os.path.join(self._fuzzer.job_dir, 'tmout_seeds')
        if not os.path.exists(ci_dir):
            os.makedirs(ci_dir)
        if not os.path.exists(tmout_dir):
            os.makedirs(tmout_dir)

        if additional_crash_input_files != None:
            for c in additional_crash_input_files:
                self.crashing_inputs.append(c)

        if additional_tmout_input_files != None:
            for c in additional_tmout_input_files:
                self.tmout_inputs.append(c)

        idx = 0
        for i in self.crashing_inputs:
            shutil.copyfile(i, os.path.join(ci_dir,
                                            'crash_seed_' + str(idx)))
            idx = idx + 1

        idx = 0
        for i in self.tmout_inputs:
            shutil.copyfile(i, os.path.join(tmout_dir,
                                            'tmout_seed_' + str(idx)))
            idx = idx + 1

    def resuming(self):
        return self._fuzzer.resuming

    def is_stuck(self):
        try:
            pending_favs = self._fuzzer.stats['fuzzer-master']['pending_favs']
        except:
            return False

        return int(pending_favs) == 0

    def write_stat(self):
        if not os.path.exists(os.path.dirname(self.stat_file)):
            os.makedirs(os.path.dirname(self.stat_file))

        with open(self.stat_file, 'w') as f:
            for key, val in self.stat.items():
                f.write("%s:%s\n" % (key, val))

    def __del__(self):
        self.write_stat()
