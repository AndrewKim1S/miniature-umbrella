Command line used to find this crash:

/root/.virtualenvs/tfuzz_test/bin/afl-unix/afl-fuzz -i /root/release/workdir_vuln5/fuzzing_vuln5_tfuzz/vuln5_tfuzz/input -o /root/release/workdir_vuln5/fuzzing_vuln5_tfuzz/vuln5_tfuzz/sync -m 8G -M fuzzer-master -x /root/release/workdir_vuln5/vuln5.dict -- /root/release/workdir_vuln5/vuln5_tfuzz/vuln5_tfuzz

If you can't reproduce a bug outside of afl-fuzz, be sure to set the same
memory limit. The limit used for this fuzzing session was 8.00 GB.

Need a tool to minimize test cases before investigating the crashes or sending
them to a vendor? Check out the afl-tmin that comes with the fuzzer!

Found any cool bugs in open-source tools using afl-fuzz? If yes, please drop
me a mail at <lcamtuf@coredump.cx> once the issues are fixed - I'd love to
add your finds to the gallery at:

  http://lcamtuf.coredump.cx/afl/

Thanks :-)
