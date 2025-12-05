#include "PriorityQueue.h"

#include <memory>
#include <stdexcept>


template<typename T>
PriorityQueue<T>::PriorityQueue() 
    : size(0) {
        
    // first one needs to be garbage/reserved
    vec.push_back(T());
}

template<typename T>
size_t PriorityQueue<T>::parent(size_t i) {
    return i / 2;
}

template<typename T>
size_t PriorityQueue<T>::left(size_t i) {
    return i * 2;
}

template<typename T>
size_t PriorityQueue<T>::right(size_t i) {
    return (i * 2) + 1;
}


//recursion
template<typename T>
void PriorityQueue<T>::shiftUp(size_t i) {

    if (i == 1) return;

    // less than because min heap
    if(vec[i] < vec[parent(i)]) {
        std::swap(vec[parent(i)], vec[i]);
    }

    shiftUp(parent(i)); 
    
}


template<typename T>
void PriorityQueue<T>::shiftDown(size_t i) {
        
    size_t swapId = i;

    if (left(i) <= size && vec[i] > vec[left(i)]) {
        swapId = left(i);
    }

    if (right(i) <= size && vec[swapId] > vec[right(i)]) {
        swapId = right(i);
    }

    if (swapId != i) {
        std::swap(vec[i], vec[swapId]);
        shiftDown(swapId);
    }

    return;
}


template<typename T>
void PriorityQueue<T>::insert(const T& newVal) {

    if (size + 1 >= vec.getSize()) {
        vec.push_back(T()); //placeholder
    }

    vec[++size] = newVal;
    shiftUp(size);

    return;
}



template<typename T>
T PriorityQueue<T>::pop() {
    T maxVal = vec[1];
    
    std::swap(vec[1], vec[size--]);
    shiftDown(1);

    return maxVal;
}


template<typename T>
const T& PriorityQueue<T>::top() const {
    return vec[1];
}




template<typename T>
bool PriorityQueue<T>::isEmpty() const {
    return size == 0;
}




