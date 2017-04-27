//
// Created by shaked571 on 3/26/17.
//

#ifndef OS2_USERLEVELMANAGER_H
#define OS2_USERLEVELMANAGER_H

#include <queue>
#include <iostream>
#include <list>
#include <unordered_map>
#include "Thread.h"
#include <memory>
#define MAX_THREAD_NUM 100 /* maximal number of threads */

//typedef std::vector<std::shared_ptr<int>> threadPtrPrVec;

static const std::string ERROR_MSG = "system error:";
static const std::string BAD_ARG_MSG = "bad arg occurred";
static const std::string BAD_ALLOC_MSG = "bad allocation occurred";
struct comparator {
    bool operator()(int i, int j) {
        return i > j;
    }
};
typedef std::priority_queue< int, std::vector<int>, comparator> minHeap;

class UserLevelManager {
public:

    /**
    *  A default constructor that have the default maximun number of threads
    */
    UserLevelManager();
    /**
     * A copy constructor
     * @param user the usrtLevelManager which copies
     */
    UserLevelManager(const UserLevelManager &user);
    /**
     * A move constructor
     * @param matrix the matrix which move
     */
    UserLevelManager(const UserLevelManager && user);

    UserLevelManager& operator=(const UserLevelManager& other);


    /**
     * A Destructor.
     * Destruct the point object.
     */
    ~UserLevelManager();



    /**
     *
     * @param maxNumOfThreads
     * @return
     */
    UserLevelManager(unsigned int quantum_usecs);

    /**
     *
     * @return
     */
    std::shared_ptr<std::priority_queue<int, std::vector<int>, comparator>> getMinHeap();
    /**
     *
     * @return
     */
    std::shared_ptr<std::list<int>> getLinkedList();
    /**
     *
     * @return
     */
    std::shared_ptr<std::unordered_map<int, std::shared_ptr<Thread>>> getHashMap();
    /**
     *
     */
    void addQuantumNum();
    /**
     *
     * @return
     */
    int getQuantumNum();

    unsigned int getThreadCounter();

    void addThreadCounter();

    int getMaxthreadNum();





private:
    // the stl that contains the deleted threads id's
    std::shared_ptr<std::priority_queue<int, std::vector<int>, comparator>> minHeapDeletedThreads;
    //contains the ready states threads
    std::shared_ptr<std::list<int>> linkedList;
    //contains all the threads
    std::shared_ptr<std::unordered_map<int,std::shared_ptr<Thread>>> hashMap;
    int numOfQuantum;
    int maxThreadsNum;
    unsigned int threadCounter;
    unsigned int quantomUsecs;
};


#endif //OS2_USERLEVELMANAGER_H
