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
// This file demonstrate using definitions of the external and internal DLLs, spread into multiple files
// Names for local calling are in fact global variables, so they are declared in one file,
// and referenced as externs in other ones
//

#include "stdafx.h"

// See dll_main_sample.cpp for general layout
// See dll_sample_2.cpp for remarks

// That file is very similar to dll_twist.cpp, except using EXTERN mode

#include"../../loadlib.h"
#include"../../loadlib_externs.h"

#include"../dll_main_sample/dll_main_sample.h"  // imported

#include"../../loadlib_exports.h"

#include"../dll_main_sample/dll_main_sample.h"  // exported

#include "twist.h"  // we use loading function defined somewhere else

DLL_DECLARE_INTERFACE_BEGIN()

int DLL_FUNC(my_sub) (int a, int b)     // this is another DLL exported function, its declaration moved away from the main file
{
    manage_underlying_module();
    return DLL_CALL_FUNC_RVAL(my_sub, 0)(1000*a, -b);
}

DLL_DECLARE_INTERFACE_END()

