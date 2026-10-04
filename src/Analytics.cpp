#include "../include/Analytics.h"
#include <iostream>
#include <iomanip>

TripResult compareRoutes(double staticTime, double dynamicTime) {
    TripResult r;
    r.staticTime = staticTime;
    r.dynamicTime = dynamicTime;
    if (staticTime > 0)
        r.improvementPercent = ((staticTime - dynamicTime) / staticTime) * 100.0;
    else
        r.improvementPercent = 0.0;
    return r;
}

void printAnalytics(const TripResult& result) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n----- RESPONSE TIME ANALYTICS -----\n";
    std::cout << "Same route, regular vehicle (no priority) : " << result.staticTime << " min\n";
    std::cout << "Ambulance (green corridor + dynamic reroute): " << result.dynamicTime << " min\n";
    std::cout << "Improvement                                 : " << result.improvementPercent << " %\n";
    std::cout << "------------------------------------\n";
}
