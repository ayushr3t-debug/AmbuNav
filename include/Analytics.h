#ifndef ANALYTICS_H
#define ANALYTICS_H

struct TripResult {
    double staticTime;         // time via normal/static shortest path (no priority)
    double dynamicTime;        // time via ambulance priority + green corridor + rerouting
    double improvementPercent;
};

TripResult compareRoutes(double staticTime, double dynamicTime);
void printAnalytics(const TripResult& result);

#endif
