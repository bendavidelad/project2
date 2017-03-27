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

//typedef std::vector<std::shared_ptr<int>> threadPtrPrVec;

static const std::string ERROR_MSG = "system error:";
static const std::string BAD_ALLOC_MSG = "bad allocation occur";
struct comparator {
    bool operator()(int i, int j) {
        return i > j;
    }
};
typedef std::priority_queue<int, std::vector<int>, comparator> minHeap;

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
    UserLevelManager(int maxNumOfThreads);

    /**
     *
     * @return
     */
    minHeap getMinHeap();
    /**
     *
     * @return
     */
    std::list<int>* getLinkedList();
    /**
     *
     * @return
     */
    std::unordered_map<int, int> getHashMap();
    /**
     *
     */
    void addQuantumNum();
    /**
     *
     * @return
     */
    int getQuantumNum();





private:
    // the stl that contains the deleted threads id's
    minHeap *minHeapDeletedThreads;
    //contains the ready states threads
    std::list<int> *linkedList;
    //contains all the threads
    std::unordered_map<int ,std::shared_ptr<Thread>> *hashMap;
    int numOfQuantum;
    int maxThredsNum;

    //std:pair<int,int> shopping(3,4);TODO font forget to kaki

};


#endif //OS2_USERLEVELMANAGER_H
