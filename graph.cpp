#include "graph.h"
#include "Airlines.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <set>
#include <iterator>
using namespace std;

//default constructor 
graph::graph()
{
}

//default deconstructor
graph::~graph(){
    map<int, vertex*>::iterator it; 
    for(it = v_arr.begin(); it != v_arr.end(); it++)
    {
        vertex* v = it->second;
        delete v;
    }
}

//Function traverses through the map of vertices in order to print it to the terminal
void graph::print_v_arr(){
    cout<<"Vertex Array"<<endl;

   map<int, vertex*>::iterator it; 
    for(it = v_arr.begin(); it != v_arr.end(); it++)
    {
        vertex* v = it->second;
        cout <<" starting new source vertex " << v -> airport_id << " vertex state:  " << v->vertexState  << endl;

        map<int, Node*>::iterator i; 
       for(i = v->adjList.begin(); i != v->adjList.end(); i++)
       {
           cout << " adjacent destination id: " << i->first << " edge state: " << i->second->edgeState << endl; 
       }
    }
}

/*
input: the source airport ID, the destination aiport ID and the weight of a particular vertex 
output: none
result: calculates the weight of every airport present in the v_arr map in km and sets 
a route/edge connection between the two airports(source and the destionation) through the 
helper function insertAdjNode() which sets edge an connection uses an adjacency list map 
*/
void graph::insert_edge(int source, int destination, int weight) {
    if (v_arr.find(source) == v_arr.end() || v_arr.find(destination) == v_arr.end())
    {
        return;
    }
    if(weight != -1){ //for ease in testing, this allows us to set the weight of an edge (otherwise use -1 to use calculate spherical dist)
        v_arr[source]->insertAdjNode(destination, weight);
        return; 
    }
    // code modified from https://www.geeksforgeeks.org/program-distance-two-points-earth/ as reference
    double R = 6371;//avg radius of Earth in km 

    // convert latitude&longitude from decimal deg to radians (longitude = x, latitude = y)
    double latSource = v_arr[source]->coordY * M_PI/180;
    double longSource = v_arr[source]->coordX * M_PI/180;
    double latDest = v_arr[destination]->coordY * M_PI/180;
    double longDest = v_arr[destination]->coordX * M_PI/180;

    // haversine formula 
    double latDist = latDest - latSource; 
    double longDist = longDest - longSource; 

    double a = pow(sin(latDist / 2), 2) + cos(latSource) * cos(latDest) * pow(sin(longDist / 2), 2); 
    double c = 2 * asin(sqrt(a)); // equivalent to c = 2 * atan(sqrt(a), sqrt(1-a))
    int distance = c * R; // the spherical distance of 2 airports in km (rounded to nearest km)
    v_arr[source]->insertAdjNode(destination, distance);
}


// this function is used to parse airport data from OpenFlights.org airport dataset
// it takes a string parameter that is the value of the airport dataset
// using file.io methods it parses and assigns airport ids along with their coordinates into the map
void graph::read_airports(string filename){
    // read airports--> for the vertex 
    string parser; 
    ifstream infile (filename); 

    while(getline(infile, parser)){

    string airportID, airportName, airportCity, airportCountry, iataID, icaoID, airportLatitude, airportLongitude; 
    string airportAltitude, airportTimezone, airportDST, airportTZtime, airportType, dataSource; 

    stringstream parseTool(parser); 

    getline(parseTool, airportID, ','); 
    getline(parseTool, airportName, ','); 
    getline(parseTool, airportCity, ','); 
    getline(parseTool, airportCountry, ','); 
    getline(parseTool, iataID, ',');
    getline(parseTool, icaoID, ','); 
    getline(parseTool, airportLatitude, ','); 
    getline(parseTool, airportLongitude, ','); 
    getline(parseTool, airportAltitude, ','); 
    getline(parseTool, airportTimezone, ',');
    getline(parseTool, airportDST, ','); 
    getline(parseTool, airportTZtime, ','); 
    getline(parseTool, airportType, ',');
    getline(parseTool, dataSource, ','); 

    if(airportID == "\\N" || airportLongitude == "\\N" || airportLatitude == "\\N"){
        continue; //if "\N" shows up any part of the useful data, ignore the airport completely and don't add it, move on to next one
    }
    int ID;
    ID = stoi(airportID);
    double x_coord = 0.0;
    double y_coord = 0.0;
    x_coord = stod(airportLongitude);
    y_coord = stod(airportLatitude);
    
    vertex *v = new vertex(ID, x_coord, y_coord); 

    //inserting vertex
    v_arr[v -> airport_id] = v;
    } 
    infile.close(); 
}


