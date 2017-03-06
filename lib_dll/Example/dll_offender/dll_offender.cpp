// DLL Sample Code - using lib_dll environment
//
// Copyright [2016-2017] Secoh
//
// Licensed under the GNU General Public License, Version 3 or later versions (the "License")
// as published by the Free Software Foundation - see <http://www.gnu.org/licenses/>
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

// This is example of code that compiles into valid DLL
// but will not load at runtime because it misses required functions

#include "stdafx.h"

// See dll_sample.cpp for general layout

#include"../../loadlib.h"
#include"../../loadlib_exports.h"
#include"../dll_main_sample/dll_main_sample.h"

DLL_DECLARE_INTERFACE_BEGIN()

int DLL_FUNC(my_add) (int a, int b)
{
    return a | b;
}

int DLL_FUNC(my_sub) (int a, int b)
{
    return a & ~b;
}

// const_pchar DLL_FUNC(my_name) (char *buffer, int limit)     // this function was required
// {                                                           // lets remove it from the DLL and see what happens
//     buffer[0] = 0;
//     return buffer;
// }

DLL_DECLARE_INTERFACE_END()

