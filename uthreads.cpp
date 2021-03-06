#include <setjmp.h>
#include <stdlib.h>
#include "uthreads.h"
#include <signal.h>
#include <sys/time.h>
#include "UserLevelManager.h"

/*
 * User-Level Threads Library (uthreads)
 * Author: OS, os@cs.huji.ac.il
 */


/* External interface */
typedef unsigned long address_t;
#define JB_SP 6
#define JB_PC 7
using namespace std;

/* A translation is required when using an address of a variable.
Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%fs:0x30,%0\n"
            "rol    $0x11,%0\n"
    : "=g" (ret)
    : "0" (addr));
    return ret;
}

void blockSignals(){
    sigset_t x;
    sigemptyset (&x);
    sigaddset(&x, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &x, NULL);
}

void unBlockSignals(){
    sigset_t x;
    sigemptyset (&x);
    sigaddset(&x, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &x, NULL);

}

//the time structs
int programQuantumUsecs;
//the handler user manager instant
UserLevelManager* user;
//the thread state array
sigjmp_buf env[MAX_THREAD_NUM];


void makeThreadReady(int tid){
    if (user->getHashMap()->find(tid) == user->getHashMap()->end()){
        return;
    }
    user->getHashMap()->at(tid)->setState(READY);
    user->getLinkedList()->push_back(tid);
}



void runNextThread(){
    user->addQuantumNum();
    int runningThreadId = user->getLinkedList()->front();
    user->getHashMap()->at(runningThreadId)->upQuantum();
    user->getHashMap()->at(runningThreadId)->setState(RUNNING);
    if (user->getHashMap()->at(runningThreadId)->getFunction() == NULL){
        unBlockSignals();
        return;
    }
    unBlockSignals();
    siglongjmp(env[runningThreadId],1);
}


void deleteSyncList(int tid){
    shared_ptr<Thread> currThread = user->getHashMap()->at(tid);
    if (currThread->getSyncList()->size() == 0){
        return;
    }

    std::list<int>::const_iterator it;
    for (it = currThread->getSyncList()->begin(); it != currThread->getSyncList()->end(); ++it){
        if (user->getHashMap()->at(*it)->getState() == SYNC_BLOCKED)
        {
            makeThreadReady(*it);
        } else if (user->getHashMap()->at(*it)->getState() == BLOCKED_BOTH){
            user->getHashMap()->at(*it)->setState(BLOCKED);
        }
    }
    currThread->bootSyncList();
}

/**
 *
 * @param sig
 */
void timer_handler(int sig)
{
    int runningThreadId = user->getLinkedList()->front();
    int ret_val  = sigsetjmp(env[runningThreadId],1);
    if (ret_val == 1) {
        return;
    }
    user->getLinkedList()->pop_front();

    //save current state
    deleteSyncList(runningThreadId);
    makeThreadReady(runningThreadId);

    user->addQuantumNum();
    runningThreadId = user->getLinkedList()->front();
    user->getHashMap()->at(runningThreadId)->upQuantum();
    user->getHashMap()->at(runningThreadId)->setState(RUNNING);
    unBlockSignals();
    siglongjmp(env[runningThreadId],1);
}

void timeBoot(){
    struct sigaction sa;
    struct itimerval timer;
    blockSignals();
    sa.sa_handler = &timer_handler;

    if (sigaction(SIGVTALRM, &sa,NULL) < 0) {
        printf("sigaction error.");
    }
    // Configure the timer to expire after 1 sec... */
    timer.it_value.tv_sec = 0;		// first time interval, seconds part
    timer.it_value.tv_usec = programQuantumUsecs;		// first time interval, seconds part

    // configure the timer to expire every 3 sec after that.
    timer.it_interval.tv_sec = 0;	// following time intervals, seconds part
    timer.it_interval.tv_usec = programQuantumUsecs;	// fo
    if (setitimer (ITIMER_VIRTUAL, &timer, NULL)) {
        printf("setitimer error.");
    }
    unBlockSignals();
}

