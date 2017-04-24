
#include <queue>
#include <iostream>
#include <list>
#include <unordered_map>
#include "uthreads.h"
using namespace std;

struct comparator {
    bool operator()(int i, int j) {
        return i > j;
    }
};

int main(int argc, char const *argv[])
{
    priority_queue<int, std::vector<int>, comparator> minHeap;
    minHeap.push(19);
    minHeap.push(29);
    minHeap.push(39);

    minHeap.push(9);
    minHeap.push(59);
    minHeap.push(99);

    cout << minHeap.top() << endl;
    minHeap.pop();
    cout << minHeap.top() << endl;
    minHeap.pop();


    std::list<int> linkedList;
    linkedList.push_back(5);
    linkedList.push_front(6);
    cout <<linkedList.front() << endl;


    std::unordered_map<int , int> hashMap;
    std:pair<int,int> shopping(3,4);
    pair<int,int> shopping1(5,4);
    hashMap.insert(shopping);
    hashMap.insert(shopping1);
    cout << hashMap.at(3) << endl;
<<<<<<< HEAD
    cout << hashMap.erase(6) << endl;
=======
//    cout << hashMap.at(4) << endl;
    try {


   uthread_init(89);
    }catch (const std::exception& e){
        printf("dddd");
    }


//    uthread_init(986);
>>>>>>> 8a75c9b4e443b20fa233fcc6b175b3b42adff975

    }