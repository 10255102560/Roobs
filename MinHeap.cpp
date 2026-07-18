#include "MinHeap.h"

void MinHeap::push(double d, int n) {
    heap.push_back({d, n});
    int i = heap.size() - 1;
    while (i > 0 && heap[(i-1)/2].dist > heap[i].dist) {
        swap(heap[i], heap[(i-1)/2]);
        i = (i-1)/2;
    }
}

HeapNode MinHeap::pop() {
    HeapNode top = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    int i = 0;
    while (true) {
        int left = 2*i+1, right = 2*i+2, smallest = i;
        if (left < static_cast<int>(heap.size()) && heap[left].dist < heap[smallest].dist) smallest = left;
        if (right < static_cast<int>(heap.size()) && heap[right].dist < heap[smallest].dist) smallest = right;
        if (smallest == i) break;
        swap(heap[i], heap[smallest]);
        i = smallest;
    }
    return top;
}

bool MinHeap::empty() {
    return heap.empty();
}