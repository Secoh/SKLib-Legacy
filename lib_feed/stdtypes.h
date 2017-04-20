/* Parser filters for standard C/C++ values

   Efficient manipulation of serialized data streams with focus on small microcomputing platforms
   without dynamic memory allocations (C-only).
   Support for unrestricted data types (strings and arrays) on larger systems where full C++ is available.

   Copyright [2016-2017] Secoh

   Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
   You may not use this file except in compliance with the License.
   Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/


#ifndef SKLIB_FEED_STDTYPES
#define SKLIB_FEED_STDTYPES

#include "transport.h"


*&%)(

will also need to add std::string, wstring, vector - if we have C++
bool, wchar_t, (u)int8, 16, 32, 64 - if available




SKLIB_FEED_STDTYPES_NO_SIZED_INTS


xxx SKLIB_FEED_STDTYPES_NO_WCHAR_T xxx






/* discrete or unlimited types : 000x xx.. .... .... */

/* 1. switch : X=0; 0000 00NP .... .... */

#define PAR_SWITCH  0x0000

#define PAR_SW_OPTIONS  0x0300   /* options mask for SWITCH */
#define PAR_SW_MINUS    0x0200   /* can accept "-" as forced no */
#define PAR_SW_PLUS     0x0100   /* can accept "+" as forced yes */

#define PAR_SWITCH_EX (PAR_Switch | PAR_SW_MINUS | PAR_SW_PLUS)   /* that behaves more like boolean KEY rather than traditional Switch */

/* 2. floating-point : X=1; 0000 01TT oooo oo..*/

#define PAR_Double  0x0500
#define PAR_Float   0x0400

#define PAR_FL_OPTIONS 0x00F0    /* options mask for FLOAT and DOUBLE */
#define PAR_FL_NAN     0x0080    /* accept NAN as input */
#define PAR_FL_INF     0x0040    /* accept INF as input */
#define PAR_FL_NOEXP   0x0020    /* prohibit exponent form */
#define PAR_FL_NONEG   0x0010    /* prohibit negative numbers */

#define PAR_DOUBLE_EXT (PAR_DOUBLE | PAR_F_NAN | PAR_F_INF)
#define PAR_FLOAT_EXT  (PAR_FLOAT | PAR_F_NAN | PAR_F_INF)

/* 3. feed to external filter (function pointer to call instead of variable) : X=2; 0000 10.. .... .... */

#define PAR_FEED  0x0800



/* variable-size types */

/* integer */

#define PAR_INT     0x2000
#define PAR_UINT    0x3000

#define PAR_INT_OPTIONS 0x0F80  /* options for INT and UINT */
#define PAR_I_X   0x0100
#define PAR_I_B   0x0200
#define PAR_I_O   0x0400
#define PAR_I_E   0x0800

#define PAR_INT_WIDTH   0x007F


enum  /* type designator & 0xE000 is code */
{
    FILTER_TYPE_FEED   = 0x0000,
    FILTER_TYPE_SWITCH = 0x1000,
    FILTER_TYPE_BOOL   = 0x2000,
    FILTER_TYPE_INT    = 0x4000,
    FILTER_TYPE_UINT   = 0x6000,
    FILTER_TYPE_FLOAT  = 0x8000,
    FILTER_TYPE_LETTER = 0xA000,
    FILTER_TYPE_STRING = 0xC000,  /* output is C++ string, wstring, or vector; not supported on small platforms */
/* 0xE000 reserved */

    FILTER_TYPE_MAJOR  = 0xE000   /* mask for selecting "high" part of the type designator */
};











/* INT and UINT types */

struct filter_collector_t
{
    union
    {
        int val_i;
        unsigned int val_u;
        long val_l;
        unsigned long val_ul;
#ifndef SKLIB_FEED_STDTYPES_NO_SIZED_INTS
        int8_t val_8;
        uint8_t val_u8;
        int16_t val_16;
        uint16_t val_u16;
        int32_t val_32;
        uint32_t val_u32;
        int64_t val_64;
        uint64_t val_u64;
#endif
#ifdef __cplusplus
        void *pvect;
#endif
    };

    union
    {
        unsigned long int_control;   /* expected data type, signed flag, etc options; also actual sign, exponent entry */
        unsigned long float_control;
    };
};

enum
{
    FILTER_INT_UNSIGNED  = 0x8000,  /* 1 unsigned, 0 signed int */
    FILTER_INT_EVT_SIGN  = 0x4000,  /* set if "-" sign is observed */
    FILTER_INT_EVT_EXP   = 0x2000   /* set if exponent entry is in progress */

    FILTER_INT_EXP_ON    = 0x1000,  /* 0x1FFF block - "lower" part of type designator */
    FILTER_INT_RADIX_ON  = 0x0800,
    FILTER_INT_OCT_OFF   = 0x0400,
    FILTER_INT_CTYPE     = 0x0200,
    FILTER_INT_TYPE_MASK = 0x0180,
    FILTER_INT_CHAR      = 0x0000,
    FILTER_INT_INT       = 0x0080,
    FILTER_INT_LONG      = 0x0100,
    FILTER_INT_8         = 0x0000,
    FILTER_INT_16        = 0x0080,
    FILTER_INT_32        = 0x0100,
    FILTER_INT_64        = 0x0180,
    FILTER_INT_COUNT_MASK= 0x007F,  /* range 1..127 positions for fixed arrays (=0 for one element, =1 is 2, etc, 0x7F is reserved for C++ vector */
    FILTER_INT_VECTOR    = 0x007F,  /* C++ vector of respective INT types on unrestricted platform - not available on some microcontrollers       */
};


bool filter_int_init(unsigned long type_designator, struct filter_int_collector_t *env);

enum { FILTER_INT_BREAK = -1; };   /* special "character" to signal end of stream */

int filter_int(int c, void *ienv);






extern "C"
{










};














