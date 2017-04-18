/* Parser filters for standard C/C++ values

   Efficient manipulation of serialized data streams for small microcomputing platforms without dynamic memory allocations (C-only)

   Copyright [2016-2017] Secoh

   Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
   You may not use this file except in compliance with the License.
   Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

#include"stdtypes.h"





bool filter_int_init(unsigned long type_designator, struct filter_collector_t *env)
{
    

enum { FILTER_INT_BREAK = -1; };   /* special "character" to signal end of stream */

int filter_int(int c, void *ienv)    /* alphabet: 0-9, A-F, x, -, + */
{
    if (c == FILTER_INT_BREAK)
    {
        
        return FLT_FINISH;
    }

    if (enabled_exp && (c == 'e' || c == 'E'))
    {
        entry_exp
    }

    if (c < '0' || c > '9')
    {

    }






extern "C"
{


int filter_int(int ch, void *result)
{
    if (ch == flt_int_ch_break) return FLT_FINISH;
    if (ch < '0' || ch > '0') return FLT_SWITCH + 1;
    *(int*)result = *(int*)result * 10 + ch - '0';
    return FLT_ACCEPT;
}


/* end of extern "C" */
};








/* returns true if can add i to v, advanced by radix r, and then v has new number (v:=v*r+i);
   false if overflow, then v doesn't change
   r is radix, a and b are accumulators of same type as v
   convention: i>0: positive numbers; i<0: negative numbers; i=0: positive or negative (no difference)  */

#define IADVANCE(v,a,b,r,i) \
( ((a)=((b)=(v))*(r)), ((a)/(r) != (b)) ? 0 : (((b)=(a)+(i)), ((((i)>0 && (b)<(a)) || ((i)<0 && (b)>(a))) ? 0 : (((v)=(b)), 1))) )

/* updates floating point number, no check
   when m <= -1, adds up v radix r increment i, like integers, with same convention i>0/i<0
   after decimal dot, caller must set m:=1, and then send next i normally
   macros will track value's sign, m, and decimal fraction
   v and m must be floating point variables; r, i may be integers  */

#define FADVANCE(v,r,i,m) ( (v) = (((m)<0) ? ((v)*(r)+(i)) : ((v)+((m)/=(r))*(i))) )


