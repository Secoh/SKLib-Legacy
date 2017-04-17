/* Parser filters for standard C/C++ values

   Efficient manipulation of serialized data streams for small microcomputing platforms without dynamic memory allocations (C-only)

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


SKLIB_FEED_STDTYPES_NO_WCHAR_T






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











enum { flt_int_ch_break = -1; };

struct 
filter_int_collector_t
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
    };

    int control;   /* expected data type, signed flag, actual sign, etc options
};




extern "C"
{










};














