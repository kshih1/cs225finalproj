#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <map>
using namespace std;

/**
* The node class represents an object storing information about a destination airport to a source airport, and is meant 
* to be only stored in the adjList map of a source airport of type vertex. Each node contains information about the
* openflights airport id (adj_ID) for the destination airport, the spherical distance of the directed edge connecting 
* the source and destination aiports (weight), and the whether or not the edge between the vertex and node is a 
* undiscovered/discovery/cross edge (edgeState, used in BFS). 
*/
class Node{
    public: 
    int weight;     // spherical distance between source and destination airport rounded to nearest km
    int adj_ID; 
    int edgeState;  // keeps track if the type of edge (BFS)
                    // undiscovered: 0
                    // discovery: 1
                    // cross: 2
    
    /**
    * Creates a node object given a destination airport id, spherical distance, and edgeState
    * 
    * @param adj_ID_ a destination airport id, assigned by openflights
    * @param weight_ the weight of the edge/route, which is the spherical distance between the 2 airports
    * @param edgeState_ keeps track if the edge is undiscovered/discovery/cross (BFS)
    */
    Node(int adj_ID_, int weight_, int edgeState_); 
}; 


/**
* The vertex class represents an object storing information about an airport including its unique openflights airport 
* ID (airport_id), x and y coordinates as respective longitude and latitude decimal degrees (coordX & coordY), edges
* or routes going out from the airport to mapped destination airports (adjList), the unweighted distance from the vertex
* to the source vertx (distanceFromSource, used in BFS) and whether or not the vertex has been visited or not (vertexState,
* used in BFS).
*/ 
class vertex{
    public: 
    int airport_id; //openflights given airportid 
    double coordX;  // decimal degree for longitude
    double coordY;  // decimal degree for latitude
    map<int, Node*>adjList; // map storing direct route destination airports, with keys consisting of the airport id
                            // and a pointer to the destination airport node
    int distanceFromSource; // number of edges to node from specified source node (BFS)
    int vertexState;    // if the vertex has been visited or not (BFS)
                        // vertexUnexplored = 0; 
                        //vertexVisited = 1; 

    /**
    * This function creates a generic vertex object representing an airport with an airport id of 0, as well as latitude and longitude
    * values of 0.  
    */
    vertex(); 
    
    /**
    * This function creates a vertex object reprsenting an airport. Each vertex contains information about its airport
    * id, as well as its longtitude and latitude coordinates. 
    *
    * @param ID the aiport id, assigned by openflights
    * @param x_coord the longitude of the airport in terms of its decimal degree
    * @param y_coord the latitude of the airport in terms of tis decimal degree
    */
    vertex(int ID, double x_coord, double y_coord); 

    /**
    * This function effectively creates a edge/route between a source airport and a destination airport by creating a 
    * node representing a destination airport and inserting the node into the adjList map variable of a source airport 
    * vertex object.
    * 
    * @param destination the destination airport id, assigned by openflights
    * @param weight the spherical distance between the source and destination airport
    */
    void insertAdjNode(int destination, int weight);
};
