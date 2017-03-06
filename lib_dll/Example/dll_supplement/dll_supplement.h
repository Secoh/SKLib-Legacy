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
// This header provides another export of function names
// this time, to other DLL
//
// See dll_sample_2.cpp, how this 3-rd level DLL is referenced and loaded
//

// ---------------------------------------------------
// Name of this DLL
//
#define DLL_SUPPLEMENT_NAME  _T("dll_supplement")

// ---------------------------------------------------
// Declaration of the functions to be exported
//
DLL_DECLARE_INTERFACE_BEGIN()

char DLL_FUNC(three_letters_func) (int n);   // will call with n=0, 1, 2

DLL_DECLARE_INTERFACE_END();

// ---------------------------------------------------
// Listing of exported functions
//
DLL_ENTRY_LIST(table_3let)
{
    DLL_ENTRY(three_letters_func,  false),  // lets make it optional
    DLL_ENTRY_END()
};

