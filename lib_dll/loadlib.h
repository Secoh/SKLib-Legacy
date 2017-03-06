// Loading executable library and retieving its functions
// Generalized for Windows/Linux portability
//
// Copyright [2016-2017] Secoh
//
// Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//

#ifndef DLL_IMPORT_SYMBOLS
#define DLL_IMPORT_SYMBOLS

// -------------------------------------------------------------------
// Extra headers for Windows/Linux environment

#if defined(_MSC_VER)
  #ifndef _WINDOWS_
  #include<windows.h>
  #endif
  #ifndef __TCHAR_DEFINED
  #include<tchar.h>
  #endif

#elif defined(__GNUC__)
  #include<errno.h>
  #include<dlfcn.h>

#else
#error Sorry, only GNU C/C++ and MS Visual Studio are supported
#endif


// -------------------------------------------------------------------
// We want to turn OFF name mangling
//
#ifdef __cplusplus
  #define DLL_DECLARE_INTERFACE_BEGIN() extern "C" {
  #define DLL_DECLARE_INTERFACE_END() }

#else
  #define DLL_DECLARE_INTERFACE_BEGIN()
  #define DLL_DECLARE_INTERFACE_END()
#endif


// -------------------------------------------------------------------
// DLL functions calling convention
//
#if defined(_MSC_VER)
  #define DLL_MODULE_TYPE HMODULE
  #define DLL_NAME_CHAR   _TCHAR
  #define DLL_CDECL       __cdecl
  #define DLL_VISIBILITY
  #define DLL_STARTUP
  #define DLL_CLEANUP
  #define DLL_EXTENSION   _T(".dll")
#else
  #define DLL_MODULE_TYPE void*
  #define DLL_NAME_CHAR char
  #define DLL_CDECL
  #define DLL_VISIBILITY __attribute__ ((visibility("hidden")))
  #define DLL_STARTUP    __attribute__((constructor))
  #define DLL_CLEANUP    __attribute__((destructor))
  #define DLL_EXTENSION   ".so"
    #ifndef _T
    #define _T(x) x
    #endif
#endif

#define DLL_FUNC(fname) DLL_VISIBILITY (DLL_CDECL * fname)               /* Functions declarators */
#define DLL_QFUNC(qual,fname) DLL_VISIBILITY qual (DLL_CDECL * fname)

typedef int (DLL_CDECL *DLL_CALLBACK_TYPE)();             // Function call entries interpretation is different from legacy WinAPI DLL
//typedef int (*DLL_CALLBACK_TYPE)();

// -------------------------------------------------------------------
// Declaring the DLL entries list
//
#define DLL_ENTRY_LIST(list)             static struct DLL_entry list[] =
#define DLL_ENTRY(fname,reqd)            { #fname "_funk", reqd, &fname, NULL }
#define DLL_ENTRY_RCODE(fname,reqd,errv) { #fname "_funk", reqd, &fname, errv }
#define DLL_ENTRY_END()                  { NULL, false, NULL, NULL }


// -------------------------------------------------------------------
// Loading DLL and storage data structures
//

struct DLL_entry
{
    const char *func_name;       // name of function to load or NULL for end of table
    bool required;               // true if the function must exist (generates error if missing); false - permit absent function
    void *func_ptr;              // will write here address of the function or NULL if not loaded
    unsigned int *sys_last_err;  // may by NULL; if not, writes here result of GetLastError() after function loading
};

// Remark: the first unrecoverable error stops loading
// if func_ptr is NULL: sets load_code to ERROR_INVALID_FUNCTION (Windows) or ENOSYS (Linux)
// if that function was required, also stops with error

struct DLL_head
{
    DLL_MODULE_TYPE hDLL;
    int load_status;              // results of loading, combination of LDL values (see below)
    unsigned int sys_last_err;    // result of GetLastError() after DLL loading
    struct DLL_entry *func_list;  // copy of the DLL_entry pointer - table with filled fields
};

#define LDL_ERROR_MASK	0x0F      /* Mask to select fatal errors */
#define LDL_FATAL_ERROR(err) ((err) & (LDL_ERROR_MASK))

#define LDL_OK      0x00    /* All is OK                              */
#define LDL_LOAD    0x01    /* Cannot load DLL                        */
#define LDL_NAME    0x02    /* Cannot load required function from DLL */

#define LDL_MISS    0x10    /* Cannot load optional function from DLL */

// Attempts to load DLLname and get addresses listed in DLLfunc
// Returns true if success (may be partial), or false on failure
// Detailed status is saved into DLLfunc and DLL_head tables, if enabled
// If any pointer is NULL and it is permitted, the content is not stored

bool load_dll(const DLL_NAME_CHAR *DLLname, struct DLL_entry *DLLfunc, struct DLL_head *DLL = NULL);

// frees library, any allocated memory, and sets DLL_entry fields appropriately (NULL, ERROR_INVALID_FUNCTION)

void unload_dll(struct DLL_head *DLL);

// find an entry in struct DLL_entry table; NULL if not found

struct DLL_entry *dll_entry_find(const char *func_name, struct DLL_entry *DLLfunc);

// Simplified shortcut functions
bool load_dll(const DLL_NAME_CHAR *DLLname, struct DLL_entry *DLLfunc, DLL_MODULE_TYPE *hDLL, int *load_status = NULL);
void unload_dll(DLL_MODULE_TYPE hDLL);	    	// <-- shortcut for FreeLibrary() / dlclose() functions


// -------------------------------------------------------------------
// Calling functions conventions
//

// Calling functions from function pointer that is NOT NULL (aka "required"):
//    fname(param, param, param)  -- just like regular finction call

// Calling functions from function pointer that may be NULL
//
// 1. Function that doesn't return value and doesnt change program's state
// Usage: DLL_CALL_PROC(fname)(param, param, param);
// Does nothing if pointer is NULL
//
#define DLL_CALL_PROC(fname) if (fname) (*(fname))
//
// 2. Function returns value, and must make "default" return value if pointer is NULL
// Usage - in assignment:   value = DLL_CALL_FUNC_RVAL(fname,default_value)(param, param, param);
// Usage - in expression:   v1 + v2 * ( DLL_CALL_FUNC_RVAL(fname,default_value)(param, param, param) ) / v3;
// (!) Note that use in expression REQUIRES extra brackets.
// Assignments are all assignment operators (=, +=, ~=, etc); Expressions are all all other operators, INCLUDING comparisons (==, >, etc)
// Return type of fname function and type of default_value must be same or compatible.
//
#define DLL_CALL_FUNC_RVAL(fname,val_default) !(fname) ? (val_default) : (*(fname))
//
// 3. Alternative to the previous: fname is function pointer and to be testsd for NULL, alt_func is alternative function to call in case of fname==NULL
// Safe for use in all C expressions, call in the same: DLL_CALL_FUNC_ALT(fname,alt_func)(param, param, param)
// (!) Note: alt_func() must be actual function defined locally. Return types of fname() and alt_func() must be same or compatible.
//
#define DLL_CALL_FUNC_ALT(fname,alt_func) (*((fname)?(fname):&(alt_func)))
//
// Remark: if function returns value but it is unused, and function is not expected to perform other work - safe to call by DLL_CALL_PROC macro
//
// Remark: use DLL_CALL_FUNC_RVAL with care. when in doubt, write the selector expression explicitly
//





#endif




