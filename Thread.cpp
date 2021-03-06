#include <stdexcept>
#include <stdlib.h>
#include <malloc.h>
#include "Thread.h"
#define STACK_SIZE 4096 /* stack size per thread (in bytes) */

static const std::string ERROR_MSG = "system error:";
using namespace std;

Thread::Thread(void (*func)(void)):syncList(new std::list<int>){
    state = READY;
    quantums_num = 0;
    function = func;
    stack = (char*)malloc(STACK_SIZE);
};

int Thread::getState(){
    return state;
}

int Thread::getQuantums(){
    return quantums_num;
}

voidFn Thread::getFunction(){
    return function;
}

Thread::Thread(const Thread &&thread) {
//Todo
}

Thread::~Thread() {
    free (stack);
}

void Thread::setId(int id) {
    this->id = id;
}

void Thread::setState(int state) {
    if (state > 4 || state < 0 ){
        throw std::invalid_argument(ERROR_MSG + "bad argument");
    }
    this->state = state;
}

void Thread::setEnvBuf (){
    sigsetjmp(envBuf , 0);
}

void Thread::upQuantum() {
    quantums_num++;
}

void Thread::bootSyncList() {
    syncList->clear();
}

std::shared_ptr<std::list<int>> Thread::getSyncList() {
    return syncList;
}

void Thread::addThreadToSyncList(int tid) {
    bool exist = false;
    for (auto it = syncList->begin(); it != syncList->end(); ++it) {
        if (*it == tid) {
            exist = true;
            break;
        }
    }
        if (exist){
            return;
        } else{
            syncList->push_back(tid);
    }

}

char* Thread::getStack() {
    return stack;
}

