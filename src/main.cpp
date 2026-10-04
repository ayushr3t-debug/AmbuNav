#include <iostream>
#include <vector>
#include <memory>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include "../include/Graph.h"
#include "../include/Vehicle.h"
#include "../include/Ambulance.h"
#include "../include/RegularVehicle.h"
#include "../include/Hospital.h"
#include "../include/TrafficSimulator.h"
#include "../include/Analytics.h"
#include "../include/LinkedList.h"

using namespace std;

// Builds a sample virtual city: 10 junctions, roads with travel time (minutes), 2 hospitals.
void buildSampleCity(Graph& city, vector<Hospital>& hospitals) {
    city.addEdge("J1", "J2", 4);
    city.addEdge("J1", "J3", 7);
    city.addEdge("J2", "J4", 3);
    city.addEdge("J3", "J4", 2);
    city.addEdge("J3", "J5", 6);
    city.addEdge("J4", "J6", 5);
    city.addEdge("J5", "J6", 3);
    city.addEdge("J5", "J7", 4);
    city.addEdge("J6", "J8", 6);
    city.addEdge("J7", "J8", 2);
    city.addEdge("J7", "J9", 5);
    city.addEdge("J8", "J10", 3);
    city.addEdge("J9", "J10", 4);

    hospitals.push_back({"CityCare Hospital", "J8", 50, 12});
    hospitals.push_back({"Metro General Hospital", "J10", 80, 30});
}

void printVehicles(const vector<unique_ptr<Vehicle>>& vehicles) {
    cout << "\n----- REGISTERED VEHICLES -----\n";
    for (const auto& v : vehicles) {
        // v->getType()/getPriority() resolve polymorphically at runtime
        cout << v->getId() << " | Type: " << v->getType()
             << " | Priority: " << v->getPriority()
             << " | Location: " << v->getLocation() << "\n";
    }
    cout << "--------------------------------\n";
}

string pathToString(const Graph& g, const vector<int>& path) {
    ostringstream oss;
    for (size_t i = 0; i < path.size(); i++) {
        oss << g.getName(path[i]);
        if (i + 1 < path.size()) oss << " -> ";
    }
    return oss.str();
}

// Finds the nearest hospital (by Dijkstra travel time) from a given junction.
pair<int, PathResult> findNearestHospital(const Graph& city, const vector<Hospital>& hospitals, int fromIdx) {
    double best = 1e18;
    int bestHospital = -1;
    PathResult bestPath;
    for (size_t h = 0; h < hospitals.size(); h++) {
        int hIdx = city.getIndex(hospitals[h].location);
        PathResult pr = city.dijkstra(fromIdx, hIdx);
        if (pr.found && pr.totalTime < best) {
            best = pr.totalTime;
            bestHospital = (int)h;
            bestPath = pr;
        }
    }
    return {bestHospital, bestPath};
}

