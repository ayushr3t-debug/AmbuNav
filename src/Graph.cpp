#include "../include/Graph.h"
#include <iostream>
#include <queue>
#include <limits>
#include <algorithm>

Graph::Graph() : numNodes(0), nameToIndex(101) {}

int Graph::addNode(const std::string& name) {
    int existing;
    if (nameToIndex.get(name, existing)) {
        return existing; // already exists
    }
    int idx = numNodes++;
    nodeNames.push_back(name);
    adjList.push_back(std::vector<Edge>());
    nameToIndex.insert(name, idx);
    return idx;
}

void Graph::addEdge(const std::string& from, const std::string& to,
                     double weight, bool bidirectional) {
    int a = addNode(from);
    int b = addNode(to);
    adjList[a].push_back(Edge{b, weight, weight, false});
    if (bidirectional) {
        adjList[b].push_back(Edge{a, weight, weight, false});
    }
}

int Graph::getIndex(const std::string& name) const {
    int idx;
    if (nameToIndex.get(name, idx)) return idx;
    return -1;
}

std::string Graph::getName(int index) const {
    if (index < 0 || index >= numNodes) return "INVALID";
    return nodeNames[index];
}

int Graph::size() const { return numNodes; }

bool Graph::closeRoad(const std::string& from, const std::string& to) {
    int a = getIndex(from), b = getIndex(to);
    if (a == -1 || b == -1) return false;
    bool found = false;
    for (auto& e : adjList[a]) if (e.to == b) { e.closed = true; found = true; }
    for (auto& e : adjList[b]) if (e.to == a) { e.closed = true; }
    return found;
}

bool Graph::openRoad(const std::string& from, const std::string& to) {
    int a = getIndex(from), b = getIndex(to);
    if (a == -1 || b == -1) return false;
    bool found = false;
    for (auto& e : adjList[a]) if (e.to == b) { e.closed = false; found = true; }
    for (auto& e : adjList[b]) if (e.to == a) { e.closed = false; }
    return found;
}

bool Graph::updateCongestion(const std::string& from, const std::string& to, double factor) {
    int a = getIndex(from), b = getIndex(to);
    if (a == -1 || b == -1) return false;
    bool found = false;
    for (auto& e : adjList[a]) if (e.to == b) { e.currentWeight = e.baseWeight * factor; found = true; }
    for (auto& e : adjList[b]) if (e.to == a) { e.currentWeight = e.baseWeight * factor; }
    return found;
}

double Graph::getBaseWeight(const std::string& from, const std::string& to) const {
    int a = getIndex(from), b = getIndex(to);
    if (a == -1 || b == -1) return -1.0;
    for (const auto& e : adjList[a]) if (e.to == b) return e.baseWeight;
    return -1.0;
}

void Graph::activateGreenCorridor(const std::vector<int>& path, double factor) {
    for (size_t i = 0; i + 1 < path.size(); i++) {
        int a = path[i], b = path[i + 1];
        for (auto& e : adjList[a]) if (e.to == b) e.currentWeight = e.baseWeight * factor;
        for (auto& e : adjList[b]) if (e.to == a) e.currentWeight = e.baseWeight * factor;
    }
}

void Graph::deactivateGreenCorridor(const std::vector<int>& path) {
    for (size_t i = 0; i + 1 < path.size(); i++) {
        int a = path[i], b = path[i + 1];
        for (auto& e : adjList[a]) if (e.to == b) e.currentWeight = e.baseWeight;
        for (auto& e : adjList[b]) if (e.to == a) e.currentWeight = e.baseWeight;
    }
}

// BFS: used for simple reachability checks (unweighted hop-count style queries)
std::vector<bool> Graph::bfsReachable(int start) const {
    std::vector<bool> visited(numNodes, false);
    if (start < 0 || start >= numNodes) return visited;
    std::queue<int> q;
    q.push(start);
    visited[start] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (const auto& e : adjList[u]) {
            if (!e.closed && !visited[e.to]) {
                visited[e.to] = true;
                q.push(e.to);
            }
        }
    }
    return visited;
}

// Dijkstra: weighted shortest path using a min-priority queue
PathResult Graph::dijkstra(int start, int end) const {
    PathResult result;
    result.found = false;
    result.totalTime = std::numeric_limits<double>::infinity();

    if (start < 0 || start >= numNodes || end < 0 || end >= numNodes) return result;

    std::vector<double> dist(numNodes, std::numeric_limits<double>::infinity());
    std::vector<int> prev(numNodes, -1);
    std::vector<bool> visited(numNodes, false);

    // min-heap of (distance, node)
    std::priority_queue<std::pair<double,int>,
                         std::vector<std::pair<double,int>>,
                         std::greater<std::pair<double,int>>> pq;

    dist[start] = 0.0;
    pq.push({0.0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (visited[u]) continue;
        visited[u] = true;
        if (u == end) break;

        for (const auto& e : adjList[u]) {
            if (e.closed) continue; // skip closed / accident roads
            double nd = d + e.currentWeight;
            if (nd < dist[e.to]) {
                dist[e.to] = nd;
                prev[e.to] = u;
                pq.push({nd, e.to});
            }
        }
    }

    if (dist[end] == std::numeric_limits<double>::infinity()) {
        return result; // not found
    }

    std::vector<int> path;
    for (int at = end; at != -1; at = prev[at]) path.push_back(at);
    std::reverse(path.begin(), path.end());

    result.path = path;
    result.totalTime = dist[end];
    result.found = true;
    return result;
}

void Graph::printGraph() const {
    std::cout << "\n----- VIRTUAL CITY MAP (" << numNodes << " junctions) -----\n";
    for (int i = 0; i < numNodes; i++) {
        std::cout << nodeNames[i] << " -> ";
        for (const auto& e : adjList[i]) {
            std::cout << nodeNames[e.to] << "(" << e.currentWeight << "m"
                       << (e.closed ? ",CLOSED" : "") << ") ";
        }
        std::cout << "\n";
    }
    std::cout << "---------------------------------------------\n";
}
