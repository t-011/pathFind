#include "Map.h"



template<typename K, typename V>
int Map<K, V>::hash(const K& key) const{

    return int(std::hash<K>{}(key) % capacity);

}


//will grow when >= 50% full
template<typename K, typename V>
void Map<K, V>::maybeResize() {
    
    if (count * 2 >= capacity) {

        int newCap = capacity * 2;
        Vector<Entry> newTable(newCap);

        //rehash
        for (const auto& entry : table) {
            if (entry.occupied) {
                int idx = int(std::hash<K>{}(entry.key) % newCap);

                while (newTable[idx].occupied) {
                    idx = (idx + 1) % newCap;
                }

                newTable[idx] = entry;
                newTable[idx].occupied = true;
            }
        }

        table = newTable;
        capacity = newCap;
    }

    return;
}

template<typename K, typename V>
Map<K, V>::Map(int cap)
    : capacity(cap), count(0)
{

    if (capacity < 1) capacity = 1;
    
    Vector<Entry> newTable(capacity);
    table = newTable;
}


template<typename K, typename V>
bool Map<K, V>::contains(const K& key) const {

    int idx = hash(key);

    bool found = false;
    while (!found && table[idx].occupied) {
        if (table[idx].key == key) {
            found = true;
        }
        else {
            idx = (idx + 1) % capacity;
        }
    }

    return found;
}


template<typename K, typename V>
V& Map<K, V>::operator[](const K& key) {

    int idx = hash(key);
    
    bool found = false;
    while (!found && table[idx].occupied) {
        if (table[idx].key == key) {
            found = true;
        }
        else {
            idx = (idx + 1) % capacity;
        }
    }

    if (!found) {
        maybeResize();

        idx = hash(key);

        while (table[idx].occupied) {
            idx = (idx + 1) % capacity;
        }

        table[idx].key = key;
        table[idx].value = V();
        table[idx].occupied = true;
        ++count;
    }

    return table[idx].value;

}