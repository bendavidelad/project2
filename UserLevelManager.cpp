#include "UserLevelManager.h"


using namespace std;
UserLevelManager::UserLevelManager(unsigned int quantum_usecs) {
    numOfQuantum = 0;
    threadCounter = 0;
    maxThredsNum = MAX_THREAD_NUM;
    quantomUsecs = quantum_usecs;
    hashMap = new unordered_map<int, shared_ptr<Thread>>;
    linkedList = new list<int>;
    minHeapDeletedThreads = new priority_queue<int, vector<int>, comparator>;
}
minHeap UserLevelManager::getMinHeap() {
    return *minHeapDeletedThreads;
}

list<int> *UserLevelManager::getLinkedList() {
    return nullptr;
}

unordered_map<int, shared_ptr<Thread>> UserLevelManager::getHashMap() {
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

UserLevelManager &UserLevelManager::operator=(const UserLevelManager &other) {
    this->hashMap = other.hashMap;
    this->linkedList = other.linkedList;
    this->maxThredsNum = other.maxThredsNum;
    this->minHeapDeletedThreads = other.minHeapDeletedThreads;
    this->numOfQuantum = other.numOfQuantum;
    return *this;
}

unsigned int UserLevelManager::getThreadCounter() {
    return threadCounter;
}

void UserLevelManager::addThreadCounter() {
    threadCounter++;
}

int UserLevelManager::getMaxthreadNum() {
    return maxThredsNum;
}

