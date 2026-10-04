#ifndef AMBULANCE_H
#define AMBULANCE_H

#include "Vehicle.h"

// Ambulance inherits Vehicle and overrides getPriority()/getType()
// -> highest routing priority + eligible for green corridor.
class Ambulance : public Vehicle {
private:
    bool emergencyActive;

public:
    Ambulance(const std::string& id, const std::string& location);

    int getPriority() const override;     // always returns highest priority
    std::string getType() const override;

    void activateEmergency();
    void deactivateEmergency();
    bool isEmergencyActive() const;
};

#endif
