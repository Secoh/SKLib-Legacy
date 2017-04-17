/* Base definitions, data types, values for transport (filter) functions in general

   Efficient manipulation of serialized data streams for small microcomputing platforms without dynamic memory allocations (C-only)

   Copyright [2016-2017] Secoh

   Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
   You may not use this file except in compliance with the License.
   Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

#ifndef SKLIB_FEED_TRANSPORT
#define SKLIB_FEED_TRANSPORT

#include "../lib_common/defs_c.h"


/* transport functions
   caller sends data items one-by-one for processing, until all input it sent, or until receiver signals caller to stop  */

/* general function - NULL in data indicates end of stream */
typedef int (*filter_function_t)(const void *data, void *env);

/* function for simplified I/O that can be covered by exchange of integers (such as, ASCII, Unicode, boolean, etc)
   there must be reserved "EOF value" to indicate end of stream,
   or the state of the receiver must be valid at any point after receiving next value,
   or env points to structure which contains caller-defined semaphore for EOF  */
typedef int (*filter_function_int_t)(int data, void *env);


/* return codes of transport functions
   below is the minimal set of codes that any caller must support

   Structure of the code.
   For purpose of transport I/O, the minimum supported int size is one byte (however, 16+ bit is more common).

   xxxx xxxx cccc nnnn

   X - application-defined information or 0
   C - when 0, "accept" code, and N contains specific outcome type / and 00 must be "unconditional accept"
       nonzero is "reject" code, and then (N+1) is length of latest elements combination that doesn't make sense
       (e.g, if N=0, length is 1, only the next item is illegal - see remark below for more details)
 
   Transport must have ability to return last elements back into the input stream (similar to ungetc() function in C).
   When caller supplies data items (i.e, letters), the next combination may be invalid for processing. For example,
   an alphabet letter when parsing integer. In such case, caller must receive the integer number and offer the offending
   letter to different parser function. Transport may request to return "upstream" by more than one element.
   Example when it is necessary - parsing hex number in C: 0xABC shall be interpreted as number, but 0xZ... results
   in returning back 3 letters as soon as the parser sees 'Z'. In this case, filter returns N=2 along with reject code.  */

#define FLT_ACCEPT  0x00  /* accepted and processed */
#define FLT_CHEW    0x01  /* not accepted, was ignored, and processing continues */
#define FLT_FINISH  0x10  /* letter is accepted and indicates end of data set, result is ready            */
                          /* by convention, filter shall exit with this code if end of stream is signaled */
#define FLT_SWITCH  0x20  /* next letter is not accepted, lower half-byte is return number N                          */
                          /* caller must return back to the input N+1 items and send data somewhere else (see remark) */

#define FLT_STOP_MASK 0xF0


/* manipulating queue of stored data items (data type must support arrays, assigns, and sizeof)
   NB! macros marked by (!!) must not be called with expressions as arguments, and may not be used as expression itself
       (**) - expression in argument must not change content of the variable(s)  */

/* declaration and initialization of the queue are separate macros due to potential C language limitations */

/* declare umbrella variable to control the queue (!!) - all macros arguments below must be single C token */
#define FLT_QUEUE_DECL(data_type_t,variable,length) \
struct {                                            \
    int N, K, P, P1;         /* buffer size, actual count, cyclic position, previous position */ \
    data_type_t V[length];   /* data array */       \
} variable;

/* initialize queue (**) */
#define FLT_QUEUE_INIT(variable) \
( ((variable).N = dim((variable).V)), ((variable).K = (variable).P = (variable).P1 = 0) )

/* check if has long enough history */
#define FLT_QUEUE_IS_DEEP(variable,depth) ((variable).K > (depth))

/* write next element into cyclic buffer, overwrite items older than "length" (**) */
/* returns data - data can be any expression (used once)                           */
#define FLT_QUEUE_PUSH(variable,data)               \
( ((variable).P1 = (variable).P),                   \
((variable).P = (((variable).P+1) % (variable).N)), \
((variable).K++),                                   \
((variable).K = (((variable).K > (variable).N) ? (variable).N : (variable).K)), \
((variable).V[(variable).P1] = (data)) )

/* retrieves stored element from the queue - assumes caller verified it has data (**) */
/* depth can be expression, used once                                                 */
#define FLT_QUEUE_POP(variable,depth) ((variable).V[(variable).K + (variable).N - (depth)])


#endif

