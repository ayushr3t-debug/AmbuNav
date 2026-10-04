#include "../include/TrafficSimulator.h"
#include <cstdlib>
#include <iostream>

void TrafficSimulator::simulateRandomCongestion(Graph& g, RouteLog& log, int numEdgesToAffect) {
    int n = g.size();
    if (n < 2) return;
    for (int i = 0; i < numEdgesToAffect; i++) {
        int a = rand() % n;
        int b = rand() % n;
        if (a == b) continue;
        double factor = 1.2 + (rand() % 150) / 100.0; // 1.2x - 2.7x slower
        std::string from = g.getName(a), to = g.getName(b);
        if (g.updateCongestion(from, to, factor)) {
            std::string entry = "Congestion update: " + from + " <-> " + to +
                                 " travel time x" + std::to_string(factor).substr(0, 4);
            log.addEntry(entry);
            std::cout << entry << "\n";
        }
    }
}

void TrafficSimulator::reportAccident(Graph& g, RouteLog& log,
                                       const std::string& from, const std::string& to) {
    if (g.closeRoad(from, to)) {
        std::string entry = "ACCIDENT reported: road " + from + " <-> " + to + " CLOSED";
        log.addEntry(entry);
        std::cout << entry << "\n";
    } else {
        std::cout << "No such road found between " << from << " and " << to << "\n";
    }
}

void TrafficSimulator::clearAccident(Graph& g, RouteLog& log,
                                      const std::string& from, const std::string& to) {
    if (g.openRoad(from, to)) {
        std::string entry = "Road CLEARED: " + from + " <-> " + to + " reopened";
        log.addEntry(entry);
        std::cout << entry << "\n";
    }
}
