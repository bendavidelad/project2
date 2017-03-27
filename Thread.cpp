#include <stdexcept>
#include <stdlib.h>
#include <malloc.h>
#include "Thread.h"
#define STACK_SIZE 4096 /* stack size per thread (in bytes) */

static const std::string ERROR_MSG = "system error:";

Thread::Thread(void (*func)(void)){
    state = READY;
    quantums_num = 0;
    function = func;
    stack = malloc(STACK_SIZE);
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
    if (state>2 || state < 0 ){
        throw std::invalid_argument(ERROR_MSG + "bad argument");
    }
    this->state = state;
}

