#!/bin/bash
# "dos2unix" by means of standard utilities:
# sed 's/\r//g' file1 > file2

EXE=./_elf_bin

mkdir $EXE 2>/dev/null
rm $EXE/* 2>/dev/null
rm $EXE/compile_history.txt 2>/dev/null

sh ../../compile.sh -KEEP -NOLIBS -DSTANDALONE_EXAMPLE -DLL $EXE/dll_main_sample   dll_main_sample  ../../lib_dll
sh ../../compile.sh -KEEP -NOLIBS -DSTANDALONE_EXAMPLE -DLL $EXE/dll.offender.dll  dll_offender/    ../../lib_dll
sh ../../compile.sh -KEEP -NOLIBS -DSTANDALONE_EXAMPLE -DLL $EXE/dll.sample.2.     dll_sample_2     ../../lib_dll/
sh ../../compile.sh -KEEP -NOLIBS -DSTANDALONE_EXAMPLE -DLL $EXE/dll_supplement.so dll_supplement/  ../../lib_dll/
sh ../../compile.sh -KEEP -NOLIBS -DSTANDALONE_EXAMPLE -DLL $EXE/dll_twist         dll_twist/       ../../lib_dll
sh ../../compile.sh -KEEP -NOLIBS -DSTANDALONE_EXAMPLE      $EXE/top_level_caller  top_level_caller ../../lib_dll

# output executables will be in directory designated in EXE variablee (see top of the script)
# notice compiler script removes extension from the output shared module, and replaces it for .so
#
# to test: run ./top_level_caller ./<module name> from executable directory
# (contrary to Windows, in Linux shared module is not searched in ./ by default)
# for example:
#./top_level_caller ./dll_main_sample.so
#./top_level_caller ./dll.sample.2.so
#./top_level_caller ./dll_twist.so



