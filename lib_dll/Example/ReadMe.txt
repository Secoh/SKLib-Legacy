Copyright [2016-2017] Secoh

Licensed under the GNU General Public License, Version 3 or later versions (the "License")
as published by the Free Software Foundation - see <http://www.gnu.org/licenses/>
You may not use this file except in compliance with the License.
Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

Before you begin - compilation note.
This package is dual Windows/Linux standalone source code built around lib_dll/
Windows. Minimum WinXP, MS Visual Studio 2008 or higher. Compilation requires
         that all projects define STANDALONE_EXAMPLE preprocessor macro.
         MinGW not tested (possibly, will not work w/o considerably code update).
Linux. GCC 4.4.x or higher. To compile, use unix_compile.sh (will run ../../compile.sh)

========================================================================
    CONSOLE APPLICATION : top_level_caller

    DYNAMIC LINK LIBRARY : dll_main_sample
                           dll_sample_2
                           dll_offender
                           dll_twist
                           dll_supplement

    USES: STATIC LIBRARY : lib_dll  (Apache 2.0 license)
========================================================================

1) top_level_caller\top_level_caller.cpp

   Program that calls DLLs and their functions.
   Usage: top_level_caller.exe [ <dll_name>.dll ]

   You can try to load any DLL from this example.
   By default, it loads dll_main_sample.dll


2) dll_main_sample\dll_main_sample.h

   The header file with DLL interface, that shall be included into the main program
   and to any DLL that uses it, for type verification.

   Then, the DLL functions from the declaration can be called just like normal C/C++ functions.


3) dll_main_sample\dll_main_sample.cpp

   The typical "minimal" DLL.


4) dll_sample_2\dll_sample_2.cpp

   Less common case with interface modifications (allowed and prohibited),
   loading DLL from DLL.


5) dll_offender\dll_offender.cpp

   A DLL with dll_main_sample interface that compiles OK but doesn't load
   because it misses a function marked as required.


6) dll_twist\dll_twist.cpp
   dll_twist\twist_agian.cpp
   dll_twist\twist.h

   Less common example, "call-through" DLL. This module can be loaded from the main program;
   it turn, it loads the dll_main_sample DLL. It uses the same DLL interface (as well as the same header file)
   in upward and in downward directions - but since lib_dll macros are actually slightly different for export
   and for import modes, the names in compilation unit don't collide.

   Also, it is good example of multi-file DLL project where the imported symbols are used from different
   source files. Since the function pointers are global variables by nature, one time they shall be declared
   as variables, all other times they shall be referenced as externs of the same type.

   Third, dll_twist example shows how to split the exported DLL functions between multiple files.


7) dll_supplement\dll_supplement.cpp
   dll_supplement\dll_supplement.h

   This is another trivial DLL example, to be loaded from dll_sample_2.


8) dllmain.cpp, where present

   In DLL projects, standard initialization section. May be used to assign global variables, open files, and
   make other simple initialization work.
   However, this code is not required for DLL mechanism itself to function. Even the file dllmain.cpp may be
   deleted completely. (See examples.)

   This file is created by default by Visual Studio. If your DLLs work only in Windows, it is safe to leave it intact.
   If meaningful initialization is desired, and both Windows and Linux compatibility is required, this file
   shall be modified in order to be used with Linux. See example in dll_main_sample.


9) stdafx.h, all projects

   Standard file required for VC++ compiler.
   Generally contains common includes and defines. In most cases, it is possible to use one-fits-all template.


10) stdafx.cpp, all projects

    Empty file required for VC++ compiler.



========================================================================
    Dependency
========================================================================
 "->" = "depends on"

 top_level_caller ---+--------------------->  dll_main_sample 
                     |                         ^  ^
                     |                         |  |
                     +-->  dll_offender -------+  |
                     |                            |
                     +-->  dll_twist  ------------+
                     |
                     +-->  dll_sample_2 --->  dll_supplement
                         

 In addition, all projects depend on lib_dll.


*** Practical recommendation for arranging dependencies ***

All DLLs that share interface declaration depend on that DLL ("primary") where it was declared.
(Technically, it is one signle file. Unfortunately, Visual Studio desn't have per-file resolution.)

Any change to the primary DLL code will cause need to rebuild essentially all code tree,
even if interface definition is not affected.
The easiest workaround is to make primary the DLL which has least chances to receive changes,
such as "demo" or "no functionality" module.

Another method is to use dedicated "project" contains only one file with declaration of DLL interface
and doesn't have any *.cpp files.



========================================================================
    Note on Character Sets
========================================================================

As normal common sense rule, all projects compiled as one system MUST use the same Character Set.

For lib_dll and Visual Studio, if lib_dll or any of packages that use lib_dll
are set to Unicode, then all lib_dll and all packages using it MUST be Unicode.



