// Usage example and testing of lib_* projects
//
// Copyright [2016-2017] Secoh
//
// Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//


#include "stdafx.h"    // this file automatically includes gcc_port.h

#include "../lib_filesys/cio.h"
#include "../lib_filesys/dir.h"

#define GETCH "getch"

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc<2)
    {
        printf("Usage: %s module [parameters ... ]\n"
"Available module names are:\n"
GETCH " - Test for no-echo blocking input\n", argv[0] );
        exit(0);
    }

    if (!strnicmp(argv[1], GETCH, dim(GETCH)-1))
    {
        while (true)
        {
            char c[2];
            memset(c, 0, sizeof(c));
            if (kbhit()) c[0] = getch();
            printf("%s\n", (*c ? c : "press key") );
            Sleep(333);
        }
    }

	return 0;
}

