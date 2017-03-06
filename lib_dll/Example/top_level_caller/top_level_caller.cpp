// DLL Sample Code - using lib_dll environment
//
// Copyright [2016-2017] Secoh
//
// Licensed under the GNU General Public License, Version 3 or later versions (the "License")
// as published by the Free Software Foundation - see <http://www.gnu.org/licenses/>
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

// This is top-level caller of DLL stack
// Shows basic example of DLL and functions calls conventions

#include "stdafx.h"

// Visually different from DLL, but logically similar;
// we go ahead and to use DLL interface, importing section...

#include"../../loadlib.h"

// ...include DLL interface we are using

#include"../dll_main_sample/dll_main_sample.h"

// ...and that's all!
// The rest of it is normal C/C++ code

int alt_sub(int, int) { return -65536; }

char BUF[1024];

int _tmain(int argc, _TCHAR* argv[]) 
{
    struct DLL_head HD;

//    we can tweak something if we have to
//    dll_entry_find("my_add", array_of_functions_and_names)->required = false;

    if (!load_dll( (argc==1 ? _T("./") DLL_SAMPLE_ADD_NAME DLL_EXTENSION : argv[1]),
                   array_of_functions_and_names,
                   &HD) )
    {
        printf("Cannot load DLL\n");
        exit(1);
    }

// Example of using calling conventions: required functions can be called straight.
// Optional functions can be called, too, but the better idea is to guard against NULL pointer
// with either alternative return value, or alternative function to call

    printf("%s\n",            my_name(BUF, 1000)                                              );
    printf("%s\n",            DLL_CALL_FUNC_RVAL(my_str, "<NULL ptr>")(BUF, 1000)             );


    // --
    srand(784638);
    for (int i=0; i<10; i++)        // few calls, just for fun
    {
        int A = (rand() % 0x100);
        int B = (rand() % 0x100);

        printf("%02X+%02X=%03X;  %d+%d=%d;  ",
                             A, B,              my_add(A, B),
                             A, B,              my_add(A, B)                                        );
        printf("%d-%d=%d\n", A, B,              DLL_CALL_FUNC_ALT(my_sub,alt_sub)(A, B)             );

    }
    // --

    unload_dll(&HD);

    return 0;
}

