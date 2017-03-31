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

// By default, support of Microsoft-specific names is disabled, notable examples being types like __int32 or DWORD.
// To enable them to compile non-Visual Studio environment, define preprocessor variable ENABLE_MICSOSOFT_SPECIFIC
// in compiler options, or uncomment the line below:
//#define ENABLE_MICSOSOFT_SPECIFIC

// To Do: MinGW suppot


#ifndef SKLIB_COMPATIBILITY
#define SKLIB_COMPATIBILITY

// 0. Verify applicability

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
#pragma message("Target: GCC / libc/ libc++")
#else
#error Sorry, GCC earlier than 4.4.x is not supported
#endif
#else
#error Sorry, only MS Visual Studio and GNU C/C++ are supported at this time
#endif

// 1. Include headers, excluding the common ones in stdafx.h, that are notmally needed with transition to GCC

#ifdef __GNUC__
#include<stdint.h>
#include<unistd.h>
#endif

// 2. May want to preserve bogus types, just in case

#if defined(ENABLE_MICSOSOFT_SPECIFIC) && !defined(_WIN32)
#include"windows_types_kludge.h"
#endif

// 3. Standard (for Linux) fixed-size int types - didn't exist in Visual Studio before version 14.0 (2015)

#ifdef _MSC_VER
#if _MSC_VER < 1900
typedef unsigned __int64 uint64_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int8  uint8_t;
typedef __int64 int64_t;
typedef __int32 int32_t;
typedef __int16 int16_t;
typedef __int8 int8_t;
#endif
#endif

#if defined(ENABLE_MICSOSOFT_SPECIFIC) && !defined(_MSC_VER)
typedef int64_t __int64;
typedef int32_t __int32;
typedef int16_t __int16;
typedef int8_t  __int8;
typedef uint64_t __uint64;
typedef uint32_t __uint32;
typedef uint16_t __uint16;
typedef uint8_t  __uint8;
#endif

// 4. printf() flags interpretation - // use in format string: " ... %" I64FMT "d ... " in place of " ... %I64d ... "

#ifdef _MSC_VER
#define I64FMT "I64"
#define PRId64 I64FMT "d"
#define PRIu64 I64FMT "u"
#define PRIX64 I64FMT "X"
#else
#define I64FMT "ll"
#endif

// 5. Couple cases of different interpretation of standard C library - normalize names to classical form or close
//    currently normalizes: ftell64, fseek64, atoi64, isnan, stricmp, strnicmp, Sleep

#if defined(_MSC_VER)
#define ftell64 _ftelli64
#define fseek64 _fseeki64
#define atoi64 _atoi64
// #define atoll _atoi64  // don't
#define isnan _isnan
#elif defined(__GNUC__)
#define stricmp strcasecmp
#define strnicmp strncasecmp
#define atoi64 atoll
#define ftell64 ftello64
#define fseek64 fseeko64
#if defined(ENABLE_MICSOSOFT_SPECIFIC)
#define _ftelli64 ftello64
#define _fseeki64 fseeko64
#define _atoi64 atoll
#define _isnan isnan
#define __max(a,b) (((a)>(b)) ? (a) : (b))    // unsafe macros, look below for OK replacement
#define __min(a,b) (((a)<(b)) ? (a) : (b))
#endif
#define _TCHAR char
#define _tmain main
#define Sleep(x) (usleep((x)*1000+1))	// useful
#endif

// 6. What do we do with bad code?

#ifdef _MSC_VER                         // for MS VStudio, options to hide specific warnings
#ifndef DISABLE_USELESS_WARNINGS
#pragma warning( once : 4244 )          // conversion from large to small type, possible loss of data (objectionable)
#pragma warning( once : 4996 )          // POSIX name VS. ISO name
#endif
#pragma warning( error : 4700 4554 )    // uninitialized local variable used; ambiguous operator precedence - make that errors
#pragma warning( error : 4806 4309 )    // bool->int result not equal to constant; truncation of a constant
#pragma warning( error : 4129 4552 )    // bad char escape sequence; operator has no effect
#endif

// 7. min, max functions and combos
// Can autodetect argument type, however, argument types must be the same
// Will have to specify a type in template brackets <> if different

template<class input_t>
inline input_t Min(const input_t &A, const input_t &B) { return (B<A) ? B : A; }    // if "equal", returns A - good for any object where strict less() is defined

template<class input_t>
inline input_t Max(const input_t &A, const input_t &B) { return !(A<B) ? A : B; }

template<class input_t>
inline input_t MinMax(const input_t &x, const input_t &lo, const input_t &hi) { return (x<lo ? lo : (hi<x ? hi : x)); }

template<class input_t>
inline bool TestRange(const input_t &x, const input_t &lo, const input_t &hi) { return !(x<lo) && (x<hi); }   // borders are included

template<class input_t>
inline bool TestRangeEx(const input_t &x, const input_t &lo, const input_t &hi) { return !(x<lo) && (x<hi); }   // borders are excluded

// 8. Very few types, functions and operators that shall have existed

typedef unsigned int Int;        // first used in 1992
typedef unsifned char Char;

#ifdef __GNUC__
inline char *strupr(char *str)   // this one is missing from GCC library - unsafe to use for anything except ASCII
{
    char *s1 = str;
    while (char c = *str) *str++ = toupper(c);
    return s1;
}
#endif

// similar to strupr(), convert all string to lower case - unsafe to use for anything except ASCII
inline char *strlow(char *str)
{
    char *s1 = str;
    while (char c = *str) *str++ = tolower(c);
    return s1;
}

// crossbreed between stricmp and memcmp, missing from standard C library - unsafe to use for anything except ASCII
inline int memicmp(const void *s1, const void *s2, size_t n)
{
    const uint8_t *m1 = (const uint8_t *)s1;
    const uint8_t *m2 = (const uint8_t *)s2;

    for (size_t i=0; i<n; i++)
    {
        int C1 = tolower((int)(m1[i]));
        int C2 = tolower((int)(m2[i]));
        if (C1 < C2) return (-1);
        if (C1 > C2) return 1;
    }

    return 0;
}

// enforce 1/0 conversion from bool -- useful if argument is int interpreted as bool
// or to suppress warning of type truncation
#define bool_int(a) ((a)?1:0)

// logical xor: check if logical A not the same as B -- missing "operator ^^"
// similar, allows no-thought use of int types
#define bool_xor(a,b) (bool_int(a) != bool_int(b))

// clean explicit conversion from integer expression to bool
#define bool_cast(v) ((v)!=0)

// get dimension of the array
#define dim(a) (sizeof(a)/sizeof(*(a)))
#define idim(a) ((int)dim(a))             // <-- when A length is not big enough to overflow int;

// standard block sizes
//#define KB(x) (1024*(x))
//#define MB(x) (1048576*(x))
//#define GB(x) (1073741824*(x))


//
#endif



