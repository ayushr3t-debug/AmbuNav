#include "../include/Vehicle.h"

Vehicle::Vehicle(const std::string& id, const std::string& location, double speed)
    : id(id), currentLocation(location), speedKmph(speed) {}

std::string Vehicle::getId() const { return id; }
std::string Vehicle::getLocation() const { return currentLocation; }
void Vehicle::setLocation(const std::string& loc) { currentLocation = loc; }
double Vehicle::getSpeed() const { return speedKmph; }
