// demo_threads.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../mtc.hpp"


#include<time.h>
#include<math.h>



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Example 1 -- Calculating math on an array, the job can be split into the independent portions
// This is (almost) minimal threaded class derived from local_thread_t

class stats_th : public local_thread_t  // "threading" class shall be derived from local_thread_t   // THIS IS MANDATORY POSITION #1 of 2
{
private:
    double *data;

public:
    int count;                     // for purpose of this example,
    double sum, sum_square;        // lets simplfy and make it like struct in plain C

    stats_th(double *addr, int length)  // lets initialize the working area in the constructor (can be separate function, too) -- base class gets initialized implicitly
    {
        data = addr;
        count = length;
        sum = sum_square = 0;
    }

    void thread_code()		       // the code that will run in parallel -- name and type are hard-coded in local_thread_t class   // THIS IS MANDATORY POSITION #2 of 2
    {
        for (int i=0; i<count; i++)
        {
            sum        += data[i];
            sum_square += data[i] * data[i];
        }
    }                              // after this function exits, base class reports this thread as "done"
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Examples 2 and 3 -- Transform an array into accending cumulative

#define PERFTEST_MULTIPLIER 1000

class cumulative_th : public local_thread_t
{
public:
    enum mode_cml { MAKE_SUM, ADD_UP, PERF_TEST, STEP_MODE };      // perftest is later addition

private:
    double *data;
    int count;
    double sum_section;
    mode_cml command;

public:
    cumulative_th(double *addr, int length)    // define working area...
    {
        data = addr;
        count = length;
    }

    bool run(mode_cml cmd, double rise = 0)      // lets overload base function to accept function selection and parameters; in the end it calls real run()
    {
        command = cmd;
        sum_section = ((cmd == ADD_UP) ? rise : 0);     // we can initialize running variable(s) in custom run() call, rather than in constructor
        return local_thread_t::run();  
    }

    double get_sum() const { return sum_section; }

    void thread_code()          // we can do all work inside thread_code() function, or can make it a redirection hub
    {
        switch (command)
        {
        case MAKE_SUM: make_sum();
                       break;
        case ADD_UP:   transform_array();
                       break;

        case PERF_TEST: for (int i=0; i<PERFTEST_MULTIPLIER; i++) transform_array();
                        break;

        case STEP_MODE: make_sum();
                        pause();                // make use of start/stop semaphore
                        transform_array();
                        break;
        }
    }

    void resume(double rise)   // want to overload again: update thread's state while it is paused - then release
    {
        sum_section = rise;
        local_thread_t::resume();
    }

    void make_sum()
    {
        for (int i=0; i<count; i++) sum_section += data[i];
    }

    void transform_array()
    {
        data[0] += sum_section;
        for (int i=1; i<count; i++) data[i] += data[i-1];
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ARR_SIZE_DEF 10000000
#define NO_THREADS_DEF 9 //17
int ARR_SIZE, NO_THREADS;

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc==1) printf("Usage: threading_test [size of data arrays] [count of threads]\nIf CPU count is given, it also does performance test\n");
    ARR_SIZE = ((argc>1) ? _ttoi(argv[1]) : ARR_SIZE_DEF);
    NO_THREADS = ((argc>2) ? _ttoi(argv[2]) : NO_THREADS_DEF);
    if (ARR_SIZE<1) { printf("Error, enter positive number\n"); exit(1); }

    double *P0 = new double[ARR_SIZE];
    double *P1 = new double[ARR_SIZE];
    double *P2 = new double[ARR_SIZE];

    srand((unsigned)time(NULL));
    for (int i=0; i<ARR_SIZE; i++)  P0[i] = P1[i] = P2[i] = (rand() % 10000);

// lets prepare by brute-force: sum, sum_squared, and cumulatuve
// note index starts with 0 for stats, and with 1 for cumulative

    double sum_bf=0, sum_square_bf=0;
    clock_t T01 = clock();
    for (int i=0; i<ARR_SIZE; i++) sum_bf += P1[i];
    for (int i=0; i<ARR_SIZE; i++) sum_square_bf += P1[i]*double(P1[i]);
    clock_t T02 = clock();
    for (int i=1; i<ARR_SIZE; i++) P2[i] += P2[i-1];
    clock_t T03 = clock();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 1. lets make trivial example of 2 working threads and 1 "dispatch" sleeping thread

    clock_t T11 = clock();

    const int N1 = ARR_SIZE/2;

    stats_th rowA(P1, N1);                  // define threads to run and their workspaces
    stats_th rowB(P1+N1, ARR_SIZE-N1);      // we split all working area in two equal pieces

    rowA.run();             // run() starts thread and exits immediately
    rowB.run();

    rowA.wait();
    rowB.wait();            // start/stop state persists as long as the object does

    clock_t T12 = clock();

    //same way to wait:
//    while (!rowA.done() || !rowB.done()) {}      // threads are running, wait here for completion -- caller doesn't have to worry about Sleep(0) call

    // it is possible to communicate with threads by manipulating rowA,B member variables
    // as good practice, threads may talk only to the "boss" and only via command/status member variables
    // status variable: read-only for boss, write-only for thread
    // command variable: write-only for boss, read-only for thread
    // most easily it is accomplished by using accessor functions

// print result

