#pragma once
#include <vector>
using namespace std;

struct HeapNode {
    double dist;
    int node;
};

class MinHeap {
    vector<HeapNode> heap;
public:
    void push(double d, int n);
    HeapNode pop();
    bool empty();
    int size() const { return static_cast<int>(heap.size()); }
};
