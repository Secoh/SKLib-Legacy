// Switches certain definitions from loadlib.h to EXTERN mode
// The header loadlib.h sets up DLL_FUNC macro to establish global variable with pointer to a function, so it can be called directly by name
//
// Copyright [2016-2017] Secoh
//
// Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//

#ifndef SKLIB_DLL_IMPORT_SYMBOLS
#include "loadlib.h"
#endif

#undef DLL_FUNC
#undef DLL_QFUNC

#if defined(_MSC_VER)
#define DLL_FUNC(fname) extern (__cdecl * fname)             /* same as in loadlib.h, but make them references */
#define DLL_QFUNC(qual,fname) extern qual (__cdecl * fname)  /* to global variables - function pointers -      */
#else                                                        /* declared in the "main" DLL file                */
#define DLL_FUNC(fname) extern DLL_VISIBILITY (* fname)
#define DLL_QFUNC(qual,fname) extern DLL_VISIBILITY qual (* fname)
#endif
