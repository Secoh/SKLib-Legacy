// stdafx.h : standard headers
//
// Copyright [2016-2017] Secoh
//
// Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//

#pragma once
#define _CRT_NONSTDC_NO_DEPRECATE   // no more pointless warnings
#define _CRT_SECURE_NO_WARNINGS     //

#ifndef _WIN32_WINNT                // (Windows-specific) - former targetver.h contents
#define _WIN32_WINNT 0x0501   //XP  // The minimum Windows version this program requires to run
#endif

#define WIN32_LEAN_AND_MEAN

#ifndef __GNUC__
#include <tchar.h>
#endif

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>

#ifndef _USE_MATH_DEFINES    // Lets use old-stype math.h defines
#define _USE_MATH_DEFINES    // such as M_PI, M_LOG2E, etc
#endif
#include<math.h>

#include<wchar.h>     // UTF Initiative

#ifndef __GNUC__
#include<conio.h>
#endif


// can include or exclude reference to full gcc/Linux portability header

#ifndef STANDALONE_EXAMPLE
#include"../lib_common/gcc_port.h"
#endif