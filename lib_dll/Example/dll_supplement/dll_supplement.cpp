// DLL Sample Code - using lib_dll environment
//
// Copyright [2016-2017] Secoh
//
// Licensed under the GNU General Public License, Version 3 or later versions (the "License")
// as published by the Free Software Foundation - see <http://www.gnu.org/licenses/>
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

//
// This file is another minimal DLL with export of a function
// (used from dll_sample_2.cpp)
//

#include "stdafx.h"

#include"../../loadlib.h"
#include"../../loadlib_exports.h"

#include"dll_supplement.h"


DLL_DECLARE_INTERFACE_BEGIN()

char DLL_FUNC(three_letters_func) (int n)   // will be called with n=0, 1, 2
{
    switch (n)
    {
    case 0: return 'S';
    case 1: return 'U';
    case 2: return 'P';
    }

    return '?';
}

DLL_DECLARE_INTERFACE_END()

