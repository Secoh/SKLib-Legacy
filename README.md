# SKLib

Windows/Linux Utility Libraries (Common API, UI, Math Functinality, Etc)

All user programs link with lib_<something> libraries, preferrably compiled-in at the same time.

Enforcing compatibility:

Library collection resorts, when possible, to the ANSI C and C++ common standard supported by all platforms. "Handy" extensions were deliberately avoided in the code. When common standard doesn't exist and the functionality is too valuable to drop, the wrapper layer is created to hide implementation details from the caller. In all other cases, the functionality is either written from scratch, or left missing.

In very few cases when an utility is needed and establishing compatibility is too expensive, the code is programmed to fail at compilation time.

Primary target: Windows XP or higher, 32-bit code, C++. Next priority is 32-bit/64-bit comatibility. Next is potential for portability to ANSI C.

Modules historically written in C preserve ANSI C compatibility.

When possible, compatibility with Linux any modern version and GCC 4.4.x or higher is maintained, if not precluded by nature of the program.

Stable projects (relatively):
   lib_dll
   lib_thread
   _texts/stdafx_h_standard_file_contents.txt 
   ./arch.bat
   ./compile.sh

In development or unusable:
   lib_common
   lib_filesys
   ./arch.sh

