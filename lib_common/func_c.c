/* Useful functions missing from C standard library

   Copyright [2016-2017] Secoh

   Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
   You may not use this file except in compliance with the License.
   Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

#include "func_c.h"

extern "C"
{

#ifdef __GNUC__
char *strupr(char *str)   /* this one is missing from GCC library - unsafe to use for anything except ASCII */
{
    char *s1 = str;
    while (char c = *str) *str++ = toupper(c);
    return s1;
}
#endif

/* similar to strupr(), convert all string to lower case - unsafe to use for anything except ASCII */
char *strlow(char *str)
{
    char *s1 = str;
    while (char c = *str) *str++ = tolower(c);
    return s1;
}

/* crossbreed between stricmp and memcmp, missing from standard C library - unsafe to use for anything except ASCII */
int memicmp(const void *s1, const void *s2, size_t n)
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

// end of extern "C"
}

