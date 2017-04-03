// Terminal-level I/O
//
// Copyright [2016-2017] Secoh
//
// Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//

#if defined(_WIN32)

#include<conio.h>
#define kbhit _kbhit
#elif defined(__GNUC__)

// lets just rewrite and declare missing functions
char getch();
int kbhit();

#endif


// ToDo: what about positioning around the screen for output?

