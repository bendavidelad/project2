

#ifndef PROJECT2_THREAD_H
#define PROJECT2_THREAD_H

static const int READY = 0;
static const int RUNNING = 1;
static const int BLOCKED = 2;
typedef void (*voidFn)();


class Thread {

    Thread(void (*func)(void));

    int getState();

    int getQuantums();

    voidFn getFunction();

protected:
    int state;
    int quantums_num;
    void (*function)(void);

};


#endif //PROJECT2_THREAD_H
