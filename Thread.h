

#ifndef PROJECT2_THREAD_H
#define PROJECT2_THREAD_H

#include <setjmp.h>
#include <list>

static const int READY = 0;
static const int RUNNING = 1;
static const int BLOCKED = 2;
typedef void (*voidFn)();


class Thread {

public:
    /**
     * A copy constructor
     * @param thread the thread which copies
     */
    Thread(const Thread &thread);
    /**
     * A move constructor
     * @param thread the thread which move
     */
    Thread(const Thread && thread);
    /**
     * A Destructor.
     * Destruct the thread object.
     */
    ~Thread();

    Thread(void (*func)(void));

    int getState();

    int getQuantums();

    void upQuantum();

    voidFn getFunction();

    void setId(int id);

    void setState(int state);

    void setEnvBuf ();

    std::list<int> getSyncList();

<<<<<<< HEAD
    void addThreadToSyncList(int tid){

    }
=======
    void addThreadToSyncList(int tid);
>>>>>>> 109319fcd29a8548d3fbf6cf92621f47b838ba64

    void bootSyncList();
private:

    int state;
    int id;
    int quantums_num;
    void (*function)(void);
    void* stack;
    sigjmp_buf envBuf;
    std::list<int> *syncList;

};


#endif //PROJECT2_THREAD_H
