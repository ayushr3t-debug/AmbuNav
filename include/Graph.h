#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <utility>
#include "HashTable.h"

// One road connecting two junctions
struct Edge {
    int to;
    double baseWeight;     // normal travel time in minutes
    double currentWeight;  // weight after congestion / green-corridor adjustment
    bool closed;           // true if road is closed (accident / blocked)
};

// Result of a shortest path query
struct PathResult {
    std::vector<int> path; // sequence of node indices
    double totalTime;      // total travel time in minutes
    bool found;
};

// The Virtual City Map: a weighted graph of junctions (nodes) and
// roads (edges). Implemented with an adjacency list.
class Graph {
private:
    int numNodes;
    std::vector<std::string> nodeNames;   // index -> junction name
    HashTable nameToIndex;                // junction name -> index (custom hashing)
    std::vector<std::vector<Edge>> adjList;

public:
    Graph();

    int addNode(const std::string& name);
    void addEdge(const std::string& from, const std::string& to,
                 double weight, bool bidirectional = true);

    int getIndex(const std::string& name) const;
    std::string getName(int index) const;
    int size() const;

    // ---- Traffic / emergency events ----
    bool closeRoad(const std::string& from, const std::string& to);
    bool openRoad(const std::string& from, const std::string& to);
    bool updateCongestion(const std::string& from, const std::string& to, double factor);
    double getBaseWeight(const std::string& from, const std::string& to) const;

    // Green corridor: temporarily reduces travel time along a given path
    // (simulating signal preemption for an approaching ambulance)
    void activateGreenCorridor(const std::vector<int>& path, double factor = 0.5);
    void deactivateGreenCorridor(const std::vector<int>& path);

    // ---- Algorithms ----
    std::vector<bool> bfsReachable(int start) const;          // BFS reachability
    PathResult dijkstra(int start, int end) const;             // weighted shortest path

    void printGraph() const;
};

#endif