int main() {
    srand((unsigned)time(nullptr));

    Graph city;
    vector<Hospital> hospitals;
    buildSampleCity(city, hospitals);

    vector<unique_ptr<Vehicle>> vehicles;
    vehicles.push_back(make_unique<Ambulance>("AMB_101", "J1"));
    vehicles.push_back(make_unique<RegularVehicle>("CAR_205", "J3"));

    RouteLog log;
    log.addEntry("System initialised with sample city (10 junctions, 2 hospitals).");

    int choice = -1;
    while (choice != 0) {
        cout << "\n================= AmbuNav Menu =================\n";
        cout << "1. Display Virtual City Map\n";
        cout << "2. Register a new vehicle\n";
        cout << "3. List registered vehicles\n";
        cout << "4. Simulate random traffic congestion\n";
        cout << "5. Report an accident / road closure\n";
        cout << "6. Clear an accident / reopen a road\n";
        cout << "7. Find shortest route between two junctions\n";
        cout << "8. Dispatch AMBULANCE to nearest hospital (priority + green corridor)\n";
        cout << "9. Show route/event log\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            city.printGraph();

        } else if (choice == 2) {
            string id, loc, type;
            cout << "Vehicle ID: "; cin >> id;
            cout << "Starting junction (e.g. J1): "; cin >> loc;
            cout << "Type (ambulance/regular): "; cin >> type;
            if (type == "ambulance")
                vehicles.push_back(make_unique<Ambulance>(id, loc));
            else
                vehicles.push_back(make_unique<RegularVehicle>(id, loc));
            log.addEntry("Vehicle registered: " + id + " (" + type + ") at " + loc);
            cout << "Vehicle registered.\n";

        } else if (choice == 3) {
            printVehicles(vehicles);

        } else if (choice == 4) {
            TrafficSimulator::simulateRandomCongestion(city, log, 3);

        } else if (choice == 5) {
            string a, b;
            cout << "From junction: "; cin >> a;
            cout << "To junction: "; cin >> b;
            TrafficSimulator::reportAccident(city, log, a, b);

        } else if (choice == 6) {
            string a, b;
            cout << "From junction: "; cin >> a;
            cout << "To junction: "; cin >> b;
            TrafficSimulator::clearAccident(city, log, a, b);

        } else if (choice == 7) {
            string a, b;
            cout << "From junction: "; cin >> a;
            cout << "To junction: "; cin >> b;
            int ai = city.getIndex(a), bi = city.getIndex(b);
            if (ai == -1 || bi == -1) {
                cout << "Invalid junction name.\n";
                continue;
            }
            PathResult pr = city.dijkstra(ai, bi);
            if (pr.found) {
                cout << "Shortest path: " << pathToString(city, pr.path) << "\n";
                cout << "Total travel time: " << pr.totalTime << " minutes\n";
            } else {
                cout << "No route available (road closures may be blocking all paths).\n";
            }

        } else if (choice == 8) {
            // --- Full ambulance dispatch demo: priority routing, green corridor,
            //     a simulated mid-route accident, dynamic rerouting, and analytics ---
            string ambId;
            cout << "Ambulance ID to dispatch (e.g. AMB_101): "; cin >> ambId;

            Ambulance* amb = nullptr;
            for (auto& v : vehicles) {
                if (v->getId() == ambId && v->getType() == "Ambulance") {
                    amb = dynamic_cast<Ambulance*>(v.get());
                    break;
                }
            }
            if (!amb) { cout << "Ambulance not found.\n"; continue; }

            int startIdx = city.getIndex(amb->getLocation());
            auto [hIdx, staticPath] = findNearestHospital(city, hospitals, startIdx);
            if (hIdx == -1) { cout << "No reachable hospital found.\n"; continue; }

            double staticTime = staticPath.totalTime; // baseline, no priority
            cout << "\nNearest hospital: " << hospitals[hIdx].name
                 << " via " << pathToString(city, staticPath.path)
                 << " (" << staticTime << " min, static route)\n";

            amb->activateEmergency();
            log.addEntry(ambId + " emergency ACTIVATED. Dispatch started towards " + hospitals[hIdx].name);

            // Activate green corridor: reduce travel time along the planned path
            city.activateGreenCorridor(staticPath.path, 0.5);
            log.addEntry("Green corridor activated along route: " + pathToString(city, staticPath.path));

            // Recompute with corridor active (this is the "dynamic" favourable route)
            int hospitalIdx = city.getIndex(hospitals[hIdx].location);
            PathResult corridorPath = city.dijkstra(startIdx, hospitalIdx);

            // Simulate a mid-route accident on one edge of the path (if long enough)
            vector<int> finalPath = corridorPath.path; // what the ambulance actually drives
            double ambulanceTime = corridorPath.totalTime; // time with corridor + any reroute

            if (corridorPath.path.size() > 2) {
                int mid = corridorPath.path.size() / 2;
                int from = corridorPath.path[mid - 1];
                int to = corridorPath.path[mid];
                string fromName = city.getName(from), toName = city.getName(to);

                TrafficSimulator::reportAccident(city, log, fromName, toName);

                int currentPos = corridorPath.path[mid - 1];
                PathResult toHere = city.dijkstra(startIdx, currentPos);     // prefix already driven
                PathResult reroute = city.dijkstra(currentPos, hospitalIdx); // recalculated remainder

                if (reroute.found) {
                    ambulanceTime = (toHere.found ? toHere.totalTime : 0) + reroute.totalTime;

                    // stitch prefix + reroute into the single final path actually driven
                    finalPath = toHere.found ? toHere.path : vector<int>{currentPos};
                    for (size_t i = 1; i < reroute.path.size(); i++) finalPath.push_back(reroute.path[i]);

                    cout << "\n[EVENT] Accident ahead on planned route!\n";
                    cout << "Recalculated route from " << fromName << ": "
                         << pathToString(city, reroute.path) << "\n";
                    log.addEntry("Dynamic reroute computed from " + fromName +
                                  " due to accident. New ETA: " + to_string(ambulanceTime) + " min");
                } else {
                    cout << "No alternate route found after accident!\n";
                }

                // Reopen the road afterwards (accident cleared once ambulance diverted)
                TrafficSimulator::clearAccident(city, log, fromName, toName);
            } else {
                cout << "\nRoute too short for a mid-route reroute demo; proceeding directly.\n";
            }

            // Fair comparison: same final route, WITH vs WITHOUT ambulance green-corridor
            // priority (base travel-time weights) -> isolates the benefit of the system.
            double noPriorityTime = 0;
            bool allEdgesKnown = true;
            for (size_t i = 0; i + 1 < finalPath.size(); i++) {
                double bw = city.getBaseWeight(city.getName(finalPath[i]), city.getName(finalPath[i + 1]));
                if (bw < 0) { allEdgesKnown = false; break; }
                noPriorityTime += bw;
            }
            if (!allEdgesKnown) noPriorityTime = staticTime; // fallback

            city.deactivateGreenCorridor(staticPath.path);
            amb->deactivateEmergency();
            amb->setLocation(hospitals[hIdx].location);
            log.addEntry(ambId + " reached " + hospitals[hIdx].name + ". Emergency deactivated.");

            cout << "\nActual route driven: " << pathToString(city, finalPath) << "\n";
            TripResult result = compareRoutes(noPriorityTime, ambulanceTime);
            printAnalytics(result);

        } else if (choice == 9) {
            log.printLog();

        } else if (choice != 0) {
            cout << "Invalid choice, try again.\n";
        }
    }

    cout << "\nExiting AmbuNav. Stay safe on the roads!\n";
    return 0;
}
