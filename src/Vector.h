#ifndef VECT_H
#define VECT_H

#include <cstddef>
#include <utility>
#include <vector>



/**
 * @brief A lightweight dynamic array class similar to std::vector.
 * @tparam T Type of elements stored in the vector.
 */
template<typename T>
class Vector {
    private:

        static constexpr size_t MINIMUM_CAP = 2;
        const int GROW_FACTOR = 2;

        T* data = nullptr;
        size_t capacity = 0;
        size_t size = 0;

        
    private:
    /**
     * @brief Reallocates memory to a new capacity and moves existing elements.
     * @param newCapacity The new capacity to allocate.
     */
    void reAlloc(size_t newCapacity);

    public:
        /**
         * @brief Default constructor. Creates an empty vector.
         */
        Vector();

        /**
         * @brief Constructs a vector with a given initial size.
         * @param size Number of elements to allocate.
         */
        Vector(size_t size);

        /**
         * @brief Copy constructor.
         * @param other The vector to copy from.
         */
        Vector(const Vector& other);

        /**
         * @brief Constructs from an std::vector.
         * @param v The std::vector to copy data from.
         */
        Vector(const std::vector<T>& v);

        /**
         * @brief Destructor. Frees allocated memory.
         */
        ~Vector();

        /**
         * @brief Copy assignment operator.
         * @param other The vector to assign from.
         * @return Reference to this vector.
         */
        Vector& operator=(const Vector& other);

        /**
         * @brief Assigns from an std::vector.
         * @param v The std::vector to copy data from.
         * @return Reference to this vector.
         */
        Vector& operator=(const std::vector<T>& v);

        /**
         * @brief Adds a new element to the end of the vector (copy version).
         * @param value The value to add.
         */
        void push_back(const T& value);

        /**
         * @brief Adds a new element to the end of the vector (move version).
         * @param value The value to move into the vector.
         */
        void push_back(T&& value);

        /**
         * @brief Removes the last element of the vector.
         */
        void pop_back();

        /**
         * @brief Clears all elements from the vector.
         */
        void clear();

        /**
         * @brief Returns the number of elements currently stored.
         * @return The number of elements in the vector.
         */
        size_t getSize() const;

        /**
         * @brief Access element at given index (const).
         * @param index Position of the element.
         * @return Const reference to the element.
         * @throws std::runtime_error if index is out of bounds.
         */
        const T& operator[](size_t index) const;

        /**
         * @brief Access element at given index (non-const).
         * @param index Position of the element.
         * @return Reference to the element.
         * @throws std::runtime_error if index is out of bounds.
         */
        T& operator[](size_t index);

        /**
         * @brief Returns pointer to the beginning of the data.
         * @return Pointer to the first element.
         */
        T* begin();

        /**
         * @brief Returns pointer past the last element.
         * @return Pointer to one-past-the-end element.
         */
        T* end();

        /**
         * @brief Returns const pointer to the beginning of the data.
         * @return Const pointer to the first element.
         */
        const T* begin() const;

        /**
         * @brief Returns const pointer past the last element.
         * @return Const pointer to one-past-the-end element.
         */
        const T* end() const;

};


#include "Vector.cpp"


#endif