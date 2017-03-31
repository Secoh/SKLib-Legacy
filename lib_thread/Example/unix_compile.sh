#!/bin/bash
# "dos2unix" by means of standard utilities:
# sed 's/\r//g' file1 > file2

EXE=./_elf_bin

mkdir $EXE 2>/dev/null
rm $EXE/* 2>/dev/null
rm $EXE/compile_history.txt 2>/dev/null

bash ../../compile.sh -KEEP -NOLIBS $EXE/demo_threads  demo_threads ../../lib_thread
# -NOINTEL

# output executables will be in directory designated in EXE variablee (see top of the script)
# notice compiler script removes extension from the output shared module, and replaces it for .so
#
# to test: run ./top_level_caller ./<module name> from executable directory
# (contrary to Windows, in Linux shared module is not searched in ./ by default)
# for example:
#./top_level_caller ./dll_main_sample.so
#./top_level_caller ./dll.sample.2.so
#./top_level_caller ./dll_twist.so



