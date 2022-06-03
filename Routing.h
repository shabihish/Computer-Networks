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
        void runLSPR();
        
    
        
    private:
        Network *network;
        std::vector<int> DVRP(std::vector<int> *routingTable, int src);

};

#endif