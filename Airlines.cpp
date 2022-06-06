#include "Airlines.h"

/**
* This function creates a generic vertex object representing an airport with an airport id of 0, as well as latitude and longitude
* values of 0.  
*/
vertex::vertex()
{
    airport_id = 0; 
    coordX = 0.0; 
    coordY = 0.0; 
}

/**
* This function creates a vertex object reprsenting an airport. Each vertex contains information about its airport
* id. 
*
* @param ID the aiport id, assigned by openflights
* @param x_coord the longitude of the airport in terms of its decimal degree
* @param y_coord the latitude of the airport in terms of tis decimal degree
*/
vertex::vertex(int ID, double x_coord, double y_coord)
{
    airport_id = ID;
    coordX = x_coord; 
    coordY = y_coord; 
}

/**
* This function effectively creates a edge/route between a source airport and a destination airport by creating a 
* node representing a destination airport and inserting the node into the adjList map variable of a source airport 
* vertex object.
* 
* @param destination the destination airport id, assigned by openflights
* @param weight the spherical distance between the source and destination airport
*/
void vertex::insertAdjNode(int destination, int weight)
{
    Node *addVertx = new Node(destination, weight, 0);  
    adjList[destination] = addVertx; 
}

/**
* Creates a node object given a destination airport id, spherical distance, and edgeState
* 
* @param adj_ID_ a destination airport id, assigned by openflights
* @param weight_ the weight of the edge/route, which is the spherical distance between the 2 airports
* @param edgeState_ keeps track if the edge is undiscovered/discovery/cross (BFS)
*/
Node::Node(int adj_ID_, int weight_, int edgeState_)
{
    weight = weight_; 
    adj_ID = adj_ID_; 
    edgeState = edgeState_; 

}
