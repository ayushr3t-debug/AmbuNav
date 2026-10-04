#include "../include/LinkedList.h"
#include <iostream>

RouteLog::RouteLog() : head(nullptr), tail(nullptr), count(0) {}

RouteLog::~RouteLog() {
    LogNode* cur = head;
    while (cur) {
        LogNode* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
}

void RouteLog::addEntry(const std::string& entry) {
    LogNode* node = new LogNode(entry);
    if (!head) {
        head = tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    count++;
}

void RouteLog::printLog() const {
    std::cout << "\n----- ROUTE / EVENT LOG (" << count << " entries) -----\n";
    LogNode* cur = head;
    int i = 1;
    while (cur) {
        std::cout << "[" << i++ << "] " << cur->entry << "\n";
        cur = cur->next;
    }
    std::cout << "---------------------------------------------\n";
}

int RouteLog::size() const {
    return count;
}
