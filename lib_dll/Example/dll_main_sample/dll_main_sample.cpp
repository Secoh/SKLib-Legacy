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
// This file demonstrates basic layout of DLL items as related to lib_dll/
//

#include "stdafx.h"

// ---------------------------------------------------
// Any common headers here

// ...

// ---------------------------------------------------
// Declare that DLL interface is used

#include"../../loadlib.h"

// ---------------------------------------------------
// In case that DLL imports from other DLLs,
// this is the place to include their control headers,
// just like the EXE recipient does

// ...

// ---------------------------------------------------
// Switch to Export mode

#include"../../loadlib_exports.h"

// ---------------------------------------------------
// Include the control header of THIS DLL

#include"dll_main_sample.h"

// ---------------------------------------------------
// Good place for any code not related to exporting
// or importing DLL functions

// ...

// ---------------------------------------------------
// Here we go with DLL interface

DLL_DECLARE_INTERFACE_BEGIN()

int DLL_FUNC(my_add) (int a, int b)
{
    return a + b;
}

int DLL_FUNC(my_sub) (int a, int b)
{
    return a - b;
}

const_pchar DLL_FUNC(my_name) (char *buffer, int limit)
{
    strncpy(buffer, "Arythmetics", limit-1);
    buffer[limit-1] = 0;
    return buffer;
}

const char DLL_QFUNC(*, my_str) (char *buffer, int limit)      // note different spelling of the same declaration
{
    strncpy(buffer, "test_str", limit-1);
    buffer[limit-1] = 0;
    return buffer;
}

DLL_DECLARE_INTERFACE_END()
