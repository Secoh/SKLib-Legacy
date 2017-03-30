// Compatibility header for MS Visual Studio and GCC/G++/POSIX
// Establishes cross-definitions of common types and library functions
// Introduces few useful symbols missing in C/C++ and/or standard environments
//
// Copyright [2016-2017] Secoh
//
// Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//

#ifndef SKLIB_COMPATIBILITY
#define SKLIB_COMPATIBILITY

#if defined(_MSC_VER)
#if _MSC_VER < 1200
#error Sorry, Visual Studio 6.0 or earlier is not supported
#elif _MSC_VER < 1500
#warning Visual Studio versions earlier than 2008 are deprecated
#else
#pragma message("Target: MS Visual Studio / Windows")
#endif
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 3)
#pragma message("Target: GCC / libc/ libc++, presumably Linux or its flavor")
#else
#error Sorry, GCC earlier than 4.4.x is not supported
#endif
#else
#error Sorry, only MS Visual Studio and GNU C/C++ are supported at this time
#end

#ifndef _WIN32
#include"windows_types_kludge.h"
#endif














#include<sys/stat.h>
#include<unistd.h>















#endif






2008














// use it in printf() instead in-line declaration of %I64d
#define INT64FMT "I64"
#define DIR_SEP	'\\'

#ifndef __GNUC__
typedef unsigned __int64 __uint64;
typedef unsigned __int32 __uint32;
typedef unsigned __int16 __uint16;
typedef unsigned __int8  __uint8;
typedef __int64 int64_t;			// <-- declarations for
typedef __int32 int32_t;			// UNIX-compatible int types
typedef __int16 int16_t;			// for independent compilation
typedef __int8 int8_t;				// (experimental)
typedef __uint64 uint64_t;
typedef __uint32 uint32_t;
typedef __uint16 uint16_t;
typedef __uint8 uint8_t;
// lets also "support" ISO C++ style of I64 printing
#define PRId64 INT64FMT"d"
#define PRIu64 INT64FMT"u"
#define PRIX64 INT64FMT"X"
#endif

// if UNIX, gcc/g++
#ifdef __GNUC__
#include<values.h>
#include<stdint.h>
#include<inttypes.h>
#include<strings.h>
typedef int64_t __int64;
typedef int32_t __int32;
typedef int16_t __int16;
typedef int8_t  __int8;
typedef uint64_t __uint64;
typedef uint32_t __uint32;
typedef uint16_t __uint16;
typedef uint8_t  __uint8;
#define getch getchar
#define _ftelli64 ftello64
#define _fseeki64 fseeko64
#define stricmp strcasecmp
#define strnicmp strncasecmp
//#define memicmp memcasecmp	-- WTF??  doesn't exist?
#define _atoi64 atoll
#undef INT64FMT
#define INT64FMT "ll"
#undef DIR_SEP
#define DIR_SEP	'/'
//typedef char _TCHAR;
#define _TCHAR char
#define _tmain main
#define _isnan isnan
//
char *gcc_compatibility_strupr(char *str);
#define strupr gcc_compatibility_strupr
//
#define __max(a,b) (((a)>(b)) ? (a) : (b))
#define __min(a,b) (((a)<(b)) ? (a) : (b))
#endif


// Multithreading support (experimental)
//
// function type to call: void (*proc)(void*);
// to open new process, call new_thread
// to wait for child to terminate, call wait_thread  // NB! it is only good for single child, use semaphores if you have many
//	// or not, it may be good to call in cycle for all PID-s
// employ Sleep(0) to intentionaly yield to other processes in waiting cycles (Windows is known to hang without this trick)
// on exit from (*proc)(), the child process is supposed to just disappear
//
#if defined(_WIN32)
#ifndef _WINDOWS_
#include<windows.h>
#endif
#include<process.h>
typedef int64_t type_pid_thread;
#define new_thread(P,M) ((int64_t)_beginthread((P),0,(M)))				// <=0 on error; otherwise, new process PID
#define wait_thread(H) (WaitForSingleObject(((HANDLE)(H)),INFINITE))	// use process PID to wait for thread's termination
//
#include<direct.h>
#define x_mkdir(name) (_mkdir((name)))
//
#include<io.h>
#define x_isftty(file) (_isatty(_fileno(file)))		// use with stdin/out/err

#elif defined(__GNUC__)

#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pthread.h>

typedef pthread_t type_pid_thread;
type_pid_thread new_thread( void (*P)(void*), void *M );
#define wait_thread(H) (pthread_join((H),NULL))


#define Sleep(x) (usleep((x)*1000+1))	// useful
//
#define x_mkdir(name) (mkdir((name), (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)))
#define x_isftty(file) (isatty(fileno(file)))
#else
#error Portable Multithreaded Support is only for Visual Studio and GCC, sorry!
#endif







#define KB(x) (1024*(x))
#define MB(x) (1048576*(x))
#define GB(x) (1073741824*(x))

// return bracketed value to the range (just like __min() or __max())
#ifdef __minmax
#error __minmax() define is alredy taken, this is error
#else
#define __minmax(x,xmin,xmax) ((x)<(xmin) ? (xmin) : ((x)>(xmax) ? (xmax) : (x)))
#endif

// test range
#ifdef __test_range
#error __test_range() define is alredy taken, this is error
#else
#define __test_range(x,xmin,xmax) ((x)>=(xmin) && (x)<=(xmax))
#endif











// Very few types, functions and operators that shall have existed

typedef unsigned Int;            // first used in 1992
typedef unsifned char Char;

#ifdef __GNUC__
inline char *strupr(char *str)   // this one is missing from GCC library
{
    char *s1 = str;
    while (char c = *str) *str++ = toupper(c);
    return s1;
}
#endif

// similar to strupr(), convert all string to lower case
inline char *strlow(char *str)
{
    char *s1 = str;
    while (char c = *str) *str++ = tolower(c);
    return s1;
}

// enforce 1/0 conversion from bool -- useful if argument is int interpreted as bool
// (p.s. brainless "conversion" int->bool->int fails if int is not 1 or 0)
#define bint(a) ((a)?1:0)

// logical xor: check if logical A not the same as B -- missing "operator ^^"
// similar, allows no-thought use of int types
#define bxor(a,b) (bint(a)!=bint(b))

// clean explicit conversion from integer expression to bool
#define bool_cast(v) ((v)!=0)

// get dimension of the array
#define dim(a) (sizeof(a)/sizeof(*(a)))
#define idim(a) ((int)dim(a))             // <-- when A lebgth is not big enough to overflow int;































// // data type sizes
//
// #if sizeof(uint8_t) < 1
// #error Size of UNIT8 must be at least 1 byte
// #endif
//
// #if sizeof(uint16_t) < 2
// #error Size of UNIT16 must be at least 2 bytes
// #endif
//
// #if sizeof(uint32_t) < 4
// #error Size of UNIT32 must be at least 4 bytes
// #endif
//

// -- for MS VStudio, options to hide specific warnings
#ifdef _MSC_VER
#pragma warning( once : 4244 )			// conversion from large to small type, possible loss of data (objectionable)
#pragma warning( once : 4996 )			// POSIX name VS. ISO name
#pragma warning( error : 4700 4554 )	// uninitialized local variable used; ambiguous operator precedence - error?
#pragma warning( error : 4806 4309 )	// bool->int result not equal to constant; truncation of a constant
#pragma warning( error : 4129 4552 )	// bad char escape sequence; operator has no effect
#endif


