// will shut up headers that use windows types without need

#ifndef WINDOWS_TYPES_KLUDGE
#define WINDOWS_TYPES_KLUDGE

#define CONST const

#define WINAPI //__stdcall // doesn't exist in linux
typedef void *HANDLE;
#define INVALID_HANDLE_VALUE ((HANDLE)(uint32_t)-1)		// normally, was __uint32
typedef HANDLE HINSTANCE;
typedef HINSTANCE HMODULE;

typedef void *PVOID;
typedef void *LPVOID;

typedef int BOOL;

typedef unsigned char UCHAR;
typedef char CHAR;
typedef unsigned char BYTE;
typedef unsigned char *PUCHAR;
typedef char *PCHAR;

typedef wchar_t WCHAR;

typedef unsigned short WORD;
typedef unsigned short USHORT;
typedef unsigned short *LPWORD;

typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef unsigned long *LPDWORD;
typedef long *LPLONG;
typedef ULONG *PULONG;

typedef CONST WCHAR *LPCWSTR;
typedef CONST CHAR *LPCSTR;
#ifdef UNICODE
 typedef LPCWSTR LPCTSTR; 
#else
 typedef LPCSTR LPCTSTR;
#endif

typedef void *LPOVERLAPPED;
typedef void *LPSECURITY_ATTRIBUTES;

#define __declspec(A)


// extend more definition here...


#endif