// this function is used to parse route data from OpenFlights.org route dataset
// it takes a string parameter that is the value of the route dataset
// using file.io methods it parses and inserts the routes as edges using the source and destination ids
void graph::read_routes(string filename){
    string parser; 
    ifstream infile (filename); 

    while(getline(infile, parser)){ 

    string airlineCode, airlineID, sourceAirport, sourceAirportID, destAirport, destAirportID, codeshareIndicator;
    string stopNumber, planeType;  

    stringstream parseTool(parser); 

    getline(parseTool, airlineCode, ','); 
    getline(parseTool, airlineID, ','); 
    getline(parseTool, sourceAirport, ','); 
    getline(parseTool, sourceAirportID, ',');
    getline(parseTool, destAirport, ',');
    getline(parseTool, destAirportID, ',');
    getline(parseTool, codeshareIndicator, ',');
    getline(parseTool, stopNumber, ','); 
    getline(parseTool, planeType, ',');

    if(sourceAirportID == "\\N" || destAirportID == "\\N"){ 
        continue; //if "\N" shows up any part of the useful data, ignore the route completely and don't add it, move to next route
    }
     int source_id, destination_id;
     source_id = stoi(sourceAirportID);
     destination_id = stoi(destAirportID);
     insert_edge(source_id, destination_id, -1); // weight = -1 means that it will be recalculated as the spherical distance of the edge
    }
    infile.close(); 
}


/*
input: the airport ID 
output: none 
result: given a source ID, thr algorithm creates a graph like structure with all the airports
included with their routes. We are able to determine the total distance of the vertices we visit from our 
starting source node. We also make sure to label the vertices and the routes visited during 
our traversal correctly to either undiscovered, discovery edges and cross edges
*/

void graph::BFS(int portId)
{
    std::ofstream out("BFS.txt");
    cout<< "Source airport: " << portId << endl; 
    out<< "source airport: " << portId << endl << endl;
    // marking all vertices unexplored
   map<int, vertex*>::iterator it; 
   for(it = v_arr.begin(); it != v_arr.end(); it++)
    {
        vertex* v = it->second;
        v->vertexState = 0;
        v->distanceFromSource = 0;
        // marking all edges unexplored
        map<int, Node*>::iterator i;
        for(i = v->adjList.begin(); i != v->adjList.end(); i++){
            Node *n = i->second;
            n->edgeState = 0; 
        } 
    }
   queue<int> q;
   v_arr[portId]->vertexState = 1; 
   q.push(portId); 

   while(!q.empty())
   {
       vertex* v1 = v_arr[q.front()]; 
       q.pop();  
       if(v1->adjList.size() == 0)
       {
           continue; 
       }

       map<int, Node*>::iterator i; 
       for(i = v1->adjList.begin(); i != v1->adjList.end(); i++)
       {
             if(v_arr.find(i->first) == v_arr.end()){
             }
           if(v_arr[i->first]->vertexState == 0)
           {
               i->second->edgeState = 1; 
               v_arr[i->first]->distanceFromSource = v1->distanceFromSource + 1; 
               v_arr[i->first]->vertexState = 1;
               cout<< "airport " << i->first << " is " << v_arr[i->first]->distanceFromSource << " edges away from the source airport "<< endl;
               out<< "airport " << i->first << " is " << v_arr[i->first]->distanceFromSource << " edges away from the source airport " << endl;
               q.push(i->first); 
           }

           else if(i->second->edgeState == 0)
           {
               i->second->edgeState = 2;  
           }
       }
   }
}

/*
input: the airport ID 
output: A map storing 2 dimensional vector with all the shortest paths from the starting node 
to every destination node given (key), that there is a valid path between them. If there is no valid
path the size of the 2d vector will be 0/empty. If it is the source node, the 2D vector will have a 
size of 1 and have a path consisting of only 1 value (the source node). Otherwise, it will store the
complete path from the given source id to the destination. 
result: given a source ID, the algorithm creates a graph like structure with all the airports
included with all their shortest routes stored in a map with the key being destination airports
and the mapped value a 2d vector of all the shortest paths. 
*/

