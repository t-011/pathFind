#include "Vector.h"

#include <stdexcept>
#include <utility>

template<typename T>
void Vector<T>::reAlloc(size_t newCapacity) {

    T* newBlock = new T[newCapacity];

    if (newCapacity < size) {
        size = newCapacity;
    }

    for (size_t i = 0; i < size; ++i) {
        newBlock[i] = std::move(data[i]);
    }

    delete[] data;
    data = newBlock;
    capacity = newCapacity;

    return;
}


template<typename T>
Vector<T>::Vector() 
    : data(nullptr), capacity(0), size(0)
{
    reAlloc(MINIMUM_CAP);
}

template<typename T>
Vector<T>::Vector(size_t size) 
    : data(new T[size]()), capacity(size), size(size)  {}


template<typename T>
Vector<T>::~Vector() {
    delete[] data;
}

template<typename T>
Vector<T>::Vector(const Vector& other)
    : data(nullptr), capacity(other.capacity), size(other.size)
{
    data = new T[capacity];
    for (size_t i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
}

template<typename T>
Vector<T>::Vector(const std::vector<T>& v) {

    size = v.size();
    capacity = v.capacity();
    data = new T[capacity];
    std::copy(v.begin(), v.end(), data);
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    
    if (this != &other) {
        delete[] data;

        size = other.size;
        capacity = other.capacity;

        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const std::vector<T>& v) {
    
    delete[] data;
    size = v.size();
    capacity = v.capacity();
    data = new T[capacity];
    std::copy(v.begin(), v.end(), data);
    
    return *this;
}

template<typename T>
void Vector<T>::push_back(const T& value) {

    if (size >= capacity) {
        reAlloc((capacity == 0 ? MINIMUM_CAP : capacity) * GROW_FACTOR);
    }

    data[size] = value;
    ++size;
}

template<typename T>
void Vector<T>::push_back(T&& value) {

    if (size >= capacity) {
        reAlloc((capacity == 0 ? MINIMUM_CAP : capacity) * GROW_FACTOR);
    }

    data[size] = std::move(value);
    ++size;
}


template<typename T>
void Vector<T>::pop_back() {

    if (size > 0) {
        --size;
    }

    return;
}


template<typename T>
void Vector<T>::clear() {

    size = 0;
    return;
}

template<typename T>
size_t Vector<T>::getSize() const {
    return size;
}


template<typename T>
const T& Vector<T>::operator[](size_t index) const {

    if (index >= size) {
        throw std::runtime_error("Out of bounds access");
    }

    return data[index];
}


template<typename T>
T& Vector<T>::operator[](size_t index)  {

    if (index >= size) {
        throw std::runtime_error("Out of bounds access");
    }

    return data[index];
}


template<typename T>
T* Vector<T>::begin() {
    return data; 
}

template<typename T>
T* Vector<T>::end() { 
    return data + size;
}

template<typename T>
const T* Vector<T>::begin() const {
    return data; 
}

template<typename T>
const T* Vector<T>::end() const { 
    return data + size; 
}

