#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <memory>

/**
 * @brief A generic Min-Heap based priority queue implementation.
 * 
 * Elements are stored in a heap-ordered structure using an underlying Vector.
 * The smallest element (based on operator<) has the highest priority.
 * 
 * @tparam T Type of elements stored in the priority queue.
 */
template<typename T>
class PriorityQueue {
    private:
        size_t size = 0;
        Vector<T> vec{};

        /**
         * @brief Returns the index of the parent node for a given index.
         * @param i Current node index.
         * @return Index of the parent node.
         */
        size_t parent(size_t i);

        /**
         * @brief Returns the index of the left child node.
         * @param i Current node index.
         * @return Index of the left child.
         */
        size_t left(size_t i);

        /**
         * @brief Returns the index of the right child node.
         * @param i Current node index.
         * @return Index of the right child.
         */
        size_t right(size_t i);

        /**
         * @brief Moves an element up the heap to restore min-heap property.
         * @param i Index of the node to shift up.
         */
        void shiftUp(size_t i);

        /**
         * @brief Moves an element down the heap to restore min-heap property.
         * @param i Index of the node to shift down.
         */
        void shiftDown(size_t i);

    public:
        /**
         * @brief Default constructor. Initializes an empty priority queue.
         */
        PriorityQueue();

        /**
         * @brief Checks whether the priority queue is empty.
         * @return True if the queue is empty, false otherwise.
         */
        bool isEmpty() const;

        /**
         * @brief Inserts a new element into the priority queue.
         * @param value The value to insert.
         */
        void insert(const T& value);

        /**
         * @brief Retrieves the element with the highest priority (smallest).
         * @return Const reference to the top element.
         * @throws std::runtime_error if the queue is empty.
         */
        const T& top() const;

        /**
         * @brief Removes and returns the element with the highest priority.
         * @return The removed element (smallest).
         * @throws std::runtime_error if the queue is empty.
         */
        T pop();
};


#include "PriorityQueue.cpp"



#endif