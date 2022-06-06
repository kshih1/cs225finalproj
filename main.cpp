#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "graph.h"
#include "Airlines.h"
using namespace std;

int main(){
  cout << "CS225 SP22 Final Project: OpenFlights Routes & Airports" << endl;
  cout << "Group Members: Anushka Pachuary (ap39), Rithvik Kopparapu (rithvik9), Kelly Shih (kshih7)" << endl;
  cout << endl; 

  graph* G = new graph();

  cout << "Enter the name of a valid airports dataset (ex: airports.txt)";
  cout << endl; 
  string airportData;
  cin >> airportData; 
  G->read_airports(airportData);
  cout << "Read airports!" << endl << endl; 

  cout << "Enter the name of a valid routes dataset (ex: routes.txt)"; 
  cout << endl;
  string routeData;
  cin >> routeData; 
  G->read_routes(routeData);
  cout << "Read routes!" << endl << endl; 
  G->graphCleaning();
  bool proj = true; 
  std::ofstream bc("betweenness.txt");
  while(proj == true){
    cout << endl; 
    cout << "Enter the number of which option you would like to select: " << endl;
    cout << "   1) BFS" << endl;
    cout << "   2) Dijsktra's" << endl; 
    cout << "   3) Betweenness Centrality" << endl; 
    cout << "   4) Finish and stop running the project (default)" << endl;
    string number; 
    cin >> number; 
    if(number == "1"){
      cout << "You have chosen BFS" << endl;
      cout << "Enter a valid OpenFlights airport id for the source airport you wish to find paths for " << endl;
      cout << "**NOTE: this airport id most have outgoing routes associated with it in the routes dataset to have a result." << endl;
      string input2;
      cin >> input2; 
      int id2 = stoi(input2);
      G->BFS(id2);
      cout << "Output data of all connected vertices from source airport " << id2 <<  ", and their respective distances from the source";
      cout << " has been successfully saved in \"BFS.txt\"!"<< endl;
    }
    else if(number == "2"){
      cout << "You have chosen Dijktra's" << endl;  
      cout << "Enter a valid OpenFlights airport id for the source airport you wish to find the shortest paths for " << endl;
      cout << "**NOTE: this airport id most have a valid route associated with it in the routes dataset to have a result" << endl;  
      string input;
      cin >> input;
      int id = stoi(input);

      map<int, vector<vector<int>>>predecessor = G->Dijkstras1(id);
      
      cout << "Enter the (case sensitive) letter of which option you woudld like to select: " << endl; 
      cout << "   A) Print all shortest paths from the source airport given the destination airport" << endl; 
      cout << "   B) Print all shortest paths from the source airport (default)" << endl; 
      cin >> input; 
      if(input == "A"){
        cout << "Enter a valid OpenFlights airport id for the destination airport get all shortest paths from the source" << endl; 
        cout << "**NOTE: this airport id most have a valid route associated with it in the routes dataset to have a result" << endl; 
        cin >> input; 
        int dest = stoi(input);
          if(predecessor[dest].size() == 0){
            cout << "These two airports are not connected!" << endl; 
          }
          for(auto path : predecessor[dest]){
            cout << "path: "; 
            for(auto airport : path){
              cout << airport << " ";  
            }
            cout << endl; 
          }
      }
      else{
        std::ofstream out("dijkstra.txt");
        out << "ALL shortest paths from source airport: " << id << endl << endl; 
        for(auto paths : predecessor){
          out << "destination: " << paths.first << endl;
          cout << "destination: " << paths.first << endl; 
          for(auto path : paths.second){
            cout << "   path: ";
            out << "    path: "; 
            for(auto airport : path){
              cout << airport << " ";  
              out << airport << ",";
            }
          cout << endl;
          out << endl; 
          }
        }
        cout << "Output data of all shortest paths from source airport " << id << " has been successfully saved in \"dijkstra.txt\"!"<< endl;  

      }
    }
    else if(number == "3"){
      cout << "You have chosen Betweenness Centrality" << endl;
      cout << "Enter a valid OpenFlights airport id for the source airport you wish to find the centrality for " << endl;
      cout << "**NOTE: this airport id most have a valid route associated with it in the routes dataset to have a result" << endl;
      cout << "**NOTE: betweenness centrality is very tedious to run on the full routes dataset and takes very very long," << endl;
      cout << "to see the results, it is better to exit the project and run the project a smaller routes dataset (ex: limited_routes.txt ";
      cout << "or routes_subset";
      cout << endl; 

      string input3;
      cin >> input3; 
      int id3 = stoi(input3);
      int result = G->Betweeness(id3);
      cout << "The betweenness centrality of source airport " << id3 << " is " << result << endl; 
      bc << "The betweenness centrality of source airport " << id3 << " is " << result << endl;
      cout << "The betweenness centrality calculation of " << id3 << " has been successfully saved in \"betweenness.txt\"!"<< endl;
    }
    else{
      proj = false;
    } 
  }    
  cout << "Exiting project" << endl;
  return 0;
}
