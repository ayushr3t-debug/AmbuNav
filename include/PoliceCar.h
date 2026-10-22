#ifndef POLICECAR_H
#define POLICECAR_H

#include "Vehicle.h"

// PoliceCar inherits Vehicle and overrides getPriority()/getType()
class PoliceCar : public Vehicle {
public:
    PoliceCar(const std::string& id, const std::string& location);

    int getPriority() const override;
    std::string getType() const override;
};

#endif
