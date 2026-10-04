#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <list>
#include <vector>
#include <utility>

// Custom hash table (separate chaining) used to map a junction / vehicle
// name (string key) to its integer index in O(1) average time.
// This is a hand-built implementation (not unordered_map) so that the
// "Hashing" concept from the Data Structures course is explicitly shown.
class HashTable {
private:
    std::vector<std::list<std::pair<std::string, int>>> buckets; // chaining via linked lists
    int capacity;
    int count;

    unsigned long hashFunction(const std::string& key) const;

public:
    explicit HashTable(int cap = 101);

    void insert(const std::string& key, int value);
    bool get(const std::string& key, int& value) const;
    bool contains(const std::string& key) const;
    int size() const;
    double loadFactor() const;
};

#endif
