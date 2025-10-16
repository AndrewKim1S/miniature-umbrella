import os
import sys
import subprocess

from fuzzer import Fuzzer as __angr_Fuzzer

def create_dict(binary, dict_filename):
    create_dict_script = os.path.join(__angr_Fuzzer._get_base(),
                                      "bin", "create_dict.py")

    args = [sys.executable, create_dict_script, binary]

    # import ipdb; ipdb.set_trace()

    with open(dict_filename, 'wb') as df:
        p = subprocess.Popen(args, stdout=df)
        retcode = p.wait()
    return retcode == 0 and os.path.getsize(dict_filename)
