// Simplified multithreading support
// Generalized for Windows/Linux portability
//
// Copyright [2016-2017] Secoh
//
// Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//

/* Bare functions to provide multithreading and to hide platform-specific details

   New data types:

      thread_rpid_t      Opaque POD type to describe a thread (use is optional)

   New functions:

      bool new_thread( void (*proc)(void*), void *data, thread_rpid_t *process_id );

         Starts new thread that runs "proc(data)" function it it
         If the thread has started, returns true and process_id; otherwise returns false and writes garbage into process_id
         Data parameter and pointer to process_id are optional

      void wait_thread( thread_rpid_t &process_id );

         Waits for a child to terminate (can only watch for single process)

   Remarks: upon return from proc(), the child process quietly disappears (and renders the PID invalid)
   In case of waiting for a thread in a dead loop, employ Sleep(0) or similar to intentionaly yield to other processes
   (Windows as recent as XP is known to hang without explicit hint)
   Better way to wait for completion of multiple threads is use of semaphores and/or completion flags  */


#ifndef SKLIB_MTHREAD_SIMPLE
#define SKLIB_MTHREAD_SIMPLE

#if defined(_MSC_VER) || defined(__MINGW32__)

#include<windows.h>
#include<process.h>

typedef uintptr_t thread_rpid_t;

inline bool new_thread(void (*P)(void*), void *M = NULL, thread_rpid_t *rpid_ptr = NULL)
{
    thread_rpid_t rpid_local;
    if (!rpid_ptr) rpid_ptr = &rpid_local;
    *rpid_ptr = _beginthread(P, 0, M);       // "-1L" if error
    return (*rpid_ptr + 1 != 0);
}

inline void wait_thread(thread_rpid_t &H) { WaitForSingleObject((HANDLE)H, INFINITE); }    // use process PID to wait for thread's termination

#elif defined(__GNUC__)

#include<sys/types.h>
#include<pthread.h>

struct thread_rpid_t { bool good; pthread_t pid; };

inline bool new_thread(void (*P)(void*), void *M = NULL, thread_rpid_t *rpid_ptr = NULL)
{
    thread_rpid_t rpid_local;
    if (!rpid_ptr) rpid_ptr = &rpid_local;
    rpid_ptr->good = !pthread_create(&(rpid_ptr->pid), NULL, (void*(*)(void*))P, M);
    return rpid_ptr->good;
}

inline void wait_thread(thread_rpid_t &H) { pthread_join(H.pid, NULL); }

#else
#error Portable Multithreading Support is only for Visual Studio and GCC, sorry!
#endif

#endif

//
// Typical useful function declaration might be:
//
// struct thread_data { bool done; < something task-related >; };
//
// void proc(void *data)
// {
//     struct thread_data *WORKSPACE = (struct thread_data *)data;
//
//     // notice that WORKSPACE->done = false; must NOT be done here
//
//     < referencing working variables as following: WORKSPACE->variable >
//
//     < working... >
//
//     WORKSPACE->done = true;
// }
//
// ...
//
// thread_data T1;
// < initialize all task-related variables >
//
// T1.done = false;                 // important: .done flag MUST be initialized outside proc(), AND before call to new_thread()
//                                  // otherwise a thread may lag and the .done variable is left uninitialized
// if (new_thread(proc, &T1))
// {
//     while (!T1.done) Sleep(0);
// }
//
//


