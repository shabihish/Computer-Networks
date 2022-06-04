#ifndef Network_H_
#define Network_H_

#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <iostream>

#include <fstream>

#include <sstream>
#include <math.h>
#include <stdio.h>
#include <sstream>

using namespace std;

class Route {
public:
    Route(int node1name, int node2name, int dist) {
        node1 = node1name;
        node2 = node2name;
        distance = dist;

    }

    int get_node1() const {
        return node1;
    }

    int get_node2() const {
        return node2;
    }

    int get_distance() const {
        return distance;
    }

    void set_distance(int dist) {
        distance = dist;
    }

private:
    int node1;
    int node2;
    int distance;

};

class Network{
    public:
        Network();
     
       
        void modifyRoute(int node1, int node2, int newDistance);
        void removeRoute(int node1, int node2);
      
        void setTopology(std::vector<std::string> topology);
        void printTopology();
        int get_numberOfNodes();
        int get_numberOfRoutes();
        Route get_route(int i);

    private:
        std::vector<Route*> routes;
        int numberOfNodes;
       
        
};

#endif