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
using namespace std;

Routing::Routing(Network *net){network = net;}



vector<int> Routing::DVRP(vector<int> *routingTable, int src) {
  
   
    int numOfNodes = routingTable->size();
    int numOfRoutes = network->get_numberOfRoutes();
    vector<int> prevHops(numOfNodes,-1);

     for (int i = 0; i < numOfNodes; i++){
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
                && (*routingTable)[u] + distance < (*routingTable)[v]){
                (*routingTable)[v] = (*routingTable)[u] + distance;
                prevHops[v] = u;
                }
            
            int tmp =u;
            u = v;
            v = tmp;
            if ((*routingTable)[u] != INT_MAX
                && (*routingTable)[u] + distance < (*routingTable)[v]){
                (*routingTable)[v] = (*routingTable)[u] + distance;
                prevHops[v] = u;
                }
        }
    }
   
    return prevHops;
    
}

vector<int> get_path(vector<int> parents, int src,int node)
{
    vector<int> path;
    while(true)
    {
        path.push_back(node+1);
        int par = parents[node];
        if(par == -1){
          
            return path;
        }
       
        node = par;
    }
   return path;
}

void print_routingTable(vector<int> routingTable, vector<int> prevHops, int src)
{
   
    cout<<"\nRouting table at node "<<src+1<<endl;
    cout<<"Dest\t\tNext Hop\tDist\t\tShortest Path\n";
    for (size_t i = 0; i < routingTable.size(); ++i){
        vector<int> path = get_path(prevHops,src,i);
       
        int nextHop = (path.size() < 2)? path[0] : path[path.size()-2];
        int dist  = (routingTable[i] == INT_MAX) ? -1 : routingTable[i];
        nextHop  = (routingTable[i] == INT_MAX) ? -1 : nextHop;
        cout<<i+1<<"\t\t"<<nextHop<<"\t\t"<<dist<<"\t\t";;
       
        int pathLength = (routingTable[i] == INT_MAX) ? -1 : path.size()-1 ;
        cout<<"[ ";
        for(int i = pathLength; i > -1; i--){
            if( i == 0 )
                cout<<path[i];
            else
                cout<<path[i]<<"->";
        }
        
        cout<<" ]"<<endl;
    }
}

void Routing::runDVRP(int src, bool allNodes)
{
    int numOfNodes = network->get_numberOfNodes();
    if(allNodes == false)
    {
        vector<int> routingTable(numOfNodes);
        vector<int> prevHops = DVRP(&routingTable, src);
        print_routingTable(routingTable, prevHops, src);
    }
    else{
        for(int i=0 ; i<numOfNodes;i++)
            runDVRP(i,false);
            
        return;
    }
}

