#include <setjmp.h>
#include <stdlib.h>
#include "uthreads.h"
#include "uthreads.h"
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>


/*
 * User-Level Threads Library (uthreads)
 * Author: OS, os@cs.huji.ac.il
 */


/* External interface */

#include "UserLevelManager.h"
using namespace std;

UserLevelManager* user;
sigjmp_buf env[MAX_THREAD_NUM];


void makeThreadReady(int tid){
    user->getHashMap().at(tid)->setState(READY);
    user->getLinkedList()->push_back(tid);
}



void runNextTread(){
    int runningThreadId = user->getLinkedList()->front();
    //check whither the function runs in the first time and in the if branches it "booted" up
    //else using siglongjmp we continue the run of the tread from the last time
    if(user->getHashMap().at(runningThreadId)->getQuantums() == 0) {
        user->getHashMap().at(runningThreadId)->getFunction()();
    }else{
        siglongjmp(env[runningThreadId],1);
    }
    user->getHashMap().at(runningThreadId)->setState(RUNNING);
}

/**
 *
 * @param sig
 */
void timer_handler(int sig)
{
    int runningThreadId = user->getLinkedList()->front();
    user->getLinkedList()->pop_front();
    makeThreadReady(runningThreadId);
    user->getHashMap().at(runningThreadId)->upQuantum();
    //save current state **TODO verify
    sigsetjmp(env[runningThreadId],1);
    runNextTread();





    printf("Timer expired\n");
}//TODO need to handle the time threw this function


/*
 * Description: This function initializes the thread library.
 * You may assume that this function is called before any other thread library
 * function, and that it is called exactly once. The input to the function is
 * the length of a quantum in micro-seconds. It is an error to call this
 * function with non-positive quantum_usecs.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_init(int quantum_usecs) {
    struct sigaction sa;
    struct itimerval timer;

    //check if the args is valid
    if (quantum_usecs <= 0 ){
        cerr << ERROR_MSG + BAD_ARG_MSG << endl;
        return -1;
    }
    // Install timer_handler as the signal handler for SIGVTALRM.
    sa.sa_handler = &timer_handler;
    if (sigaction(SIGVTALRM, &sa,NULL) < 0) {
        printf("sigaction error.");
    }
    // Configure the timer to expire after 1 sec... */
    timer.it_value.tv_sec = 0;		// first time interval, seconds part
    timer.it_value.tv_usec = 0;		// first time interval, seconds part

    // configure the timer to expire every 3 sec after that.
    timer.it_interval.tv_sec = 0;	// following time intervals, seconds part
    timer.it_interval.tv_usec = (unsigned int)quantum_usecs;	// fo

    try {
        user =  new UserLevelManager((unsigned int)quantum_usecs);
    }
    catch (const std::bad_alloc &e) {
        cerr << ERROR_MSG + BAD_ALLOC_MSG << endl;
        return -1;
    }
//     Install timer_handler as the signal handler for SIGVTALRM.
    sa.sa_handler = &timer_handler;
    if (setitimer (ITIMER_VIRTUAL, &timer, NULL)) {
        printf("setitimer error.");
    }

//    uthread_spawn(siglongjmp())
    return 0;
}


/*
 * Description: This function creates a new thread, whose entry point is the
 * function f with the signature void f(void). The thread is added to the end
 * of the READY threads list. The uthread_spawn function should fail if it
 * would cause the number of concurrent threads to exceed the limit
 * (MAX_THREAD_NUM). Each thread should be allocated with a stack of size
 * STACK_SIZE bytes.
 * Return value: On success, return the ID of the created thread.
 * On failure, return -1.
*/
int uthread_spawn(void (*f)(void)){
    if(user->getHashMap().size() == user->getMaxthreadNum()){
        return  -1;
    }
    std::shared_ptr<Thread> thread(new Thread(f));
    int tid;
    if(user->getMinHeap().empty()){
        tid = user->getThreadCounter();
        user->addThreadCounter();
    } else{
        tid = user->getMinHeap().top();
        user->getMinHeap().pop();
    }
    thread->setId(tid);
    std::pair<int , shared_ptr<Thread>> newThread(tid , thread);
    user->getHashMap().insert(newThread);
    makeThreadReady(tid);
    return tid;
}


/*
 * Description: This function terminates the thread with ID tid and deletes
 * it from all relevant control structures. All the resources allocated by
 * the library for this thread should be released. If no thread with ID tid
 * exists it is considered as an error. Terminating the main thread
 * (tid == 0) will result in the termination of the entire process using
 * exit(0) [after releasing the assigned library memory].
 * Return value: The function returns 0 if the thread was successfully
 * terminated and -1 otherwise. If a thread terminates itself or the main
 * thread is terminated, the function does not return.
*/
int uthread_terminate(int tid){
    if (tid == 0){
        // TODO delete(&user);
    }
    if (user->getHashMap().erase(tid) == 0){
        cerr << ERROR_MSG + BAD_ARG_MSG << endl;
        return -1;
    }
    user->getLinkedList()->remove(tid);
    user->getMinHeap().push(tid);


}



/*
 * Description: This function blocks the thread with ID tid. The thread may
 * be resumed later using uthread_resume. If no thread with ID tid exists it
 * is considered as an error. In addition, it is an error to try blocking the
 * main thread (tid == 0). If a thread blocks itself, a scheduling decision
 * should be made. Blocking a thread in BLOCKED state has no
 * effect and is not considered as an error.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_block(int tid);


/*
 * Description: This function resumes a blocked thread with ID tid and moves
 * it to the READY state. Resuming a thread in a RUNNING or READY state
 * has no effect and is not considered as an error. If no thread with
 * ID tid exists it is considered as an error.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_resume(int tid);


/*
 * Description: This function blocks the RUNNING thread until thread with
 * ID tid will move to RUNNING state (i.e.right after the next time that
 * thread tid will stop running, the calling thread will be resumed
 * automatically). If thread with ID tid will be terminated before RUNNING
 * again, the calling thread should move to READY state right after thread
 * tid is terminated (i.e. it won’t be blocked forever). It is considered
 * as an error if no thread with ID tid exists or if the main thread (tid==0)
 * calls this function. Immediately after the RUNNING thread transitions to
 * the BLOCKED state a scheduling decision should be made.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_sync(int tid);


/*
 * Description: This function returns the thread ID of the calling thread.
 * Return value: The ID of the calling thread.
*/
int uthread_get_tid();


/*
 * Description: This function returns the total number of quantums that were
 * started since the library was initialized, including the current quantum.
 * Right after the call to uthread_init, the value should be 1.
 * Each time a new quantum starts, regardless of the reason, this number
 * should be increased by 1.
 * Return value: The total number of quantums.
*/
int uthread_get_total_quantums(){
    return user->getQuantumNum();
}


/*
 * Description: This function returns the number of quantums the thread with
 * ID tid was in RUNNING state. On the first time a thread runs, the function
 * should return 1. Every additional quantum that the thread starts should
 * increase this value by 1 (so if the thread with ID tid is in RUNNING state
 * when this function is called, include also the current quantum). If no
 * thread with ID tid exists it is considered as an error.
 * Return value: On success, return the number of quantums of the thread with ID tid. On failure, return -1.
*/
int uthread_get_quantums(int tid){
    if (user->getHashMap().find(tid) == user->getHashMap().end()){
        return -1;
    } else{
        return user->getHashMap().at(tid)->getQuantums();
    }
}


