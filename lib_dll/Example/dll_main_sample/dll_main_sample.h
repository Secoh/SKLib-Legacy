// DLL Sample Code - using lib_dll environment
//
// Copyright [2016-2017] Secoh
//
// Licensed under the GNU General Public License, Version 3 or later versions (the "License")
// as published by the Free Software Foundation - see <http://www.gnu.org/licenses/>
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

// This is example of typical and extended DLL code
// Order of the blocks of the code may be critical, see remarks

// ---------------------------------------------------
// This header is strictly for relaying names
// from DLL to EXE (or to another DLL)
//
// In case if DLL uses another DLL, you must include
// the underlying DLL's header into the user's DLL code
// just like it is done for the top-level executable.
// IF DLL uses the same interface (header) for both
// imports and exports, you must include the header
// twice (see example in dll_sample_2.cpp)
//

// ---------------------------------------------------
// < Optional >
// Name of the DLL of THIS export, as it appears on disk
//
#define DLL_SAMPLE_ADD_NAME  _T("dll_main_sample")
#define DLL_SAMPLE_ADD_NAME2 _T("dll_sample_2")     // for purpose of exercise it doesn't matter where we declare DLL names themselves
#define DLL_SAMPLE_OFFENDER  _T("dll_offender")

// ---------------------------------------------------
// Declaration of the functions to be exported
// These function references can be used
// just like regular functions, from the caller prospective
//
DLL_DECLARE_INTERFACE_BEGIN()       // no ";" allowed here

int DLL_FUNC(my_add) (int a, int b);
int DLL_FUNC(my_sub) (int a, int b);

typedef const char *const_pchar;                            // Limitation: if anything more than primitive type is to be returned by function,
const_pchar DLL_FUNC(my_name) (char *buffer, int limit);    // the definition MUST use typedef to package it into single type token

const char DLL_QFUNC(*, my_str) (char *buffer, int limit);  // Alternative way to declare the same

// Note: declaring typedef char *pchar; and then using const pchar is NOT the same
// (constant pointer instead pointer to constant)

DLL_DECLARE_INTERFACE_END();        // ";" here is optional

// ---------------------------------------------------
// Listing of exported functions
// This array is used to load DLL and populate
// the function references declared above
//
DLL_ENTRY_LIST(array_of_functions_and_names)
{
    DLL_ENTRY(my_add,  true),
    DLL_ENTRY(my_sub,  false),      // optional function is denoted by "false"
    DLL_ENTRY(my_name, true),
    DLL_ENTRY(my_str,  false),
    DLL_ENTRY_END()
};