    int cntres = rowA.count + rowB.count;
    double avgres = (rowA.sum + rowB.sum) / cntres;
    double stdres2 = (rowA.sum_square + rowB.sum_square) / cntres - avgres * avgres;
    printf("N=%d, Avg=%lf (delta=%lf)\nStdev=%lf (delta=%lf)\n", cntres, avgres, sum_bf/ARR_SIZE-avgres,
                sqrt(__max(stdres2,0)), sqrt(__max((sum_square_bf-sum_bf*sum_bf/ARR_SIZE)/ARR_SIZE,0))-sqrt(__max(stdres2,0)) );

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2. less trivial example - some random number or threads in two-stage process
// the threads must start and stop using syncronization
// as mentioned above, syncronization is orchestrated by the "boss thread"
// P.S. again, controlling thread has almost zero CPU footprint

// (!) Notice overhead cost the multithreading version imposes on the algorithm, comparing to its plain version

    clock_t T21 = clock();

    // threads descriptors storage (in form of array of pointers)
    cumulative_th **task_list = new cumulative_th*[NO_THREADS];     // equivalent for constant size:  cumulative_th *task_list[NO_THREADS];

    int *offset = new int[NO_THREADS+1];

    // just for fun, divide ARR_SIZE into random chunks
    // make accending cumulative array, then normalize it on ARR_SIZE
    offset[0] = 0;
    for (int i=1; i<=NO_THREADS; i++) offset[i] = offset[i-1] + (rand() % 1000) + 10000;     // original +0 distorts performance test, because of many small chunks; consequently large ones slow it down
    for (int i=1; i<NO_THREADS; i++) offset[i] = (int)(offset[i]*double(ARR_SIZE)/offset[NO_THREADS]+0.5);  // note we have one extra element in buffer[]
    offset[NO_THREADS] = ARR_SIZE;

    // initialize threads and and run stage #1
    for (int i=0; i<NO_THREADS; i++)
    {
        task_list[i] = new cumulative_th(P1+offset[i], (int)(offset[i+1]-offset[i]));
        task_list[i]->run(cumulative_th::MAKE_SUM);
    }

    cumulative_th::wait_all(task_list, NO_THREADS);

    // prepare stage #2
    // what it does. local thread can only see its working area, but for cumulative, we only need to know the sum of array BEFORE the starting point
    // to calculate the desired sum, we need to add up sums of chunks (we have them on previous step) before the given chunk

    double *rise = new double[NO_THREADS+1];

    rise[0] = 0;
    for (int i=1; i<NO_THREADS; i++) rise[i] = rise[i-1] + task_list[i-1]->get_sum();

    // run stage #2
    for (int i=0; i<NO_THREADS; i++) task_list[i]->run(cumulative_th::ADD_UP, rise[i]);
    cumulative_th::wait_all(task_list, NO_THREADS);

    clock_t T22 = clock();

// lets compare with brute-force series

    double rel_diff_sum = 0;
    for (int i=0; i<ARR_SIZE; i++)
    {
        double S = P1[i] + P2[i];
        rel_diff_sum += abs(P1[i]-P2[i]) / __max(S, 1.0);
    }
    printf("Cumulative test: total relative difference is %lf\n", rel_diff_sum);

    for (int i=0; i<ARR_SIZE; i++) P1[i] = P0[i];   // lets revert working area for the next test - cumulative_th works on P1[]
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 3. Even less trivial example of staged process that uses start-stop semaphore

    clock_t T23 = clock();

    for (int i=0; i<NO_THREADS; i++) task_list[i]->run(cumulative_th::STEP_MODE, 0);
    cumulative_th::wait_all(task_list, NO_THREADS);

    rise[0] = 0; for (int i=1; i<NO_THREADS; i++) rise[i] = rise[i-1] + task_list[i-1]->get_sum();  // prepare offsets again...

    for (int i=0; i<NO_THREADS; i++) task_list[i]->resume(rise[i]);
    cumulative_th::wait_all(task_list, NO_THREADS);

    clock_t T24 = clock();

    rel_diff_sum = 0;
    for (int i=0; i<ARR_SIZE; i++)
    {
        double S = P1[i] + P2[i];
        rel_diff_sum += abs(P1[i]-P2[i]) / __max(S, 1.0);
    }
    printf("Start-stop cumulative test: total relative difference is %lf\n", rel_diff_sum);

    printf("Timing, ms: Stats = %lf(%lf)\nCumulative = %lf(%lf), without exit = %lf\n",
        (T12-T11)*1000.0/CLOCKS_PER_SEC, (T02-T01)*1000.0/CLOCKS_PER_SEC,
        (T22-T21)*1000.0/CLOCKS_PER_SEC, (T03-T02)*1000.0/CLOCKS_PER_SEC, (T24-T23)*1000.0/CLOCKS_PER_SEC);

    if (argc<=2) exit(0);

// performance test on longer run

    clock_t T31 = clock();
    for (int j=0; j<PERFTEST_MULTIPLIER; j++) for (int i=1; i<ARR_SIZE; i++) P2[i] += P2[i-1];
    clock_t T32 = clock();
    for (int i=0; i<NO_THREADS; i++) task_list[i]->run(cumulative_th::PERF_TEST, 0);
    cumulative_th::wait_all(task_list, NO_THREADS);
    clock_t T33 = clock();

    printf("Perftest, ms: MT=%lf; 1T=%lf\n", (T33-T32)*1000.0/CLOCKS_PER_SEC, (T32-T31)*1000.0/CLOCKS_PER_SEC);

	return 0;
}



