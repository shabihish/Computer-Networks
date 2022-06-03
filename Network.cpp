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

Network::Network(){numberOfNodes = 0;}

void Network::setTopology(vector<std::string> topology)
{
    for(size_t i=0;i<topology.size();i=i+3)
    {
        int n1 = stoi(topology[i])-1;
        int n2 = stoi(topology[i+1])-1;
        int dist = stoi(topology[i+2]);
        routes.push_back(new Route(n1, n2, dist));
        numberOfNodes = max({numberOfNodes, n1+1, n2+1});
    }
}

void Network::printTopology() {

    vector<vector<int>> net(numberOfNodes, vector<int>(numberOfNodes, -1));

    for (auto & route : routes) {
        int node1 = route->get_node1();
        int node2 = route->get_node2();
        net[node1][node2] = route->get_distance();
        net[node2][node1] = route->get_distance();
     
    }

    cout << "u|v |";
    for (int i = 1; i <= numberOfNodes; i++)
        cout << "\t" << i;
    cout << endl << right << setfill('.') << setw(numberOfNodes * 9) << "." << endl;
    for (int i = 0; i < numberOfNodes; i++) {
        cout << i + 1 << "   | ";
        for (int j = 0; j < numberOfNodes; j++) {
            if(i==j){
                cout <<"\t"<< 0 ; 
                continue;
            }
            else   
                cout <<"\t"<< net[i][j];
        }
        cout << "\n\n";
    }
}

int Network::get_numberOfNodes(){return numberOfNodes;}

int Network::get_numberOfRoutes(){return routes.size();}

Route Network::get_route(int i)
{
    return *(routes)[i];
}

void Network::modifyRoute(int node1, int node2, int  newDistance)
{
  
    if(node1 == node2)
    {
        cout<<"Error: Source node and destination node must be different"<<endl;
        return;
    }
    for (size_t j = 0; j < routes.size(); j++) {
        if((routes[j]->get_node1() == node1 && routes[j]->get_node2()== node2)
            || (routes[j]->get_node1() == node2 && routes[j]->get_node2()== node1))
        {
            routes[j]->set_distance(newDistance);
            return;
        }       
    }
    numberOfNodes = max({numberOfNodes, node1, node2});
    routes.push_back(new Route(node1 ,node2 , newDistance));
}

void Network::removeRoute(int node1, int node2)
{
   
        for (size_t j = 0; j < routes.size(); j++) {
            if((routes[j]->get_node1() == node1 && routes[j]->get_node2()== node2)||
                (routes[j]->get_node1() == node2 && routes[j]->get_node2()== node1))
            {
               routes.erase(routes.begin()+j);
               return;
            }  
        }
}
