# SKLib
Windows/Linux Utility Libraries (Common API, UI, Math Functinality, Etc)

All user programs link with lib_<something> libraries, preferrably compiled-in at the same time.

Compatibility:

An effort is made to delay any compatibility issues: to lower-level libraries or to C/C++ compatibility.
However, the target execution platform is assumed console/command line of Windows XP or higher, C++.

Compatibility with Linux any modern version and GCC 4.4.x or later is also implied,
if not precluded by nature of the program and is not prohibitively expensive.

Language: C++, being considered extension of ANSI C. Try to keep extending aspects rather limited.
Goal is to keep reasonable backward portability options to plain C.
