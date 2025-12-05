#ifndef MAP_H
#define MAP_H


#include "Vector.h"


/**
 * @brief A simple hash map implementation using open addressing.
 * 
 * Uses linear probing for collision resolution and automatically resizes
 * when the table is more than 50% full.
 * 
 * @tparam K Type of the key.
 * @tparam V Type of the value.
 */
template<typename K, typename V>
class Map {
    private:
        /**
         * @brief Internal structure representing a key-value pair.
         */
        struct Entry {
            K key{};
            V value{};
            bool occupied = false; 
        };

        Vector<Entry> table{};
        int capacity;
        int count;

        /**
         * @brief Computes a hash index for a given key.
         * @param key The key to hash.
         * @return Index in the hash table.
         */
        int hash(const K& key) const;

        /**
         * @brief Resizes the hash table if it becomes more than 50% full.
         * 
         * Rehashes all existing entries into the new table.
         */
        void maybeResize();

    public:
        /**
         * @brief Constructs a map with an initial capacity.
         * @param cap Initial table capacity (defaults to 8).
         */
        Map(int cap = 8);

        /**
         * @brief Checks whether the map contains a given key.
         * @param key The key to search for.
         * @return True if the key exists, false otherwise.
         */
        bool contains(const K& key) const;

        /**
         * @brief Accesses the value associated with a key.
         * 
         * If the key does not exist, it is inserted with a default value.
         * 
         * @param key The key to look up or insert.
         * @return Reference to the value associated with the key.
         */
        V& operator[](const K& key);
};



#include "Map.cpp"


#endif 