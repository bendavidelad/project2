#include "UserLevelManager.h"

UserLevelManager::UserLevelManager(int maxNumOfThreads) {
    hashMap = new unordered_map<int, int>;
    linkedList = new list<int>;
    minHeapDeletedThreads = new priority_queue<int, vector<int>, comparator>;
    numOfQuantum = 0;
    if (hashMap &&  linkedList && minHeapDeletedThreads){
        return 0;
    }else{
        return -1;
    }
}

