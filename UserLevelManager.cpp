#include "UserLevelManager.h"



UserLevelManager::UserLevelManager(int maxNumOfThreads) {
    numOfQuantum = 0;
    maxThredsNum = maxNumOfThreads;
    try {
        hashMap = new unordered_map<int, int>;
        linkedList = new list<int>;
        minHeapDeletedThreads = new priority_queue<int, vector<int>, comparator>;


}

minHeap UserLevelManager::getMinHeap() {
    return *minHeapDeletedThreads;
}

list<int> *UserLevelManager::getLinkedList() {
    return nullptr;
}

unordered_map<int, int> UserLevelManager::getHashMap() {
    return *hashMap;
}

void UserLevelManager::addQuantumNum() {
    numOfQuantum++;
}

int UserLevelManager::getQuantumNum() {
    return numOfQuantum;
}

UserLevelManager::UserLevelManager() {

}

UserLevelManager::UserLevelManager(const UserLevelManager &&user) {

}

UserLevelManager::UserLevelManager(const UserLevelManager &user) {

}

UserLevelManager::~UserLevelManager() {
    delete this->hashMap;
    delete this->linkedList;
    delete this->minHeapDeletedThreads;
}

