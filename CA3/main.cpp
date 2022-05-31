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

#define TOPOLOGY "topology"
#define SHOW "show"

class Route
{
    public:
        Route(int node1name, int node2name , int dist)
        {
            node1 = node1name;
            node2 = node2name;
            distance = dist;

        }
        int get_node1()
        {
            return node1;
        }
        int get_node2()
        {
            return node2;
        }
        int get_distance()
        {
            return distance;
        }
    private:
        int node1;
        int node2;
        int distance;

};

int create_routing_table(string line, vector<Route*> *RoutingTable)
{
    int maxNodeNum = 0;
    const char delim = ' ';
    
    std::vector<std::string> out;
    std::stringstream ss(line);
 
    std::string s;
  
    while (std::getline(ss, s, delim)) {
      string n1 = s.substr(0,s.find('-'));
      s = s.erase(0,n1.length()+1);
      string n2 = s.substr(0,s.find('-'));
      s = s.erase(0,n2.length()+1);
      string dist = s;
    
      maxNodeNum = max({maxNodeNum, stoi(n1), stoi(n2)});
      RoutingTable->push_back(new Route(stoi(n1), stoi(n2), stoi(dist)));
   
}
 return maxNodeNum;
}

void print_topology(vector<Route*> RoutingTable , int countOfNodes)
{
   
    vector<vector<int>> net( countOfNodes , vector<int> (countOfNodes,0)); 
   
  
    for(int i=0 ;i < RoutingTable.size(); i++)
    {
        int node1 = RoutingTable[i]->get_node1();
        int node2 = RoutingTable[i]->get_node2();
        net[node1-1][node2-1] = RoutingTable[i]->get_distance();
       
    }
  
    cout<<"u|v |";
    for(int i = 1 ; i<= countOfNodes ; i++)
         cout<<setw(2)<<i;
    cout <<endl<< right << setfill('.') << setw(10)<<"."<<endl;
    for(int i=0 ;i< countOfNodes ; i++)
     {
        cout<<i+1<<"  | ";
       for(int j = 0;j<countOfNodes; j++)
         {
             cout<<net[i][j]<<" ";
         }
        cout<<endl;
     }
}


// 

int main()
{
   
    vector<Route*> RoutingTable;
    int countOfNodes;
    while(true)
    {
    std::string inputLine;
    getline(cin,inputLine);
    string command = inputLine.substr(0, inputLine.find(' '));
   
   
    if( command == TOPOLOGY)

        countOfNodes =  create_routing_table(inputLine.erase(0,command.length()+1), &RoutingTable);
       
    if(command == SHOW)
        print_topology(RoutingTable,countOfNodes);
    }
}