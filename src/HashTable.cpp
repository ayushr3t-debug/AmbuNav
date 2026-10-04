#include "../include/HashTable.h"

HashTable::HashTable(int cap) : capacity(cap), count(0) {
    buckets.resize(capacity);
}

// djb2 string hashing algorithm
unsigned long HashTable::hashFunction(const std::string& key) const {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c); // hash * 33 + c
    }
    return hash % capacity;
}

void HashTable::insert(const std::string& key, int value) {
    unsigned long idx = hashFunction(key);
    for (auto& pr : buckets[idx]) {
        if (pr.first == key) {        // key already exists -> update
            pr.second = value;
            return;
        }
    }
    buckets[idx].push_back({key, value});
    count++;
}

bool HashTable::get(const std::string& key, int& value) const {
    unsigned long idx = hashFunction(key);
    for (const auto& pr : buckets[idx]) {
        if (pr.first == key) {
            value = pr.second;
            return true;
        }
    }
    return false;
}

bool HashTable::contains(const std::string& key) const {
    int dummy;
    return get(key, dummy);
}

int HashTable::size() const {
    return count;
}

double HashTable::loadFactor() const {
    return static_cast<double>(count) / capacity;
}
