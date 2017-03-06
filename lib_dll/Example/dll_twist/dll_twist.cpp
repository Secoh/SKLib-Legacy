// DLL Sample Code - using lib_dll environment
//
// Copyright [2016-2017] Secoh
//
// Licensed under the GNU General Public License, Version 3 or later versions (the "License")
// as published by the Free Software Foundation - see <http://www.gnu.org/licenses/>
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

// This is example of using more advanced features
// Order of the blocks of the code is critical, see remarks

//
// This file demonstrates another chain calling (see dll_sample_2.cpp for basic example)
// This DLL relays to exactly the same functions as in underlying DLL, but does some extra work
//
// Notice that we are reusing function names, variable names, etc, everywhere
// but it doesnt create conflict
// (Macros are using suffixes to separate imports from exports; and static variables
// are used for function lists to prevent linking errors.)
//

#include "stdafx.h"

// See dll_main_sample.cpp for general layout
// See dll_sample_2.cpp for remarks

#include"../../loadlib.h"

// -----------------------------------------------------
// This line shows the trick.
// The same DLL interface (dll_main_sample.h) is referenced twice:
// first time as import, second time as export.
// Compare with dll_sample_2.cpp which loads DLL with different interface
//
#include"../dll_main_sample/dll_main_sample.h"  // this is import
//
// -----------------------------------------------------

#include"../../loadlib_exports.h"

#include"../dll_main_sample/dll_main_sample.h"  // this is export

// Lets load underlying DLL

#include "twist.h"     // include reference to function(s) needed by multiple parts of the program, for type checking

static bool first_call = true;
void manage_underlying_module()
{
    if (first_call) load_dll(_T("./") DLL_SAMPLE_ADD_NAME DLL_EXTENSION, array_of_functions_and_names);
    first_call = false;
}

// Call underlying DLL functions but do extra work in the middle

DLL_DECLARE_INTERFACE_BEGIN()

int DLL_FUNC(my_add) (int a, int b)
{
    manage_underlying_module();
    return DLL_CALL_FUNC_RVAL(my_add, 0)(-a, -b);       // reuses the same name looking downward; returns something on failure
}                                                       // notice "upward" and "downward" names do not collide

// Lets move this function to other file of this project
// twist_again.cpp
//
//int DLL_FUNC(my_sub) (int a, int b)
//{
//    manage_underlying_module();
//    return DLL_CALL_FUNC_RVAL(my_sub, 0)(-a, -b);
//}

const_pchar DLL_FUNC(my_name) (char *buffer, int limit)
{
    manage_underlying_module();
    memset(buffer, 0, limit);
    buffer[0] = 'X';
    strncpy(buffer+1, ( DLL_CALL_FUNC_RVAL(my_name, "<NULL>")(buffer+1, limit) ), limit-2);
    return buffer;
}

// we'll miss the optional item here

DLL_DECLARE_INTERFACE_END()