map<int, vector<vector<int>>> graph::Dijkstras1(int source_id){
    map<int, double>distance; 
    map<int, vector<vector<int>>>predecessor; 
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> Q; 

    map<int, vertex*>::iterator it; 
    for(it = v_arr.begin(); it != v_arr.end(); it++){
        distance[it->first] = numeric_limits<double>::infinity();
    }

    distance[source_id] = 0.0; 
    vector<int> path;
    path.push_back(source_id); 
    predecessor[source_id].push_back(path);

    Q.push(make_pair(distance[source_id], source_id)); 

    while(!Q.empty()){

       int id = Q.top().second; 
       int distId = Q.top().first;  
       Q.pop(); 
       vertex* v1 = v_arr[id];
       map<int, Node*>::iterator i; 
       for(i = v1->adjList.begin(); i != v1->adjList.end(); i++)
       { 
           if(distance[i->first] > distId + i->second->weight){ // found a new shortest path to replace current shortest path(s)
               distance[i->first] = distId + i->second->weight; 
               Q.push(make_pair(distance[i->first], i->first)); 
               predecessor[i->first].clear(); 
               for(auto path : predecessor[id]){
                   vector<int> p = path;
                   p.push_back(i->first);
                   predecessor[i->first].push_back(p);
               }   
           }
           else if(distance[i->first] == distId + i->second->weight){ // add another valid shortest path(s) to vector of paths; 
                for(auto path : predecessor[id]){
                   vector<int> p = path;
                   p.push_back(i->first);
                   predecessor[i->first].push_back(p);
               }   
           }
       }

    }
    return predecessor; 
}

/*
input: the airport ID 
output: the centrality value for the given source airport ID 
result: We used Dijkstras in order to compute all the possible shortest paths 
of each of the pairs that do not inclide the centrality node. The total number 
of shortest paths betwen a pair would give us the denominator and the number of times
any of paths cross our centrality node would give us our numerator value for that
pair. Adding all the pairs numerator divided by the denominator values calculated 
this way would result in the centrality node for the chosen airport ID
*/
double graph::Betweeness(int source_id){ 
    vector<pair<int, int>>VertexPairs; 
    double totalCentrality = 0;
    double numerator = 0;   

    map<int, vertex*>::iterator it; 
    map<int, vertex*>::iterator it2; 
    for(it = v_arr.begin(); it != v_arr.end(); it++){
        for(it2 = v_arr.begin(); it2 != v_arr.end(); it2++){
            if(it2->first != it->first && it2->first != source_id && it->first != source_id){
                VertexPairs.push_back(pair<int, int>(it->first, it2->first));  
            }
        }
         map<int, vector<vector<int>>>predecent = Dijkstras1(it->first); 

        for(size_t i = 0; i < VertexPairs.size(); i++){
            int current = VertexPairs[i].second;  
             if(predecent[current].size() == 1 && predecent[0].size() == 1){ // just first condition is 0
                 continue; 
             }
            vector<vector<int>> paths = predecent[current];
            double denom = paths.size(); 
            for(auto path : paths){ 
                for(auto airport : path){
                    if(airport == source_id){
                        numerator++;     
                    } 
                }     
            }     
        if(denom != 0){ // if no path between the pair, not a valid pair to consider --> don't add (undef behavior)
            totalCentrality += (numerator/denom);
        } 
        numerator = 0; 
      }
      VertexPairs.clear();    
  } 
    return totalCentrality;
}

/*
input: none
output: none 
result: traverses through the vertex map and whenever we encounter an empty adjacency list 
of a particular vertex(this vertex was present in the airports dataset but not in the routes dataset), 
then we would remove that particular vertex from the map. 
*/

void graph::graphCleaning(){
    vector<int>erasingKeys; 

    for(auto it: v_arr){
        int keyNumber = it.first; 
        vertex *v = it.second; 
        map<int, Node*>::iterator i; 
        if(v->adjList.size() == 0)
        {
            erasingKeys.push_back(keyNumber); 
        }

    }

    for(unsigned long i = 0; i < erasingKeys.size(); i++){
        v_arr.erase(erasingKeys[i]); 

        for(auto it: v_arr){
            if(it.second->adjList.find(erasingKeys[i]) != it.second->adjList.end()){
                it.second->adjList.erase(erasingKeys[i]); 
            }
        }
    }
}
