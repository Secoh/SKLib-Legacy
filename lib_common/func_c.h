/* Useful functions missing from C standard library

   Copyright [2016-2017] Secoh

   Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
   You may not use this file except in compliance with the License.
   Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

#ifndef SKLIB_COM_FUNCC
#define SKLIB_COM_FUNCC

extern "C"
{


#ifdef __GNUC__
char *strupr(char *str);   /* this one is missing from GCC library - unsafe to use for anything except ASCII */
#endif

/* similar to strupr(), convert all string to lower case - unsafe to use for anything except ASCII */
char *strlow(char *str);

/* crossbreed between stricmp and memcmp, missing from standard C library - unsafe to use for anything except ASCII */
int memicmp(const void *s1, const void *s2, size_t n);


// end of extern "C"
}

