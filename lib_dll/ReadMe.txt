Copyright [2016-2017] Secoh

Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
You may not use this file except in compliance with the License.
Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

========================================================================
    STATIC LIBRARY : lib_dll
========================================================================

(Limited) support for dual-platform loadable modules,
and simplified interface to declare and use them.

This module shall be statically linked to DLL(s) and to program(s) that
call DLL(s). These DLLs and programs must be dependent on lib_dll.
Higher versions of Visual Studio may also require adding references.


1) loadlib.h

The main header. It must be included first when lib_dll is used,
both into DLL(s) and into the program that calls DLL(s).

2) loadlib_exports.h

The "exports" header. It is intended for declaration of DLL interface
and implementation. It is only used inside DLL that exports functions.

After loadlib.h, and before inclusion of loadlib_exports.h,
the lib_dll macros are in "imports" mode and can be used to load
another DLL from a DLL.

3) loadlib.cpp

The code providing functionality for loading function table from DLL.

4) loadlib_externs.h

Supplemental header for managing multiple-file DLL projects.
Since the function pointers being imported are internally global variables,
they shall be declared only once. loadlib_externs.h modifies
the function descriptions so they appear as "extern" counterparts.

5) stdafx.h

Standard header to include all necessary headers and make
all necessary definitions.

6) stdafx.cpp

Empty file mandated by Visual Studio


========================================================================
    USING lib_dll
========================================================================

See Example subdirectory for the functional example of DLL loading.

