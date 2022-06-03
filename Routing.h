#ifndef Routing_H_
#define Routing_H_

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
using namespace std;

class Routing{
    public:
        Routing(Network *net);
        void runDVRP(int src, bool allNodes = true);
        void runLSPR(int src, bool allNodes = true);

    private:
        Network *network;
        vector<int> DVRP(vector<int> *routingTable, int src);
        vector<int> LSPR(vector<int> routingTable, int src);

};

#endif