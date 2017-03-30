// Simplified multithreading class support
// Generalized for Windows/Linux portability
//
// Copyright [2016-2017] Secoh
//
// Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//

// Simple virtual class for building multithreading evironment


#ifndef SKLIB_MTHREAD_SIMPLE
#include"mt.hpp"               // functional definitions and locks are there

class local_thread_t
{
private:
    int runstate;                    // thread writes // state of the thread
    int flip_resume;                 // boss writes   // flip register - change to this flag signals thread to resume
    int naptime;                     // boss writes   // time delay, if any, on checking status (see remarks)

    static void run_thread(void *self)      // interface with underlying OS API
    {
        ((local_thread_t*)self)->thread_code();
        ((local_thread_t*)self)->runstate = STATE_IDLE;
    }

protected:
    inline void pause()                     // user thread can call pause() from thread_code() to freeze itself
    {                                       // until caller thread ("boss") resumes execution
        int flip_state = flip_resume;
        runstate = STATE_PAUSED;
        while (flip_state == flip_resume) if (naptime >= 0) Sleep(naptime);
        runstate = STATE_RUNNING;
    }

public:
    enum { STATE_IDLE=0, STATE_RUNNING, STATE_PAUSED };

    inline local_thread_t()     // prepare a thread on standby - implicit call from derived class constructor
    {
        tune();
        flip_resume = 0;
        runstate = STATE_IDLE;
    }

    inline bool run()           // launch the thread - can be overloaded by user
    {
        if (runstate != STATE_IDLE) return false;

        runstate = STATE_RUNNING;
        return new_thread(run_thread, this);
    }

    inline void resume()                                 // AFTER thread is seen in PAUSED state, kick it out of pause() sleep - intended to work like mutex lock release
    { if (runstate == STATE_PAUSED) flip_resume++; }     // must NOT be called in dead loop

    inline bool done(bool *terminated = NULL) const      // test if thread run is finished or paused - shall be used strongly after exit from run() and/or pause()
    {                                                    // *terminated is optional flag to test if thread exited completely
        if (naptime >= 0) Sleep(naptime);
        if (terminated) *terminated = (runstate == STATE_IDLE);
        return (runstate != STATE_RUNNING);
    }

    enum { NO_SLEEP=(-1), EASY_SLEEP=0 };
    inline void tune(int sleep_time_ms = EASY_SLEEP) { naptime = sleep_time_ms; }   // fine-tune system sleep time

    inline void wait() const { while (!done()); }        // stop execution of the caller thread until thread of THIS object finishes

    template<class derived_t>
    static inline void wait_all(const derived_t * const * task_list, int count)     // version of wait() for multiple jobs initialized as array pointers to threaded objects;
    {                                                                               // class type can be implicit
        for (int i=0; i<count; i++) task_list[i]->wait();                           // P.S. works much like UNIX shell's "wait" command
    }

    // this is "function pointer" to actual working code,
    // and also will require user to populate it
    virtual void thread_code() = 0;

    // REMARK on using Sleep()
    // On older Windows systems (as recent as XP, however), running a thread in a dead loop can confuse the task scheduler
    // and hog all CPU resourses until the thread exits - regardless on total CPU count and/or priorities of other threads.
    // Making a system call (e.g., Sleep) from the loop breaks this condition and allows kernel scheduler to function.
    // Statement of this sort stood true for period of time strting at least from CP/M systems.
    // Better be safe than sorry.
};

#endif

/* Using local_thread_t class

   The main purpose of this code is to accelerate by multithreading those algorithms that can be broken into
   multiple INDEPENDENT pieces, may be at some overhead cost.

   When a threaded class is set up, it has its own working data and working code, ideally, completely isolated from the outside world.
   Communication with other processes is done by signaling status and receiving commands.

   To avoid using OS-defined mutex locks and other dirty magic tricks, the following rules can be observed.
   A thread is running on isolated "inner world", either by space-domain, or time-domain, or logic-domain. (Notable exclusion: atomic incrementing.)
   Threads can only communicate to the single "boss thread", which runs at very low priority and essentially serves as a scheduler.
   Communication variables are either read-only, or write-only. For any write-only variable, only single thread is allowed to write.
   
   There are two required positions when the threaded class is declared: the derivation itself, and the required thread code.
   They are marked below on sample class declaration:

class my_class : public local_thread_t  // (must-have #1) must be derived from local_thread_t
{
private:
    < data and functions >

public:
    < data and functions >

    my_class( < working area metrics > )   // remark: due to nature of this class, constructor is a good place to setup the workspace partition
    {                                      //         but this can also be done elsewhere
        < general initialization >
        < working area setup >
    }

    void thread_code()		       // (must-have #2) this code will run in a thread -- name and type are hard-coded in local_thread_t class
    {
        < processing >
    }                                  // after this function exits, base class reports this thread as "done"
};

To launch a thread, call run() from the base class. Run() can be overloaded so it can accept additional info.
To verify a thread have finished, call member function done().

When needed, thread can be divided into portions separated by call to protected base class function pause().
It pauses execution of the thread, makes done() to report finish (portion of the task), and then waits for call to resume().
However, tests show that overhead for launching threads is marginal.   */







