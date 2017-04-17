/* Common but overlooked features, plus few historical tweaks - C only

   Copyright [2016-2017] Secoh

   Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
   You may not use this file except in compliance with the License.
   Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

#ifndef SKLIB_COM_DEFC
#define SKLIB_COM_DEFC


/* get dimension of the array */

#define dim(a) (sizeof(a)/sizeof(*(a)))
#define idim(a) ((int)dim(a))             /* <-- when A length is not big enough to overflow int */


/* first used in 1992 */

typedef unsigned int Int;
typedef unsigned long Long;
typedef unsigned char Char;


/* boolean tweaks */

/* enforce 1/0 conversion from bool -- useful if argument is int interpreted as bool
   or to suppress warning of type truncation  */
#define bool_int(a) ((a)?1:0)

/* logical xor: check if logical A not the same as B -- missing "operator ^^"
   similar, allows no-thought use of int types  */
#define bool_xor(a,b) (bool_int(a) != bool_int(b))

/* clean explicit conversion from integer expression to bool */
#define bool_cast(v) ((v)!=0)


/* sizes */

/* standard block sizes */
/*#define KB(x) (1024*(x))
#define MB(x) (1048576*(x))
#define GB(x) (1073741824*(x)) */


#endif

