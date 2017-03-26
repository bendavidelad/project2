#include "Thread.h"


Thread::Thread(void (*func)(void)){
    state = READY;
    quantums_num = 0;
    function = func;
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

}

