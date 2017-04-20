
/* types */

#ifdef SKLIB_FEED_STDTYPES_NO_SIZED_INTS
typedef unsigned long unicode_t;
typedef unsigned long utf32_t;
typedef unsigned char utf8_t;
typedef unsigned int utf16_t;
#else
typedef uint32_t unicode_t;
typedef uint32_t utf32_t;
typedef uint8_t utf8_t;
typedef uint16_t int utf16_t;
#endif





/* receive unicode_t, write utf-8 */


struct uni_transform
{
    void *caller_data;
    filter_function_t furhter_call;


state


};



int uncode_to_utf8(unicode_t letter, struct uni_transform *env);






-------------------------------------------

example

int write_file(transport_t ch, void *env)
{
    unsigned char c = (unsigned char)ch.value;
    return ( fwrite(&c, 1, 1, (FILE*)env) ? FLT_ACCEPT : FLT_CHEW );
}



save_file(FILE *f, unicode_t *text)
{
    while (*text) unicode_to_utf(*text++, env);
}










