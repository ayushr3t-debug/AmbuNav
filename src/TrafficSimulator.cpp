#include "../include/TrafficSimulator.h"
#include <cstdlib>
#include <iostream>

void TrafficSimulator::simulateRandomCongestion(
    Graph& g,
    RouteLog& log,
    int numEdgesToAffect
) {
    int n = g.size();

    // No roads can be updated if fewer than two junctions exist.
    if (n < 2 || numEdgesToAffect <= 0) {
        std::cout << "No roads available for congestion simulation.\n";
        return;
    }

    int updated = 0;
    int attempts = 0;
    int maxAttempts = numEdgesToAffect * 10;

    while (updated < numEdgesToAffect &&
           attempts < maxAttempts) {
        attempts++;

        int a = rand() % n;
        int b = rand() % n;

        // A road cannot connect a junction to itself.
        if (a == b) {
            continue;
        }

        double factor =
            1.2 + (rand() % 150) / 100.0;

        std::string from = g.getName(a);
        std::string to = g.getName(b);

        // Update only if the road exists.
        if (g.updateCongestion(from, to, factor)) {
            std::string entry =
                "Congestion update: " + from + " <-> " + to +
                ", travel time x" + std::to_string(factor).substr(0, 4);

            log.addEntry(entry);
            std::cout << entry << '\n';

            updated++;
        }
    }

    if (updated == 0) {
        std::cout << "No roads were updated. Check the graph connections.\n";
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
