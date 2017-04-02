// Basic Atomic operations that have be made the same in Windows and Linux
//
// Copyright [2016-2017] Secoh
//
// Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//

#if defined(_WIN32)

#include<intrin.h>

// Visual Studio documentation at some point requires that variables for atomic operations shall be aligned to 32-bit boundary
#define ATOMIC_ALIGN __declspec(align(32))

// Shortcuts if we need an int with soecific bit dapth - use them just like an int type. (Notice that 'int' and 'unsigned' types also work if use "long" version.)
typedef long atomic_int8_t;
typedef unsigned long atomic_uint8_t;
typedef long atomic_int16_t;
typedef unsigned long atomic_uint16_t;
typedef long atomic_int32_t;
typedef unsigned long atomic_uint32_t;
typedef __int64 atomic_int64_t;
typedef unsigned __int64 atomic_uint64_t;

// Using the atomic int types above shall be transparent to C++ program, because the standard requires integer promotion, and size of long is size of int,
// and the overflow condition is handled the same way by hardware regardless of signed/unsigned type.
// However, fully correct way to use the variable is to make explicit cast before it goes into expression, like the following:
// { atomic_uint_16_t V=0; atomic_fetch_and_add(&V,1); printf("%u\n", (uint16_t)V); }
// The atomic type casts shall also work on pointers, but only on Lower-Byte-First architectures.

// Supported operations
// Sadly, once required to use int/long to cover for shorter types, we cannot undo it and must use int32 for all remaining operations.

inline long            atomic_fetch_and_add(volatile long            *ptr, long            val) { return _InterlockedExchangeAdd(ptr, val); }
inline unsigned long   atomic_fetch_and_add(volatile unsigned long   *ptr, unsigned long   val) { return (unsigned long)_InterlockedExchangeAdd((volatile long *)ptr, (long)val); }
inline atomic_int64_t  atomic_fetch_and_add(volatile atomic_int64_t  *ptr, atomic_int64_t  val) { return _InterlockedExchangeAdd64(ptr, val); }
inline atomic_uint64_t atomic_fetch_and_add(volatile atomic_uint64_t *ptr, atomic_uint64_t val) { return (atomic_uint64_t)_InterlockedExchangeAdd64((volatile __int64 *)ptr, (__int64)val); }

inline long            atomic_fetch_and_sub(volatile long            *ptr, long            val) { return _InterlockedExchangeAdd(ptr, -val); }
inline unsigned long   atomic_fetch_and_sub(volatile unsigned long   *ptr, unsigned long   val) { return (unsigned long)_InterlockedExchangeAdd((volatile long *)ptr, -(long)val); }
inline atomic_int64_t  atomic_fetch_and_sub(volatile atomic_int64_t  *ptr, atomic_int64_t  val) { return _InterlockedExchangeAdd64(ptr, -val); }
inline atomic_uint64_t atomic_fetch_and_sub(volatile atomic_uint64_t *ptr, atomic_uint64_t val) { return (atomic_uint64_t)_InterlockedExchangeAdd64((volatile __int64 *)ptr, -(__int64)val); }

inline long            atomic_fetch_and_and(volatile long            *ptr, long            val) { return _InterlockedAnd(ptr, val); }
inline unsigned long   atomic_fetch_and_and(volatile unsigned long   *ptr, unsigned long   val) { return (unsigned long)_InterlockedAnd((volatile long *)ptr, (long)val); }
inline atomic_int64_t  atomic_fetch_and_and(volatile atomic_int64_t  *ptr, atomic_int64_t  val) { return _InterlockedAnd64(ptr, val); }
inline atomic_uint64_t atomic_fetch_and_and(volatile atomic_uint64_t *ptr, atomic_uint64_t val) { return (atomic_uint64_t)_InterlockedAnd64((volatile __int64 *)ptr, (__int64)val); }

inline long            atomic_fetch_and_or (volatile long            *ptr, long            val) { return _InterlockedOr(ptr, val); }
inline unsigned long   atomic_fetch_and_or (volatile unsigned long   *ptr, unsigned long   val) { return (unsigned long)_InterlockedOr((volatile long *)ptr, (long)val); }
inline atomic_int64_t  atomic_fetch_and_or (volatile atomic_int64_t  *ptr, atomic_int64_t  val) { return _InterlockedOr64(ptr, val); }
inline atomic_uint64_t atomic_fetch_and_or (volatile atomic_uint64_t *ptr, atomic_uint64_t val) { return (atomic_uint64_t)_InterlockedOr64((volatile __int64 *)ptr, (__int64)val); }

inline long            atomic_fetch_and_xor(volatile long            *ptr, long            val) { return _InterlockedXor(ptr, val); }
inline unsigned long   atomic_fetch_and_xor(volatile unsigned long   *ptr, unsigned long   val) { return (unsigned long)_InterlockedXor((volatile long *)ptr, (long)val); }
inline atomic_int64_t  atomic_fetch_and_xor(volatile atomic_int64_t  *ptr, atomic_int64_t  val) { return _InterlockedXor64(ptr, val); }
inline atomic_uint64_t atomic_fetch_and_xor(volatile atomic_uint64_t *ptr, atomic_uint64_t val) { return (atomic_uint64_t)_InterlockedXor64((volatile __int64 *)ptr, (__int64)val); }

#elif defined(__GNUC__)

#define ATOMIC_ALIGN

typedef int8_t   atomic_int8_t;     // GCC doesn't need to cope with Microsoft brain decease
typedef uint8_t  atomic_uint8_t;
typedef int16_t  atomic_int16_t;
typedef uint16_t atomic_uint16_t;
typedef int32_t  atomic_int32_t;
typedef uint32_t atomic_uint32_t;
typedef int64_t  atomic_int64_t;
typedef uint64_t atomic_uint64_t;

#define atomic_fetch_and_add __sync_fetch_and_add
#define atomic_fetch_and_sub __sync_fetch_and_sub
#define atomic_fetch_and_or  __sync_fetch_and_or
#define atomic_fetch_and_and __sync_fetch_and_and
#define atomic_fetch_and_xor __sync_fetch_and_xor

#else
#error Atomics is not supported outside MS Visual Studio or GCC
#endif
