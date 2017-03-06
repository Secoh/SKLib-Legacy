// Switches certain definitions from loadlib.h to EXPORT mode
// see usage details in loadlib.h
//
// Copyright [2016-2017] Secoh
//
// Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//

#ifndef DLL_IMPORT_SYMBOLS
#include "loadlib.h"
#endif

#undef DLL_FUNC
#undef DLL_QFUNC
#undef DLL_ENTRY_LIST
#undef DLL_ENTRY
#undef DLL_ENTRY_RCODE

#define DLL_ENTRY_LIST(list) static struct DLL_entry list##_list[] =
#define DLL_ENTRY(fname,reqd) { #fname "_funk", reqd, NULL, NULL }
#define DLL_ENTRY_RCODE(fname,reqd,variable) { #fname "_funk", reqd, NULL, &variable }

#if defined(_MSC_VER)
#define DLL_FUNC(fname) __declspec(dllexport) __cdecl fname##_funk
#define DLL_QFUNC(qual,fname) __declspec(dllexport) qual __cdecl fname##_funk      /* workaround for ability to declare returned pointers */
#else
#define DLL_FUNC(fname) fname##_funk
#define DLL_QFUNC(qual,fname) qual fname##_funk
#endif
