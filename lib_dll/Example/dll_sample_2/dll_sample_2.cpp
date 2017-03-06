// DLL Sample Code - using lib_dll environment
//
// Copyright [2016-2017] Secoh
//
// Licensed under the GNU General Public License, Version 3 or later versions (the "License")
// as published by the Free Software Foundation - see <http://www.gnu.org/licenses/>
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

// This is example of typical and extended DLL code
// Order of the blocks of the code is critical, see remarks
//
// This file demonstrates sharing interface between multiple DLL's,
// omission of optional functions, and linking to 2nd level DLL
// All information is related to lib_dll/
//

#include "stdafx.h"

// ---------------------------------------------------
// DLL section

// See dll_main_sample.cpp for general layout

#include"../../loadlib.h"

// ---------------------------------------------------
// Import function(s) from other DLL(s)

#include"../dll_supplement/dll_supplement.h"

// ---------------------------------------------------
// EXPORTS section

#include"../../loadlib_exports.h"

// ---------------------------------------------------
// This DLL shares exported interface with "primary" DLL

#include"../dll_main_sample/dll_main_sample.h"

// ---------------------------------------------------
// DLL interface implementation

DLL_DECLARE_INTERFACE_BEGIN()

int DLL_FUNC(my_add) (int a, int b /*, int c */ )      // Shared header provides Function Type Check
{
    return a | b;
}

/* This function will be missing from DLL.
   No problem if it is still listed in DLL interface declaration.
   Also, this function was optional, it is allowed to fail on loading.

int DLL_FUNC(my_sub) (int a, int b)
{
    return a & ~b;
}
*/

//const_pchar DLL_FUNC(my_name) (char *buffer, int limit)         // because of typedef const char *const_pchar;
const char DLL_QFUNC(*, my_name) (char *buffer, int limit)        // these two declarations can be used/mixed interchangeably
{                                                                 // see below one more
    strncpy(buffer, "Bitwise", limit-1);
    buffer[limit-1] = 0;
    return buffer;
}

static bool first_call = true;

// const char DLL_QFUNC(*, my_str) (char *buffer, int limit)
const_pchar DLL_FUNC(my_str) (char *buffer, int limit)
{
    if (first_call)
    {
        first_call = false;
        load_dll(_T("./") DLL_SUPPLEMENT_NAME DLL_EXTENSION, table_3let);     // in this function, we load and call from underlying DLL
    }

    int n;
    for (n=0; n<3 && n<limit-1; n++) buffer[n] = DLL_CALL_FUNC_RVAL(three_letters_func, '.')(n);
    buffer[n] = '\0';
    return buffer;
}

DLL_DECLARE_INTERFACE_END()

