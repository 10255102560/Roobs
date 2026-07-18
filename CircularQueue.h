#pragma once
#include <vector>
#include <stdexcept>
using namespace std;

template<typename T>
class CircularQueue {
    vector<T> data;
    int head;
    int tail;
    int cnt;
    int cap;

public:
    explicit CircularQueue(int capacity) : data(capacity), head(0), tail(0), cnt(0), cap(capacity) {}

    bool enqueue(const T& item) {
        if (full()) return false;
        data[tail] = item;
        tail = (tail + 1) % cap;
        cnt++;
        return true;
    }

    T dequeue() {
        if (empty()) throw runtime_error("CircularQueue: dequeue from empty queue");
        T item = data[head];
        head = (head + 1) % cap;
        cnt--;
        return item;
    }

    T& front() {
        if (empty()) throw runtime_error("CircularQueue: front on empty queue");
        return data[head];
    }

    const T& front() const {
        if (empty()) throw runtime_error("CircularQueue: front on empty queue");
        return data[head];
    }

    bool empty() const { return cnt == 0; }
    bool full() const { return cnt == cap; }
    int size() const { return cnt; }
    int capacity() const { return cap; }
};
