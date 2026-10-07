# AmbuNav — Smart Traffic & Ambulance Route Optimizer

## 1. What this project does

AmbuNav simulates a city road network and solves two problems together:

1. Find the **shortest route** between any two junctions (normal traffic routing).
2. Give an **ambulance priority** over that same network — using a "green corridor"
   (temporarily cheaper travel time along its path) and **dynamically recalculating**
   the route the instant an accident/road closure appears on the way.

It's a console (terminal) application — no internet, no real maps, all data is a
simulated "virtual city." This matches the Phase-I proposal exactly.

## 2. How to build and run

```bash
make        # compiles everything into ./ambunav
./ambunav   # run the interactive menu
```

Requires a C++17 compiler (g++ 9+ recommended). No external libraries needed —
everything is standard C++ (STL) plus our own hand-built data structures.

## 3. Folder structure

```
AmbuNav/
├── include/            # header files (.h)
│   ├── Graph.h          -> virtual city map + Dijkstra + BFS
│   ├── HashTable.h       -> custom hash table (junction name -> index)
│   ├── LinkedList.h      -> custom singly linked list (event/route log)
│   ├── Vehicle.h         -> abstract base class
│   ├── Ambulance.h       -> Vehicle subclass (high priority)
│   ├── RegularVehicle.h  -> Vehicle subclass (low priority)
│   ├── Hospital.h        -> plain data struct
│   ├── TrafficSimulator.h-> accident / congestion event generator
│   └── Analytics.h       -> static vs. dynamic route comparison
├── src/                 # implementation files (.cpp) — one per header + main.cpp
├── data/                # sample city layout reference
├── docs/                # concept-to-code mapping (see section 6 below)
├── Makefile
└── README.md
```

## 4. Menu walkthrough

| # | Option | What it shows |
|---|--------|----------------|
| 1 | Display Virtual City Map | Prints the graph (10 junctions, weighted roads) |
| 2 | Register a new vehicle | Create an `Ambulance` or `RegularVehicle` (OOP in action) |
| 3 | List registered vehicles | `getPriority()`/`getType()` resolve polymorphically per object |
| 4 | Simulate random congestion | Randomly raises edge weights (models real traffic) |
| 5 | Report accident / road closure | Closes an edge — Dijkstra will avoid it |
| 6 | Clear accident | Reopens the edge |
| 7 | Find shortest route | Plain Dijkstra between any two junctions |
| **8** | **Dispatch ambulance** | **The full demo**: nearest-hospital search, green-corridor activation, a simulated mid-route accident, automatic dynamic rerouting, and a before/after analytics report |
| 9 | Show route/event log | Prints the full linked-list event log of the session |

**Recommended live demo for Phase-II evaluation:** run option `8` with
`AMB_101`. It exercises almost every module in one go and prints a clear
analytics summary at the end (regular-vehicle time vs. ambulance time on the
same final route, isolating exactly how much the green corridor + priority
saved).

## 5. Sample city used (`data/city_data.txt` for reference)

10 junctions `J1`–`J10`, 13 bidirectional roads with travel-time weights
(minutes), 2 hospitals: **CityCare Hospital** (at J8) and **Metro General
Hospital** (at J10).

## 6. Concept-to-code mapping (for your Phase-II slide / mentor Q&A)

| Course concept | Where it is in code |
|---|---|
| **Graph (adjacency list)** | `Graph.h/.cpp` — `std::vector<std::vector<Edge>> adjList` |
| **Dijkstra's Algorithm** | `Graph::dijkstra()` — min-priority-queue based shortest path |
| **BFS** | `Graph::bfsReachable()` — unweighted reachability check |
| **Queue / Priority Queue** | `std::priority_queue` inside `dijkstra()`; `std::queue` inside `bfsReachable()` |
| **Linked List** | `LinkedList.h/.cpp` — custom singly linked `RouteLog` for the event log |
| **Hashing** | `HashTable.h/.cpp` — custom separate-chaining hash table mapping junction/vehicle names to indices in O(1) average time |
| **Arrays** | `nodeNames`, `adjList`, hash table buckets — all array/vector backed |
| **Classes & Encapsulation** | `Vehicle` (private/protected members, public getters/setters) |
| **Inheritance** | `Ambulance : public Vehicle`, `RegularVehicle : public Vehicle` |
| **Polymorphism** | `Vehicle::getPriority()` / `getType()` are `virtual`, overridden differently per subclass, called through a `vector<unique_ptr<Vehicle>>` |
| **STL** | `vector`, `queue`, `priority_queue`, `list`, `unique_ptr`, `string` used throughout |

## 7. Modules mapped to the proposal's "Key Features" slide

- **Virtual City Map** → `buildSampleCity()` in `main.cpp` + `Graph` class
- **Dynamic Traffic Simulation** → `TrafficSimulator::simulateRandomCongestion()`
- **Shortest and Dynamic Routes** → `Graph::dijkstra()`, re-invoked on demand
- **Ambulance Priority and Green Corridor** → `Ambulance::getPriority()` +
  `Graph::activateGreenCorridor()` / `deactivateGreenCorridor()`
- **Accident and Road Closure Management** → `Graph::closeRoad()/openRoad()`,
  `TrafficSimulator::reportAccident()/clearAccident()`

## 8. What's NOT yet built (not finished)

- No GUI/visualization yet — console only.
- City data is hardcoded in `buildSampleCity()`; not yet read from an external file at runtime (the `data/city_data.txt` is a reference layout, not yet parsed).
- No persistent storage (database) of trips/analytics across runs — the event log is in-memory and resets on exit.
- Congestion simulation is random, not based on real traffic patterns.
