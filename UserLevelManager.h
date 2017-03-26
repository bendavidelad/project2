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

struct comparator {
    bool operator()(int i, int j) {
        return i > j;
    }
};

class UserLevelManager {
public:
    UserLevelManager(int maxNumOfThreads){
        hashMap = new unordered_map<int, int>;

    }


private:
    priority_queue<int, std::vector<int>, comparator> minHeap;


    list<int> linkedList;

    unordered_map<int , int> *hashMap;


    //std:pair<int,int> shopping(3,4);fdfd


};


#endif //OS2_USERLEVELMANAGER_H
