#ifndef TRAFFICREPORT_H
#define TRAFFICREPORT_H

#include <iostream>
#include <string>
#include <iomanip>

class TrafficReport {
private:
    int totalVehicles;
    int regularVehicles;
    int emergencyVehicles;

public:
    TrafficReport()
        : totalVehicles(0),
          regularVehicles(0),
          emergencyVehicles(0) {}

    void recordRegularVehicle() {
        regularVehicles++;
        totalVehicles++;
    }

    void recordEmergencyVehicle() {
        emergencyVehicles++;
        totalVehicles++;
    }

    double getEmergencyPercentage() const {
        if (totalVehicles == 0)
            return 0.0;

        return (emergencyVehicles * 100.0) / totalVehicles;
    }

    void displayReport() const {
        std::cout << "\n===== TRAFFIC REPORT =====\n";
        std::cout << "Total Vehicles: "
                  << totalVehicles << '\n';

        std::cout << "Regular Vehicles: "
                  << regularVehicles << '\n';

        std::cout << "Emergency Vehicles: "
                  << emergencyVehicles << '\n';

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Emergency Vehicle Percentage: "
                  << getEmergencyPercentage() << "%\n";

        std::cout << "==========================\n";
    }
};

#endif
