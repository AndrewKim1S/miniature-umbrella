Command line used to find this crash:

/root/.virtualenvs/tfuzz_test/bin/afl-unix/afl-fuzz -i /root/release/work_vuln5_2/fuzzing_vuln5_unix_tfuzz_0/vuln5_unix_tfuzz_0/input -o /root/release/work_vuln5_2/fuzzing_vuln5_unix_tfuzz_0/vuln5_unix_tfuzz_0/sync -m 8G -M fuzzer-master -x /root/release/work_vuln5_2/vuln5_unix.dict -- /root/release/work_vuln5_2/vuln5_unix_tfuzz_0/vuln5_unix_tfuzz_0 

If you can't reproduce a bug outside of afl-fuzz, be sure to set the same
memory limit. The limit used for this fuzzing session was 8.00 GB.

Need a tool to minimize test cases before investigating the crashes or sending
them to a vendor? Check out the afl-tmin that comes with the fuzzer!

Found any cool bugs in open-source tools using afl-fuzz? If yes, please drop
me a mail at <lcamtuf@coredump.cx> once the issues are fixed - I'd love to
add your finds to the gallery at:

  http://lcamtuf.coredump.cx/afl/

Thanks :-)
