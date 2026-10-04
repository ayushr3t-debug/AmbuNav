#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string>

// Node of a singly linked list
struct LogNode {
    std::string entry;
    LogNode* next;
    explicit LogNode(const std::string& e) : entry(e), next(nullptr) {}
};

// A hand-built singly linked list used to keep a running event/route log
// (accidents reported, routes dispatched, reroutes triggered, etc.)
class RouteLog {
private:
    LogNode* head;
    LogNode* tail;
    int count;

public:
    RouteLog();
    ~RouteLog();

    void addEntry(const std::string& entry);
    void printLog() const;
    int size() const;
};

#endif
