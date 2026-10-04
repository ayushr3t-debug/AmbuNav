#include "../include/Ambulance.h"

Ambulance::Ambulance(const std::string& id, const std::string& location)
    : Vehicle(id, location, 60.0), emergencyActive(false) {}

int Ambulance::getPriority() const { return 100; } // highest priority
std::string Ambulance::getType() const { return "Ambulance"; }

void Ambulance::activateEmergency() { emergencyActive = true; }
void Ambulance::deactivateEmergency() { emergencyActive = false; }
bool Ambulance::isEmergencyActive() const { return emergencyActive; }
