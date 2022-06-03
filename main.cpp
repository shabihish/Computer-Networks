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

#define TOPOLOGY "topology"
#define SHOW "show"
#define DVRP "dvrp"
#define LSPR "lspr"
#define MODIFY "modify"

#define REMOVE "remove"
#define delim1 ' '
#define delim2 '-'
using namespace std;
////
// topology 13-9-7 13-6-4 9-12-5 6-4-19 6-11-25 6-8-17 6-2-2 4-10-11 10-5-3 10-12-12 5-1-6 11-8-16 8-3-4 12-3-8 12-7-8 2-7-13 3-7-1 1-7-7
vector<string> string_split(const string& strr) {
	vector<string> result;
    stringstream ss(strr);
    string s;

    while (getline(ss, s, delim1)) {
        string n1 = s.substr(0, s.find(delim2));
        s = s.erase(0, n1.length() + 1);
        string n2 = s.substr(0, s.find(delim2));
        s = s.erase(0, n2.length() + 1);
        string dist = s;
        result.push_back(n1);
        result.push_back(n2);
        result.push_back(dist);

    }
	return result;
}

int main()
{
   
    Network network;
    Routing routing = Routing(&network);
      
     while (true) {
        string inputLine;
        getline(cin, inputLine);
        string command = inputLine.substr(0, inputLine.find(delim1));
        vector<string> inputData = string_split(inputLine.erase(0, command.length() + 1));
     
        if (command == TOPOLOGY)
            network.setTopology(inputData);

        if (command == SHOW)
            network.printTopology();

        if(command == LSPR)
        {
            if(inputData.size() == 0)
                routing.runLSPR( -1, true);
            else
                routing.runLSPR(stoi(inputData[0])-1,false);
        }
        if(command == DVRP)
        {
            if(inputData.size() == 0)
                routing.runDVRP( -1, true);
            else
                routing.runDVRP(stoi(inputData[0])-1,false);
        }
        if(command == MODIFY)
        {
            network.modifyRoute(stoi(inputData[0])-1,stoi(inputData[1])-1, stoi(inputData[2]));
        }

        if(command == REMOVE)
        {
            network.removeRoute(stoi(inputData[0])-1,stoi(inputData[1])-1);
        }
        cout<<endl;
    }
}