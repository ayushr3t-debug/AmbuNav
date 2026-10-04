#include "../include/RegularVehicle.h"

RegularVehicle::RegularVehicle(const std::string& id, const std::string& location)
    : Vehicle(id, location, 40.0) {}

int RegularVehicle::getPriority() const { return 1; }
std::string RegularVehicle::getType() const { return "Regular"; }
