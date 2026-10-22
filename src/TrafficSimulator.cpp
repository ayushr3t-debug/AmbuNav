#include "../include/TrafficSimulator.h"
#include <cstdlib>
#include <iostream>



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
