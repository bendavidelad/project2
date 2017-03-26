//
// Created by shaked571 on 3/26/17.
//

#ifndef OS2_USERLEVELMANAGER_H
#define OS2_USERLEVELMANAGER_H

#include <queue>
#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;
typedef priority_queue<int, vector<int>, comparator> minHeap;
struct comparator {
    bool operator()(int i, int j) {
        return i > j;
    }
};

class UserLevelManager {
public:
    /**
     * the construuctor
     * @param maxNumOfThreads
     * @return null
     */
    UserLevelManager(int maxNumOfThreads);

    minHeap getMinHeap();

    list<int> getLinkedList();

    unordered_map<int, int> getHashMap();

    void addQuantumNum(){
        numOfQuantum++;
    }

    int getQuantumNum(){
        return numOfQuantum;
    }



private:
    // the stl that contains the deleted threads id's
    minHeap *minHeapDeletedThreads;
    //contains the ready states threads
    list<int> *linkedList;
    //contains all the threads
    unordered_map<int , int> *hashMap;
    int numOfQuantum;

    //std:pair<int,int> shopping(3,4);TODO font forget to kaki

};


#endif //OS2_USERLEVELMANAGER_H
