#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <string>

struct Hospital {
    std::string name;
    std::string location; // junction name where hospital is situated
    int capacity;
    int currentPatients;
};

#endif
