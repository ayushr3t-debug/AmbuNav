#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

// Abstract base class — demonstrates encapsulation (private data, public
// accessors) and sets up polymorphism via the pure virtual getPriority().
class Vehicle {
protected:
    std::string id;
    std::string currentLocation;
    double speedKmph;

public:
    Vehicle(const std::string& id, const std::string& location, double speed = 40.0);
    virtual ~Vehicle() = default;

    std::string getId() const;
    std::string getLocation() const;
    void setLocation(const std::string& loc);
    double getSpeed() const;

    // Pure virtual -> every derived vehicle type must define its own
    // priority and type. This is runtime polymorphism in action.
    virtual int getPriority() const = 0;
    virtual std::string getType() const = 0;
};

#endif
