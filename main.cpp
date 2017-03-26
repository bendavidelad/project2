
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
    hashMap.insert(shopping);
    cout << hashMap.at(3) << endl;
//    cout << hashMap.at(4) << endl;

}