#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <iostream>

#include <fstream>

#include <sstream>
#include <math.h>
#include <stdio.h>
#include <sstream>
#include "Network.h"
#include "Routing.h"

//using namespace std;


Routing::Routing(Network *net) { network = net; }


vector<int> Routing::DVRP(vector<int> *routingTable, int src) {
    int numOfNodes = routingTable->size();
    int numOfRoutes = network->get_numberOfRoutes();
    vector<int> prevHops(numOfNodes, -1);

    for (int i = 0; i < numOfNodes; i++) {
        (*routingTable)[i] = INT_MAX;
        prevHops[i] = -1;
    }
    (*routingTable)[src] = 0;

    for (int i = 1; i <= numOfRoutes - 1; i++) {
        for (int j = 0; j < numOfRoutes; j++) {

            Route route = network->get_route(j);
            int u = route.get_node1();
            int v = route.get_node2();
            int distance = route.get_distance();

            if ((*routingTable)[u] != INT_MAX
                && (*routingTable)[u] + distance < (*routingTable)[v]) {
                (*routingTable)[v] = (*routingTable)[u] + distance;
                prevHops[v] = u;
            }

            int tmp = u;
            u = v;
            v = tmp;
            if ((*routingTable)[u] != INT_MAX
                && (*routingTable)[u] + distance < (*routingTable)[v]) {
                (*routingTable)[v] = (*routingTable)[u] + distance;
                prevHops[v] = u;
            }
        }
    }
    return prevHops;
}

vector<int> get_path(vector<int> parents, int src, int node) {
    vector<int> path;
    while (true) {
        path.push_back(node + 1);
        int par = parents[node];
        if (par == -1) {
            return path;
        }
        node = par;
    }
}

void print_routingTable(vector<int> routingTable, vector<int> prevHops, int src) {

    cout << "\nRouting table at node " << src + 1 << endl;
    cout << "Dest\t\tNext Hop\tDist\t\tShortest Path\n";
    for (size_t i = 0; i < routingTable.size(); ++i) {
        vector<int> path = get_path(prevHops, src, i);

        int nextHop = (path.size() < 2) ? path[0] : path[path.size() - 2];
        int dist = (routingTable[i] == INT_MAX) ? -1 : routingTable[i];
        nextHop = (routingTable[i] == INT_MAX) ? -1 : nextHop;
        cout << i + 1 << "\t\t" << nextHop << "\t\t" << dist << "\t\t";

        int pathLength = (routingTable[i] == INT_MAX) ? -1 : path.size() - 1;
        cout << "[ ";
        for (int i = pathLength; i > -1; i--) {
            if (i == 0)
                cout << path[i];
            else
                cout << path[i] << "->";
        }

        cout << " ]" << endl;
    }
}

void Routing::runDVRP(int src, bool allNodes) {
    int numOfNodes = network->get_numberOfNodes();
    if (allNodes) {
        for (int i = 0; i < numOfNodes; i++)
            runDVRP(i, false);

        return;
    } else {
        vector<int> routingTable(numOfNodes);
        vector<int> prevHops = DVRP(&routingTable, src);
        print_routingTable(routingTable, prevHops, src);
    }
}

int minDistance(vector<int> dist, bool sptSet[]) {
    // Initialize min value
    int min = INT_MAX, min_index = INT_MIN;
    for (int i = 0; i < dist.size(); i++) {
        if (sptSet[i] || dist[i] > min)
            continue;
        min = dist[i], min_index = i;
    }
    return min_index;
}

void printPath(int parent[], int j) {
    // Base Case : If j is source
    if (parent[j] == -1)
        return;
    printPath(parent, parent[j]);
    cout << j << " ";
}

int printSolution(vector<int> dist, int n, int parent[]) {
    int src = 0;
    cout << "Vertex\t Distance\tPath";
    for (int i = 1; i < dist.size(); i++) {
        printf("\n%d -> %d \t\t %d\t\t%d ", src, i, dist[i],
               src);
        printPath(parent, i);
    }
}


vector<int> Routing::LSPR(vector<int> routingTable, int src) {
    int n = routingTable.size();

    bool sptSet[n] = {false};
    int parent[n] = {-1};

    for (int i = 0; i < n; i++)
        if (i != src)
            routingTable[i] = INT_MAX;
        else
            routingTable[i] = 0;
    vector<vector<int>> networkGraph(n);
    for (int i = 0; i < n; i++) {
        networkGraph.emplace_back(vector<int>(n));
        for (int j = 0; j < n; j++) {
            networkGraph[i].emplace_back(0);
        }
    }

    for (int i = 0; i < network->get_numberOfRoutes(); i++) {
        Route route = network->get_route(i);

        int dist = route.get_distance();
        if (dist <= 0)
            continue;
        int node1 = route.get_node1();
        int node2 = route.get_node2();

        networkGraph[node1][node2] = dist;
        networkGraph[node2][node1] = dist;
    }

    for (int count = 0; count < n - 1; count++) {
        int u = minDistance(routingTable, sptSet);

        sptSet[u] = true;

        for (int v = 0; v < V; v++)
            if (!sptSet[v] && networkGraph[u][v]
                && routingTable[u] + networkGraph[u][v] < routingTable[v]) {
                parent[v] = u;
                routingTable[v] = routingTable[u] + networkGraph[u][v];
            }
    }
    printSolution(routingTable, n, parent);
//    return nullptr;
}

void Routing::runLSPR(int src, bool allNodes) {
    int numOfNodes = network->get_numberOfNodes();
    if (allNodes) {
        for (int i = 0; i < numOfNodes; i++)
            runLSPR(i, false);
        return;
    } else {
        vector<int> routingTable(numOfNodes);
        vector<int> prevHops = DVRP(&routingTable, src);
//        print_routingTable(routingTable, prevHops, src);
    }
}

