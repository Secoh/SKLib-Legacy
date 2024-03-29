# SKLib-Legacy

**This repository is the early attempt to summarize my C/C++ experience.
It is still functional and it may be populated with more projects going forward,
but it is considered outdated and is NOT supported. Use SKLib instead.**

Windows/Linux Utility Libraries (Common API, UI, Math, Etc)

The library collection represents an effort to provide simple functions and declaration for general operations that are frequently needed, and/or missing in the C/C++ standards, and/or too expensive to program each time from scratch.

All user programs link with lib_<something> libraries, preferrably compiled-in at the same time.

Enforcing compatibility:

The library collection resorts, when possible, to the ANSI C and C++ common standards supported by all platforms. "Handy" extensions were deliberately avoided in the code. When common standard doesn't exist and the functionality is too valuable to drop, the wrapper layer is created to hide implementation details from the caller. In all other cases, the functionality is either written from scratch, or left missing.

In very few cases when an utility is badly needed and establishing compatibility is too expensive, the code is programmed to fail at compilation time.

Primary target: Windows XP or higher, 32-bit code, C++. Next priority is GCC/Linux port and 64-bit compatibility. Next is potential for portability to ANSI C.

Modules historically written in C preserve ANSI C compatibility, unless it doesn't make sense anymore.

When possible, compatibility with Linux any modern version and GCC 4.4.x or higher is maintained, if not precluded by nature of the program.

Stable projects (relatively):
   lib_dll;
   lib_thread;
   _texts/stdafx_h_standard_file_contents.txt;
   ./arch.bat;
   ./arch.sh;
   ./compile.sh

In development or unusable:
   lib_common;
   lib_filesys;
   test_bench
