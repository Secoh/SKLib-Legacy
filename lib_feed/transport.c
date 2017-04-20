/* Base definitions, data types, values for transport (filter) functions in general

   Efficient manipulation of serialized data streams for small microcomputing platforms without dynamic memory allocations (C-only)

   Copyright [2016-2017] Secoh

   Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
   You may not use this file except in compliance with the License.
   Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

#include "transport.h"

transport_t wrap_vptr(const void *data)
{
    transport_t R;
    R.data = data;
    return R;
}

transport_t wrap_u32(unsigned long value)
{
    transport_t R;
    R.value = value;
    return R;
}
