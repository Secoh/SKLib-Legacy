/* Parser filters for standard C/C++ values

   Efficient manipulation of serialized data streams for small microcomputing platforms without dynamic memory allocations (C-only)

   Copyright [2016-2017] Secoh

   Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
   You may not use this file except in compliance with the License.
   Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

#include"stdtypes.h"


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
