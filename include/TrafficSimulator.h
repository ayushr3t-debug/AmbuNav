#ifndef TRAFFICSIMULATOR_H
#define TRAFFICSIMULATOR_H

#include "Graph.h"
#include "LinkedList.h"
#include <string>

// Static-method utility class that mutates a Graph to simulate
// real-world traffic conditions: random congestion, accidents, closures.
class TrafficSimulator {
public:
    static void simulateRandomCongestion(Graph& g, RouteLog& log, int numEdgesToAffect = 3);
    static void reportAccident(Graph& g, RouteLog& log, const std::string& from, const std::string& to);
    static void clearAccident(Graph& g, RouteLog& log, const std::string& from, const std::string& to);
};

#endif
