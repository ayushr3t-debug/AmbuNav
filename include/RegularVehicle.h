#ifndef REGULARVEHICLE_H
#define REGULARVEHICLE_H

#include "Vehicle.h"

// Another Vehicle subtype -> shows a second override of the same
// virtual interface (polymorphism: same call, different behaviour).
class RegularVehicle : public Vehicle {
public:
    RegularVehicle(const std::string& id, const std::string& location);

    int getPriority() const override;  // low priority
    std::string getType() const override;
};

#endif