/*
 * Description: This function initializes the thread library.
 * You may assume that this function is called before any other thread library
 * function, and that it is called exactly once. The input to the function is
 * the length of a quantum in micro-seconds. It is an error to call this
 * function with non-positive quantum_usecs.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_init(int quantum_usecs) {
    blockSignals();
    //check if the args is valid
    if (quantum_usecs <= 0 ){
        cerr << ERROR_MSG + BAD_ARG_MSG << endl;
        unBlockSignals();
        return -1;
    }

    // Install timer_handler as the signal handler for SIGVTALRM.
    programQuantumUsecs = quantum_usecs;
    timeBoot();

    try {
        user =  new UserLevelManager((unsigned int)quantum_usecs);
    }
    catch (const std::bad_alloc &e) {
        cerr << ERROR_MSG + BAD_ALLOC_MSG << endl;
        return -1;
    }
    //init to the main thread
    uthread_spawn(NULL);
    runNextThread();
    unBlockSignals();
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
    if(user->getHashMap()->size() == user->getMaxthreadNum()){
        return  -1;
    }
    std::shared_ptr<Thread> thread(new Thread(f));
    int tid;
    if(user->getMinHeap()->empty()){
        tid = user->getThreadCounter();
    } else{
        tid = user->getMinHeap()->top();
        user->getMinHeap()->pop();
    }
    user->addThreadCounter();
    thread->setId(tid);
    std::pair<int , shared_ptr<Thread>> newThread(tid, thread);
    address_t sp = (address_t)(thread->getStack()) + STACK_SIZE - sizeof(address_t);
    address_t pc = (address_t)f;
    sigsetjmp(env[tid], 1);
    (env[tid]->__jmpbuf)[JB_SP] = translate_address(sp);
    (env[tid]->__jmpbuf)[JB_PC] = translate_address(pc);
    sigemptyset(&env[0]->__saved_mask);
    (*user->getHashMap()).insert(newThread);
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
    blockSignals();
    if (tid == 0){
        delete(user);
        unBlockSignals();
        exit(0);
     }
    if (user->getHashMap()->find(tid) == user->getHashMap()->end()){
        cerr << ERROR_MSG + BAD_ARG_MSG << endl;
        unBlockSignals();
        return -1;
    }
    deleteSyncList(tid);

    if (user->getLinkedList()->front() == tid) {
        user->getLinkedList()->pop_front();
        user->addQuantumNum();
        int runningThreadId = user->getLinkedList()->front();
        user->getHashMap()->at(runningThreadId)->upQuantum();
        user->getHashMap()->at(runningThreadId)->setState(RUNNING);
        timeBoot();
        unBlockSignals();
        siglongjmp(env[runningThreadId], 1);

    }else if(user->getHashMap()->at(tid)->getState() == READY){
        user->getLinkedList()->remove(tid);
    }
    user->getHashMap()->erase(tid);
    user->getMinHeap()->push(tid);
    unBlockSignals();
    return 0;
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
int uthread_block(int tid){
    blockSignals();
    if ((tid == 0) || (user->getHashMap()->find(tid) == user->getHashMap()->end())){
        cerr << ERROR_MSG + BAD_ARG_MSG << endl;
        unBlockSignals();
        return -1;
    }
    if (user->getHashMap()->at(tid)->getState() == RUNNING) {
        int runningThreadId = user->getLinkedList()->front();
        user->getLinkedList()->pop_front();
        //save current state **
        int ret_val = sigsetjmp(env[runningThreadId],1);
        if (ret_val == 1) {
            unBlockSignals();
            return 0;
        }
        user->getHashMap()->at(tid)->setState(BLOCKED);
        user->addQuantumNum();
        runningThreadId = user->getLinkedList()->front();
        user->getHashMap()->at(runningThreadId)->upQuantum();
        user->getHashMap()->at(runningThreadId)->setState(RUNNING);
        deleteSyncList(tid);
        timeBoot();
        unBlockSignals();
        siglongjmp(env[runningThreadId],1);
   } else if (user->getHashMap()->at(tid)->getState() == READY){
        user->getLinkedList()->remove(tid);
        user->getHashMap()->at(tid)->setState(BLOCKED);
    } else if (user->getHashMap()->at(tid)->getState() == SYNC_BLOCKED){
        user->getHashMap()->at(tid)->setState(BLOCKED_BOTH);
    }
    unBlockSignals();
    return 0;
}


/*
 * Description: This function resumes a blocked thread with ID tid and moves
 * it to the READY state. Resuming a thread in a RUNNING or READY state
 * has no effect and is not considered as an error. If no thread with
 * ID tid exists it is considered as an error.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_resume(int tid){
    blockSignals();
    if (user->getHashMap()->find(tid) == user->getHashMap()->end()){
        cerr << ERROR_MSG + BAD_ARG_MSG << endl;
        unBlockSignals();
        return -1;
    }
    if ((user->getHashMap()->at(tid)->getState() == BLOCKED)){
        makeThreadReady(tid);
    }else if ((user->getHashMap()->at(tid)->getState() == BLOCKED_BOTH)){
        user->getHashMap()->at(tid)->setState(SYNC_BLOCKED);
    }
    unBlockSignals();
    return 0;
}


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
int uthread_sync(int tid){
    blockSignals();
    if ((user->getLinkedList()->front() == 0) || (user->getLinkedList()->front() == tid)||
            (user->getHashMap()->find(tid) == user->getHashMap()->end())){
        cerr << ERROR_MSG + BAD_ARG_MSG << endl;
        unBlockSignals();
        return -1;
    }

    user->getHashMap()->at(tid)->addThreadToSyncList(user->getLinkedList()->front());
    int runningThreadId = user->getLinkedList()->front();
    user->getLinkedList()->pop_front();
    //save current state
    int ret_val = sigsetjmp(env[runningThreadId],1);
    if (ret_val == 1) {
        unBlockSignals();
        return 0;
    }

    deleteSyncList(runningThreadId);

    user->getHashMap()->at(runningThreadId)->setState(SYNC_BLOCKED);

    user->addQuantumNum();

    runningThreadId = user->getLinkedList()->front();

    user->getHashMap()->at(runningThreadId)->upQuantum();
    user->getHashMap()->at(runningThreadId)->setState(RUNNING);
    unBlockSignals();
    siglongjmp(env[runningThreadId],1);
}


/*
 * Description: This function returns the thread ID of the calling thread.
 * Return value: The ID of the calling thread.
*/
int uthread_get_tid(){
    return user->getLinkedList()->front();
}


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
    if (user->getHashMap()->find(tid) == user->getHashMap()->end()){
        return -1;
    } else{
        return user->getHashMap()->at(tid)->getQuantums();
    }
}


void printLinkedList(){
    std::list<int>::const_iterator iterator;
    for (iterator = user->getLinkedList()->begin(); iterator != user->getLinkedList()->end();
         ++iterator) {
        std::cout << *iterator << "<--";
    }
    cout << endl;
}