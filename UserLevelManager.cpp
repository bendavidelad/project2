#include "UserLevelManager.h"


using namespace std;
UserLevelManager::UserLevelManager(unsigned int quantum_usecs) : hashMap(new unordered_map<int , shared_ptr<Thread>>),
                                                                 minHeapDeletedThreads(new priority_queue<int, vector<int>, comparator>)
{
    numOfQuantum = 0;
    threadCounter = 0;
    maxThreadsNum = MAX_THREAD_NUM;
    quantomUsecs = quantum_usecs;
    linkedList = new list<int>;
}
std::shared_ptr<std::priority_queue<int, std::vector<int>, comparator>> UserLevelManager::getMinHeap() {
    return minHeapDeletedThreads;
}

list<int>* UserLevelManager::getLinkedList() {
    return linkedList;
}

std::shared_ptr<std::unordered_map<int, std::shared_ptr<Thread>>> UserLevelManager::getHashMap() {
    return hashMap;
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
    delete this->linkedList;
}

UserLevelManager &UserLevelManager::operator=(const UserLevelManager &other) {
    this->hashMap = other.hashMap;
    this->linkedList = other.linkedList;
    this->maxThreadsNum = other.maxThreadsNum;
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
    return maxThreadsNum;
}

