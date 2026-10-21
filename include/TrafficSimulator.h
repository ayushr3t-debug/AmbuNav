#ifndef TRAFFICSIMULATOR_H
#define TRAFFICSIMULATOR_H

#include "TrafficReport.h"
#include "Graph.h"
#include "LinkedList.h"
#include <string>

// Static-method utility class that mutates a Graph to simulate
// real-world traffic conditions: random congestion, accidents, closures.
class TrafficSimulator {
private:
    TraficReprt report;
public:
    static void simulateRandomCongestion(Graph& g, RouteLog& log, int numEdgesToAffect = 3);
    static void reportAccident(Graph& g, RouteLog& log, const std::string& from, const std::string& to);
    static void clearAccident(Graph& g, RouteLog& log, const std::string& from, const std::string& to);

// Access traffic report
    TrafficReport& getReport() {
        return report;
    }

    const TrafficReport& getReport() const {
        return report;
    }

    // Display traffic statistics
    void displayTrafficReport() const {
        report.displayReport();
    }
};

#endif
