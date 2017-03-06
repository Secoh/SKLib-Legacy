// Loading executable library and retieving its functions
// Generalized for Windows/Linux portability
//
// Copyright [2016-2017] Secoh
//
// Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//

#include"stdafx.h"
#include"loadlib.h"

#ifdef __GNUC__
#define DLL_LOADLIB(name) dlopen((name), ((RTLD_LAZY)|(RTLD_LOCAL)))
#define DLL_UNLOADLIB(handle) dlclose(handle)
#define DLL_LOADFUNC(handle,name) dlsym((handle), (name))
#define DLL_GETERROR_LIB(handle) ((handle) ? 0 : (ELIBACC))
#define DLL_GETERROR_FUNC(pointr) ((pointr) ? 0 : (ENOSYS))
#define DLL_ERROR_INVALID_FUNCTION (ENOSYS)
#else
#define DLL_LOADLIB(name) ::LoadLibrary(name)
#define DLL_UNLOADLIB(handle) ::FreeLibrary(handle)
#define DLL_LOADFUNC(handle,name) ::GetProcAddress((handle), (name))
#define DLL_GETERROR_LIB(handle) ::GetLastError()
#define DLL_GETERROR_FUNC(pointr) ::GetLastError()
#define DLL_ERROR_INVALID_FUNCTION (ERROR_INVALID_FUNCTION)
#endif

// Attempts to load DLLname and get addresses listed in DLLfunc
// Returns true if success (may be partial), or false on failure
// Detailed status is saved into DLLfunc and DLL_head tables, if enabled
// If any pointer is NULL and it is permitted, the content is not stored

bool load_dll(const DLL_NAME_CHAR *DLLname, struct DLL_entry *DLLfunc, struct DLL_head *DLL)
{
    struct DLL_head LD;
    LD.func_list = DLLfunc;
    LD.load_status = LDL_LOAD;
    
    LD.hDLL = DLL_LOADLIB(DLLname);
    LD.sys_last_err = DLL_GETERROR_LIB(LD.hDLL);

    if (LD.hDLL)
    {
        LD.load_status = LDL_OK;

        for (int i=0; !LDL_FATAL_ERROR(LD.load_status) && DLLfunc[i].func_name; i++)
        {
            DLL_CALLBACK_TYPE FuncAddr = (DLL_CALLBACK_TYPE)DLL_LOADFUNC(LD.hDLL, DLLfunc[i].func_name);
            *(DLL_CALLBACK_TYPE*)(DLLfunc[i].func_ptr) = FuncAddr;
            if (DLLfunc[i].sys_last_err) *(DLLfunc[i].sys_last_err) = DLL_GETERROR_FUNC(FuncAddr);
            if (!FuncAddr)
            {
                if (DLLfunc[i].required) LD.load_status = LDL_NAME;
                else LD.load_status |= LDL_MISS;
            }
        }

        if (!LDL_FATAL_ERROR(LD.load_status))
        {
            if (DLL) *DLL = LD;
            return true;
        }
    }

    // not loaded

    unload_dll(&LD);

    if (DLL) *DLL = LD;
    return !LDL_FATAL_ERROR(LD.load_status);
}

// frees library and sets DLL_entry fields appropriately (NULL and ERROR_INVALID_FUNCTION/ENOSYS)

void unload_dll(struct DLL_head *DLL)
{
    if (DLL->hDLL) DLL_UNLOADLIB(DLL->hDLL);

    for (int i=0; DLL->func_list[i].func_name; i++)
    {
        *(DLL_CALLBACK_TYPE*)(DLL->func_list[i].func_ptr) = NULL;
        if (DLL->func_list[i].sys_last_err) *(DLL->func_list[i].sys_last_err) = DLL_ERROR_INVALID_FUNCTION;
    }
}

// find an entry in struct DLL_entry table; NULL if not found

struct DLL_entry *dll_entry_find(const char *func_name, struct DLL_entry *DLLfunc)
{
    for (int i=0; DLLfunc[i].func_name; i++)
        if (!strcmp(DLLfunc[i].func_name, func_name)) return DLLfunc+i;

    return NULL;
}

// Shortcuts

bool load_dll(const DLL_NAME_CHAR *DLLname, struct DLL_entry *DLLfunc, DLL_MODULE_TYPE *hDLL, int *load_status)
{
    struct DLL_head LD;
    load_dll(DLLname, DLLfunc, &LD);
    *hDLL = LD.hDLL;
    if (load_status) *load_status = LD.load_status;
    return !LDL_FATAL_ERROR(LD.load_status);
}

void unload_dll(DLL_MODULE_TYPE hDLL) { if (hDLL) DLL_UNLOADLIB(hDLL); }


