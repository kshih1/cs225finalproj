#include "../cs225/catch/catch.hpp"
#include "../Airlines.h"
#include "../graph.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

// testing data parsing
  TEST_CASE("read single airport"){
      graph* singleA = new graph();
      string airportData = "./tests/airport_single.txt"; 
      singleA->read_airports(airportData);
       REQUIRE(singleA->v_arr[1]->airport_id == 1); //require
       REQUIRE(singleA->v_arr[1]->coordX == 145.391998291);
       REQUIRE(singleA->v_arr[1]->coordY == -6.081689834590001);
  } 

 TEST_CASE("read two airports with a one way route"){
    graph* route1 = new graph();
    route1->read_airports("./tests/route_single_airports.txt");

     //check 2 airports read correctly
     REQUIRE(route1->v_arr[2965]->airport_id == 2965);
     REQUIRE(route1->v_arr[2965]->coordX == 39.9566); 
     REQUIRE(route1->v_arr[2965]->coordY == 43.449902);
     REQUIRE(route1->v_arr[2990]->airport_id == 2990);
     REQUIRE(route1->v_arr[2990]->coordX == 49.278701782227);
     REQUIRE(route1->v_arr[2990]->coordY == 55.606201171875); 

    // check route/edge info is correct
    route1->read_routes("./tests/route_single.txt");
    REQUIRE(route1->v_arr[2965]->adjList[2990]->weight == 1506);
    REQUIRE(route1->v_arr[2965]->adjList[2990]->adj_ID == 2990);
    REQUIRE(route1->v_arr[2965]->adjList[2990]->edgeState == 0);
 }

// complicated read airports & routes (airports with many routes, with missing essential info), with data cleaning
// small subset of actual data
TEST_CASE("complicated data read&parse"){
    graph* route1 = new graph();
    route1->read_airports("./tests/complex_airport.txt");
    REQUIRE(route1->v_arr[5446]->airport_id == 5446);
    REQUIRE(route1->v_arr[5446]->coordY == 77.4886016846);
    REQUIRE(route1->v_arr[5446]->coordX == -69.3887023926);
    REQUIRE(route1->v_arr[10]->airport_id == 10);
    REQUIRE(route1->v_arr[10]->coordY == 76.5311965942);
    REQUIRE(route1->v_arr[10]->coordX == -68.7032012939);
    REQUIRE(route1->v_arr[5448]->airport_id == 5448);
    REQUIRE(route1->v_arr[5448]->coordY == 72.7901992798);
    REQUIRE(route1->v_arr[5448]->coordX == -56.1305999756);

    REQUIRE(route1->v_arr[3]->airport_id == 3);
    route1->read_routes("./tests/complex_route.txt");
    route1->graphCleaning(); 
    // REQUIRE(route1->v_arr[3]->airport_id == 3); 
    // above case segfaults if not commented out, which is expected as graphCleaning() removes any airports without routes connected
    // to it
    REQUIRE(route1->v_arr[5446]->adjList[10]->weight == 107);
    REQUIRE(route1->v_arr[5446]->adjList[10]->adj_ID == 10);
    REQUIRE(route1->v_arr[5446]->adjList[10]->edgeState == 0);

    REQUIRE(route1->v_arr[5446]->adjList[5448]->weight == 641);
    REQUIRE(route1->v_arr[5446]->adjList[5448]->adj_ID == 5448);
    REQUIRE(route1->v_arr[5446]->adjList[5448]->edgeState == 0);

    REQUIRE(route1->v_arr[10]->adjList[5446]->weight == 107);
    REQUIRE(route1->v_arr[5448]->adjList[5446]->weight == 641);
}   

// //testing BFS
// //check all vertices have been visited
// //check only certain edges have been visited (0/1/2)
TEST_CASE("BFS path"){ 
    // make graph
    graph* g = new graph();
    // add dummy vertices
    vertex *v1 = new vertex(1, 1, 1); 
    g->v_arr[v1->airport_id] = v1;
    vertex *v2 = new vertex(2, 2, 2); 
    g->v_arr[v2->airport_id] = v2;
    vertex *v3 = new vertex(3, 3, 3); 
    g->v_arr[v3->airport_id] = v3;
    vertex *v4 = new vertex(4, 4, 4); 
    g->v_arr[v4->airport_id] = v4;
    vertex *v5 = new vertex(5, 4, 4); 
    g->v_arr[v5->airport_id] = v5;
    vertex *v6 = new vertex(6, 4, 4); 
    g->v_arr[v6->airport_id] = v6;
    vertex *v7 = new vertex(7, 4, 4); 
    g->v_arr[v7->airport_id] = v7;

    // 1 -> 2 -> 3-> 4 -> 5 -> 6 -> 7
    g->insert_edge(1, 2, -1);
    g->insert_edge(2, 3, -1);
    g->insert_edge(3, 4, -1);
    g->insert_edge(4, 5, -1);
    g->insert_edge(5, 6, -1);
    g->insert_edge(6, 7, -1);

    g->BFS(1);     
    //check all connected BFS vertices are explored 
    map<int, vertex*>::iterator it;
    for(it = g->v_arr.begin(); it != g->v_arr.end(); it++)
    {
        vertex* v = it->second;
        REQUIRE(v->vertexState == 1);
    }

    //check only some edges are discovered & others are cross edges
    REQUIRE(g->v_arr[1]->adjList[2]->edgeState == 1);
    REQUIRE(g->v_arr[2]->adjList[3]->edgeState == 1);
    REQUIRE(g->v_arr[3]->adjList[4]->edgeState == 1);
    REQUIRE(g->v_arr[4]->adjList[5]->edgeState == 1);
    REQUIRE(g->v_arr[5]->adjList[6]->edgeState == 1);
    REQUIRE(g->v_arr[6]->adjList[7]->edgeState == 1);

    // check correct distanceFromSource
    REQUIRE(g->v_arr[1]->distanceFromSource == 0);
    REQUIRE(g->v_arr[2]->distanceFromSource == 1);
    REQUIRE(g->v_arr[3]->distanceFromSource == 2);
    REQUIRE(g->v_arr[4]->distanceFromSource == 3);
    REQUIRE(g->v_arr[5]->distanceFromSource == 4);
    REQUIRE(g->v_arr[6]->distanceFromSource == 5);
    REQUIRE(g->v_arr[7]->distanceFromSource == 6);

    g->BFS(2);
    //check only vertices with a path from 2 are explored
    for(it = g->v_arr.begin(); it != g->v_arr.end(); it++)
    {
        vertex* v = it->second;
        if(v->airport_id != 1){
            REQUIRE(v->vertexState == 1);
        }
        else{
            REQUIRE(v->vertexState == 0);
        }
    }

    REQUIRE(g->v_arr[1]->adjList[2]->edgeState == 0);
    REQUIRE(g->v_arr[2]->adjList[3]->edgeState == 1);
    REQUIRE(g->v_arr[3]->adjList[4]->edgeState == 1);
    REQUIRE(g->v_arr[4]->adjList[5]->edgeState == 1);
    REQUIRE(g->v_arr[5]->adjList[6]->edgeState == 1);
    REQUIRE(g->v_arr[6]->adjList[7]->edgeState == 1);

    REQUIRE(g->v_arr[3]->distanceFromSource == 1);
    REQUIRE(g->v_arr[4]->distanceFromSource == 2);
    REQUIRE(g->v_arr[5]->distanceFromSource == 3);
    REQUIRE(g->v_arr[6]->distanceFromSource == 4);
    REQUIRE(g->v_arr[7]->distanceFromSource == 5);
}

TEST_CASE("BFS loop path"){
    // make graph
    graph* g = new graph();
    // add dummy vertices
    vertex *v1 = new vertex(1, 1, 1); 
    g->v_arr[v1->airport_id] = v1;
    vertex *v2 = new vertex(2, 2, 2); 
    g->v_arr[v2->airport_id] = v2;
    vertex *v3 = new vertex(3, 3, 3); 
    g->v_arr[v3->airport_id] = v3;
    vertex *v4 = new vertex(4, 4, 4); 
    g->v_arr[v4->airport_id] = v4;
    vertex *v5 = new vertex(5, 4, 4); 
    g->v_arr[v5->airport_id] = v5;
    vertex *v6 = new vertex(6, 4, 4); 
    g->v_arr[v6->airport_id] = v6;
    vertex *v7 = new vertex(7, 4, 4); 
    g->v_arr[v7->airport_id] = v7;

    //insert edges 
    g->insert_edge(1, 2, -1);
    g->insert_edge(2, 3, -1);
    g->insert_edge(3, 4, -1);
    g->insert_edge(4, 5, -1);
    g->insert_edge(5, 6, -1);
    g->insert_edge(6, 7, -1);
    g->insert_edge(7, 1, -1);

    g->BFS(4);
    //check all connected BFS vertices are explored
    map<int, vertex*>::iterator it;
    for(it = g->v_arr.begin(); it != g->v_arr.end(); it++)
    {
        vertex* v = it->second;
        REQUIRE(v->vertexState == 1);
    }
    // check appropriate edges are explored
    REQUIRE(g->v_arr[1]->adjList[2]->edgeState == 1);
    REQUIRE(g->v_arr[2]->adjList[3]->edgeState == 1);
    REQUIRE(g->v_arr[3]->adjList[4]->edgeState == 2);
    REQUIRE(g->v_arr[4]->adjList[5]->edgeState == 1);
    REQUIRE(g->v_arr[5]->adjList[6]->edgeState == 1);
    REQUIRE(g->v_arr[6]->adjList[7]->edgeState == 1);
    REQUIRE(g->v_arr[7]->adjList[1]->edgeState == 1);

    //check correct distfromsource
    REQUIRE(g->v_arr[5]->distanceFromSource == 1);
    REQUIRE(g->v_arr[6]->distanceFromSource == 2);
    REQUIRE(g->v_arr[7]->distanceFromSource == 3);
    REQUIRE(g->v_arr[1]->distanceFromSource == 4);
    REQUIRE(g->v_arr[2]->distanceFromSource == 5);
    REQUIRE(g->v_arr[3]->distanceFromSource == 6);

    g->BFS(3);
    //check appropriate edges are explored
    REQUIRE(g->v_arr[1]->adjList[2]->edgeState == 1);
    REQUIRE(g->v_arr[2]->adjList[3]->edgeState == 2);
    REQUIRE(g->v_arr[3]->adjList[4]->edgeState == 1);
    REQUIRE(g->v_arr[4]->adjList[5]->edgeState == 1);
    REQUIRE(g->v_arr[5]->adjList[6]->edgeState == 1);
    REQUIRE(g->v_arr[6]->adjList[7]->edgeState == 1);
    REQUIRE(g->v_arr[7]->adjList[1]->edgeState == 1);
}

TEST_CASE("BFS tree"){
    // make graph
    graph* g = new graph();
    // add dummy vertices
    vertex *v1 = new vertex(1, 1, 1); 
    g->v_arr[v1->airport_id] = v1;
    vertex *v2 = new vertex(2, 2, 2); 
    g->v_arr[v2->airport_id] = v2;
    vertex *v3 = new vertex(3, 3, 3); 
    g->v_arr[v3->airport_id] = v3;
    vertex *v4 = new vertex(4, 4, 4); 
    g->v_arr[v4->airport_id] = v4;
    vertex *v5 = new vertex(5, 4, 4); 
    g->v_arr[v5->airport_id] = v5;
    vertex *v6 = new vertex(6, 4, 4); 
    g->v_arr[v6->airport_id] = v6;
    vertex *v7 = new vertex(7, 4, 4); 
    g->v_arr[v7->airport_id] = v7;

    //insert edges
    g->insert_edge(1, 2, -1);
    g->insert_edge(1, 3, -1);
    g->insert_edge(2, 4, -1);
    g->insert_edge(2, 5, -1);
    g->insert_edge(3, 6, -1);
    g->insert_edge(3, 7, -1);

    g->BFS(1);     
    //check all BFS vertices are explored
    map<int, vertex*>::iterator it; 
    for(it = g->v_arr.begin(); it != g->v_arr.end(); it++)
    {
        vertex* v = it->second;
        REQUIRE(v->vertexState == 1);
    }

    //check only all edges discovery in this case
    REQUIRE(g->v_arr[1]->adjList[2]->edgeState == 1);
    REQUIRE(g->v_arr[1]->adjList[3]->edgeState == 1);
    REQUIRE(g->v_arr[2]->adjList[4]->edgeState == 1);
    REQUIRE(g->v_arr[2]->adjList[5]->edgeState == 1);
    REQUIRE(g->v_arr[3]->adjList[6]->edgeState == 1);
    REQUIRE(g->v_arr[3]->adjList[7]->edgeState == 1);

    REQUIRE(g->v_arr[2]->distanceFromSource == 1);
    REQUIRE(g->v_arr[3]->distanceFromSource == 1);
    REQUIRE(g->v_arr[4]->distanceFromSource == 2);
    REQUIRE(g->v_arr[5]->distanceFromSource == 2);
    REQUIRE(g->v_arr[6]->distanceFromSource == 2);
    REQUIRE(g->v_arr[6]->distanceFromSource == 2);

    g->BFS(6);
    // only 6 is explored bc no outgoing edges from 6
    for(it = g->v_arr.begin(); it != g->v_arr.end(); it++)
    {
        vertex *v = it->second; 
        if(v->airport_id == 6){
            REQUIRE(v->vertexState == 1);
        }
        else{
            REQUIRE(v->vertexState == 0);
        }
    }
    // no edges explored for same reason
    REQUIRE(g->v_arr[1]->adjList[2]->edgeState == 0);
    REQUIRE(g->v_arr[1]->adjList[3]->edgeState == 0);
    REQUIRE(g->v_arr[2]->adjList[4]->edgeState == 0);
    REQUIRE(g->v_arr[2]->adjList[5]->edgeState == 0);
    REQUIRE(g->v_arr[3]->adjList[6]->edgeState == 0);
    REQUIRE(g->v_arr[3]->adjList[7]->edgeState == 0);
}

TEST_CASE("BFS tree with loop"){
    // make graph
    graph* g = new graph();
    // add dummy vertices
    vertex *v1 = new vertex(1, 1, 1); 
    g->v_arr[v1->airport_id] = v1;
    vertex *v2 = new vertex(2, 2, 2); 
    g->v_arr[v2->airport_id] = v2;
    vertex *v3 = new vertex(3, 3, 3); 
    g->v_arr[v3->airport_id] = v3;
    vertex *v4 = new vertex(4, 4, 4); 
    g->v_arr[v4->airport_id] = v4;
    vertex *v5 = new vertex(5, 4, 4); 
    g->v_arr[v5->airport_id] = v5;
    vertex *v6 = new vertex(6, 4, 4); 
    g->v_arr[v6->airport_id] = v6;
    vertex *v7 = new vertex(7, 4, 4); 
    g->v_arr[v7->airport_id] = v7;

    //insert edges 
    g->insert_edge(1, 2, -1);
    g->insert_edge(1, 3, -1);
    g->insert_edge(2, 4, -1);
    g->insert_edge(4, 2, -1);
    g->insert_edge(2, 5, -1);
    g->insert_edge(3, 6, -1);
    g->insert_edge(3, 7, -1);
    g->insert_edge(6, 2, -1);

    g->BFS(1);     
    //check all BFS vertices are explored
    map<int, vertex*>::iterator it; 
    for(it = g->v_arr.begin(); it != g->v_arr.end(); it++)
    {
        vertex* v = it->second;
        REQUIRE(v->vertexState == 1);
    }

    //check only some edges are discovered & others are cross edges
    REQUIRE(g->v_arr[1]->adjList[2]->edgeState == 1);
    REQUIRE(g->v_arr[1]->adjList[3]->edgeState == 1);
    REQUIRE(g->v_arr[2]->adjList[4]->edgeState == 1);
    REQUIRE(g->v_arr[2]->adjList[5]->edgeState == 1);
    REQUIRE(g->v_arr[3]->adjList[6]->edgeState == 1);
    REQUIRE(g->v_arr[3]->adjList[7]->edgeState == 1);
    REQUIRE(g->v_arr[4]->adjList[2]->edgeState == 2);
    REQUIRE(g->v_arr[6]->adjList[2]->edgeState == 2);

    g->BFS(3);
    //explore all edges except 1 (no path to it)
    for(it = g->v_arr.begin(); it != g->v_arr.end(); it++)
    {
        vertex* v = it->second;
        if(v->airport_id == 1){
            REQUIRE(v->vertexState == 0);
        }
        else{
            REQUIRE(v->vertexState == 1);
        }
    }
    //check only some edges are discovered, cross, and undiscovered 
    REQUIRE(g->v_arr[1]->adjList[2]->edgeState == 0);
    REQUIRE(g->v_arr[1]->adjList[3]->edgeState == 0);
    REQUIRE(g->v_arr[2]->adjList[4]->edgeState == 1);
    REQUIRE(g->v_arr[2]->adjList[5]->edgeState == 1);
    REQUIRE(g->v_arr[3]->adjList[6]->edgeState == 1);
    REQUIRE(g->v_arr[3]->adjList[7]->edgeState == 1);
    REQUIRE(g->v_arr[4]->adjList[2]->edgeState == 2);
    REQUIRE(g->v_arr[6]->adjList[2]->edgeState == 1);

    REQUIRE(g->v_arr[6]->distanceFromSource == 1);
    REQUIRE(g->v_arr[7]->distanceFromSource == 1);
    REQUIRE(g->v_arr[2]->distanceFromSource == 2);
    REQUIRE(g->v_arr[4]->distanceFromSource == 3);
    REQUIRE(g->v_arr[5]->distanceFromSource == 3);
}

TEST_CASE("BFS"){
    // make graph
    graph* g = new graph();
    // add dummy vertices
    vertex *v1 = new vertex(1, 1, 1); 
    g->v_arr[v1->airport_id] = v1;
    vertex *v2 = new vertex(2, 2, 2); 
    g->v_arr[v2->airport_id] = v2;
    vertex *v3 = new vertex(3, 3, 3); 
    g->v_arr[v3->airport_id] = v3;
    vertex *v4 = new vertex(4, 4, 4); 
    g->v_arr[v4->airport_id] = v4;

    //insert edges so k4 
    g->insert_edge(1, 2, -1);
    g->insert_edge(1, 3, -1);
    g->insert_edge(1, 4, -1);
    g->insert_edge(2, 1, -1);
    g->insert_edge(2, 3, -1);
    g->insert_edge(2, 4, -1);
    g->insert_edge(3, 1, -1);
    g->insert_edge(3, 2, -1);
    g->insert_edge(3, 4, -1);
    g->insert_edge(4, 1, -1);
    g->insert_edge(4, 2, -1);
    g->insert_edge(4, 3, -1);

    REQUIRE(g->v_arr[1]->adjList[2]->adj_ID == 2);
    REQUIRE(g->v_arr[2]->adjList[1]->adj_ID == 1);
    // REQUIRE(g->v_arr[1]->adjList[4]->weight == 3);

    g->BFS(1);     
    //check all BFS vertices are explored
    map<int, vertex*>::iterator it; 
    for(it = g->v_arr.begin(); it != g->v_arr.end(); it++)
    {
        vertex* v = it->second;
        REQUIRE(v->vertexState == 1);
    }

    //check only some edges are discovered & others are cross edges
    REQUIRE(g->v_arr[1]->adjList[2]->edgeState == 1);
    REQUIRE(g->v_arr[1]->adjList[3]->edgeState == 1);
    REQUIRE(g->v_arr[1]->adjList[4]->edgeState == 1);
    REQUIRE(g->v_arr[2]->adjList[1]->edgeState == 2);
    REQUIRE(g->v_arr[2]->adjList[3]->edgeState == 2);
    REQUIRE(g->v_arr[2]->adjList[4]->edgeState == 2);
    REQUIRE(g->v_arr[3]->adjList[1]->edgeState == 2);
    REQUIRE(g->v_arr[3]->adjList[2]->edgeState == 2);
    REQUIRE(g->v_arr[3]->adjList[4]->edgeState == 2);
    REQUIRE(g->v_arr[4]->adjList[1]->edgeState == 2);
    REQUIRE(g->v_arr[4]->adjList[2]->edgeState == 2);
    REQUIRE(g->v_arr[4]->adjList[3]->edgeState == 2);

    REQUIRE(g->v_arr[2]->distanceFromSource == 1);
    REQUIRE(g->v_arr[3]->distanceFromSource == 1);
    REQUIRE(g->v_arr[4]->distanceFromSource == 1);

    g->BFS(3);
    // all edges explored
    for(it = g->v_arr.begin(); it != g->v_arr.end(); it++)
    {
        vertex* v = it->second;
        REQUIRE(v->vertexState == 1);
    }
    //check only some edges are discovered & others are cross edges
    REQUIRE(g->v_arr[1]->adjList[2]->edgeState == 2);
    REQUIRE(g->v_arr[1]->adjList[3]->edgeState == 2);
    REQUIRE(g->v_arr[1]->adjList[4]->edgeState == 2);
    REQUIRE(g->v_arr[2]->adjList[1]->edgeState == 2);
    REQUIRE(g->v_arr[2]->adjList[3]->edgeState == 2);
    REQUIRE(g->v_arr[2]->adjList[4]->edgeState == 2);
    REQUIRE(g->v_arr[3]->adjList[1]->edgeState == 1);
    REQUIRE(g->v_arr[3]->adjList[2]->edgeState == 1);
    REQUIRE(g->v_arr[3]->adjList[4]->edgeState == 1);
    REQUIRE(g->v_arr[4]->adjList[1]->edgeState == 2);
    REQUIRE(g->v_arr[4]->adjList[2]->edgeState == 2);
    REQUIRE(g->v_arr[4]->adjList[3]->edgeState == 2);
}

// tests Dijkstra1 on a weighted graph, at all nodes, with some node pairs having multiple shortest paths
TEST_CASE("dijkstra1"){
    // make graph
    graph* g = new graph();
    // add dummy vertices
    vertex *v1 = new vertex(1, 1, 1); 
    g->v_arr[v1->airport_id] = v1;
    vertex *v2 = new vertex(2, 2, 2); 
    g->v_arr[v2->airport_id] = v2;
    vertex *v3 = new vertex(3, 3, 3); 
    g->v_arr[v3->airport_id] = v3;
    vertex *v4 = new vertex(4, 4, 4); 
    g->v_arr[v4->airport_id] = v4;

    g->insert_edge(1, 2, 2);

    g->insert_edge(1, 3, 3);
    g->insert_edge(3, 1, 3);

    g->insert_edge(2, 3, 5);

    g->insert_edge(2, 4, 3);
    g->insert_edge(4, 2, 3);

    g->insert_edge(3, 4, 2);
    g->insert_edge(4, 3, 2);

    //source node is 3
    map<int, vector<vector<int>>>predecessor = g->Dijkstras1(3);
    vector<vector<int>> paths; 

    paths = predecessor[1];
    REQUIRE(paths[0][0] == 3); // check shortest path from 3 -> 1 = {{3, 1}}
    REQUIRE(paths[0][1] == 1);

    paths = predecessor[2]; 
    REQUIRE(paths[0][0] == 3); // check shortest paths from 3 -> 2 = {{3, 4, 2}, {3, 1, 2}}
    REQUIRE(paths[0][1] == 4);
    REQUIRE(paths[0][2] == 2);
    REQUIRE(paths[1][0] == 3);
    REQUIRE(paths[1][1] == 1);
    REQUIRE(paths[1][2] == 2);

    paths = predecessor[3];
    REQUIRE(paths[0][0] == 3); // check shortest path from 3 -> 3 is only itself

    paths = predecessor[4]; // check shortest path from 3 -> 4 = {{3, 4}}
    REQUIRE(paths[0][0] == 3);
    REQUIRE(paths[0][1] == 4);

    // source node is now 1
    predecessor = g->Dijkstras1(1);

    paths = predecessor[1]; 
    REQUIRE(paths[0][0] == 1); // check shortest path from 1 -> 1 is only itself

    paths = predecessor[2]; // shortest path 1 -> 2 = {{1, 2}}
    REQUIRE(paths[0][0] == 1);
    REQUIRE(paths[0][1] == 2);

    paths = predecessor[3]; // shortest path 1 -> 3 = {{1, 3}}
    REQUIRE(paths[0][0] == 1);
    REQUIRE(paths[0][1] == 3);

    paths = predecessor[4]; // shortest path 1 -> 4 = {{1, 2, 4}, {1, 3, 4}}
    REQUIRE(paths[0][0] == 1);
    REQUIRE(paths[0][1] == 2);
    REQUIRE(paths[0][2] == 4);
    REQUIRE(paths[1][0] == 1);
    REQUIRE(paths[1][1] == 3);
    REQUIRE(paths[1][2] == 4);

    // source node is now 2
    predecessor = g->Dijkstras1(2); 

    paths = predecessor[1]; // shortest path 2 -> 1 = {{2, 3, 1}, {2, 4, 3, 1}}
    REQUIRE(paths[0][0] == 2);
    REQUIRE(paths[0][1] == 3);
    REQUIRE(paths[0][2] == 1);
    REQUIRE(paths[1][0] == 2);
    REQUIRE(paths[1][1] == 4);
    REQUIRE(paths[1][2] == 3);
    REQUIRE(paths[1][3] == 1);

    paths = predecessor[2]; // shortest path 2 -> 2 itself
    REQUIRE(paths[0][0] == 2);

    paths = predecessor[3]; // shortest path 2 -> 3 {{2, 3}, {2, 4, 3}}
    REQUIRE(paths[0][0] == 2);
    REQUIRE(paths[0][1] == 3);
    REQUIRE(paths[1][0] == 2);
    REQUIRE(paths[1][1] == 4);
    REQUIRE(paths[1][2] == 3);

    paths = predecessor[4]; // shortest path 2 -> 4 {{2, 4}}
    REQUIRE(paths[0][0] == 2);
    REQUIRE(paths[0][1] == 4);

    //source node is now 4
    predecessor = g->Dijkstras1(4); 
    paths = predecessor[1]; // shortest path 4 -> 1 {{4, 3, 1}}
    REQUIRE(paths[0][0] == 4);
    REQUIRE(paths[0][1] == 3);
    REQUIRE(paths[0][2] == 1);
    
    paths = predecessor[2]; // shortest path 4 -> 2 {{4, 2}}
    REQUIRE(paths[0][0] == 4);
    REQUIRE(paths[0][1] == 2);

    paths = predecessor[3]; // shortest path 4 -> 3 {{4, 3}}
    REQUIRE(paths[0][0] == 4);
    REQUIRE(paths[0][1] == 3);

    paths = predecessor[4]; // shortest path 4 -> 4 itself
    REQUIRE(paths[0][0] == 4);
}

TEST_CASE("dijsktra2"){
    // make graph: another directed graph, but there are no possible paths from some nodes to others
    graph* g = new graph();
    // add dummy vertices
    vertex *v1 = new vertex(1, 1, 1); 
    g->v_arr[v1->airport_id] = v1;
    vertex *v2 = new vertex(2, 2, 2); 
    g->v_arr[v2->airport_id] = v2;
    vertex *v3 = new vertex(3, 3, 3); 
    g->v_arr[v3->airport_id] = v3;
    vertex *v4 = new vertex(4, 4, 4); 
    g->v_arr[v4->airport_id] = v4;
    vertex *v5 = new vertex(5, 5, 5); 
    g->v_arr[v5->airport_id] = v5;
    vertex *v6 = new vertex(6, 6, 6); 
    g->v_arr[v6->airport_id] = v6;
    

    g->insert_edge(1, 2, 9);

    g->insert_edge(1, 3, 4);
    g->insert_edge(3, 2, 4);

    g->insert_edge(2, 4, 12);

    g->insert_edge(2, 5, 5);
    g->insert_edge(3, 5, 13);

    g->insert_edge(5, 4, 3);
    g->insert_edge(4, 6, 2);

    g->insert_edge(5, 6, 15); 

    //source is 1
    map<int, vector<vector<int>>>predecessor = g->Dijkstras1(1);

    vector<vector<int>> paths; 
    paths = predecessor[1];
    REQUIRE(paths[0][0] == 1); // check shortest path from 1 -> 1 = {{ 1 }}

    paths = predecessor[2]; 
    REQUIRE(paths[0][0] == 1); // check shortest paths from 1 -> 2 = {{1, 3, 2}}
    REQUIRE(paths[0][1] == 3);
    REQUIRE(paths[0][2] == 2);

    paths = predecessor[3];
    REQUIRE(paths[0][0] == 1); // check shortest path from 1 - 3 = {{1, 3}}
    REQUIRE(paths[0][1] == 3);

    paths = predecessor[4]; // check shortest path from 1 -> 4 = {{1, 3, 2, 5, 4}}
    REQUIRE(paths[0][0] == 1);
    REQUIRE(paths[0][1] == 3);
    REQUIRE(paths[0][2] == 2);
    REQUIRE(paths[0][3] == 5);
    REQUIRE(paths[0][4] == 4);

    paths = predecessor[5]; // check shortest path from 1 -> 5 = {{1, 3, 2, 5}}
    REQUIRE(paths[0][0] == 1);
    REQUIRE(paths[0][1] == 3);
    REQUIRE(paths[0][2] == 2);
    REQUIRE(paths[0][3] == 5);

    paths = predecessor[6]; // check shortest path from 1 -> 6 = {{1, 3, 2, 5, 4, 6}}
    REQUIRE(paths[0][0] == 1);
    REQUIRE(paths[0][1] == 3);
    REQUIRE(paths[0][2] == 2);
    REQUIRE(paths[0][3] == 5);
    REQUIRE(paths[0][4] == 4);
    REQUIRE(paths[0][5] == 6);
    
    predecessor = g->Dijkstras1(2);
    
    paths = predecessor[1]; // check shortest path from 2 -> 1 is nothing
    REQUIRE(paths.size() == 0); // in this case no path from 2 -> 1 as there is no possible path that way so paths are empty

    paths = predecessor[2]; // check shortest path from 2 -> 2 = {{2}}
    REQUIRE(paths[0][0] == 2);

    paths = predecessor[3]; // check shortest path from 2 -> 3 doesn't exist
    REQUIRE(paths.size() == 0);

    paths = predecessor[4]; // check shortest path from 2 -> 4 = {{2, 5, 4}}
    REQUIRE(paths[0][0] == 2);
    REQUIRE(paths[0][1] == 5);
    REQUIRE(paths[0][2] == 4);

    paths = predecessor[5]; // check shortest path from 2 -> 5 = {{2, 5}}
    REQUIRE(paths[0][0] == 2);
    REQUIRE(paths[0][1] == 5);
    
    paths = predecessor[6]; // check shortest path from 2 -> 6 = {{2, 5, 4, 6}}
    REQUIRE(paths[0][0] == 2);
    REQUIRE(paths[0][1] == 5);
    REQUIRE(paths[0][2] == 4);
    REQUIRE(paths[0][3] == 6);

    predecessor = g->Dijkstras1(3);
    paths = predecessor[1]; // check shortest path from 3 -> 1 doesn't exist 
    REQUIRE(paths.size() == 0);

    paths = predecessor[2]; // check shortest path from 3 -> 2 = {{3, 2}}
    REQUIRE(paths[0][0] == 3);
    REQUIRE(paths[0][1] == 2);

    paths = predecessor[3]; // check shortest path from 3 -> 3 = {{3}}
    REQUIRE(paths[0][0] == 3);

    paths = predecessor[4]; // check shortest path from 3 -> 4 = {{3, 2, 5, 4}}
    REQUIRE(paths[0][0] == 3);
    REQUIRE(paths[0][1] == 2);
    REQUIRE(paths[0][2] == 5);
    REQUIRE(paths[0][3] == 4);

    paths = predecessor[5]; // check shortest path from 3 -> 5 = {{3, 2, 5}}
    REQUIRE(paths[0][0] == 3);
    REQUIRE(paths[0][1] == 2);
    REQUIRE(paths[0][2] == 5);

    paths = predecessor[6]; // check shortest path from 3 -> 6 = {{3, 2, 5, 4, 6}}
    REQUIRE(paths[0][0] == 3);
    REQUIRE(paths[0][1] == 2);
    REQUIRE(paths[0][2] == 5);
    REQUIRE(paths[0][3] == 4);
    REQUIRE(paths[0][4] == 6);
    
    predecessor = g->Dijkstras1(4);
    paths = predecessor[1]; // check shortest paths from 4 to each of: 1, 2, 3, 5 doesn't exist (only has edge pointing to 6)
    REQUIRE(paths.size() == 0);
    paths = predecessor[2];
    REQUIRE(paths.size() == 0);
    paths = predecessor[3];
    REQUIRE(paths.size() == 0);
    paths = predecessor[5];
    REQUIRE(paths.size() == 0);

    paths = predecessor[4]; // check shortest path 4 -> 4 = {{4}}
    REQUIRE(paths[0][0] == 4);

    paths = predecessor[6]; // check shortest path 4 -> 6 = {{4, 6}}
    REQUIRE(paths[0][0] == 4);
    REQUIRE(paths[0][1] == 6);

    predecessor = g->Dijkstras1(5);
    paths = predecessor[1]; // check shortest paths from 5 to each of: 1, 2, 3 doesn't exist
    REQUIRE(paths.size() == 0);
    paths = predecessor[2];
    REQUIRE(paths.size() == 0);
    paths = predecessor[3];
    REQUIRE(paths.size() == 0);

    paths = predecessor[4]; // check shortest path from 5 -> 4 = {{5, 4}}
    REQUIRE(paths[0][0] == 5);
    REQUIRE(paths[0][1] == 4);

    paths = predecessor[5]; // check shortest path from 5 -> 5 = {{5}}
    REQUIRE(paths[0][0] == 5);

    paths = predecessor[6]; // check shortest path from 5 -> 6 = {{5, 4, 6}}
    REQUIRE(paths[0][0] == 5);
    REQUIRE(paths[0][1] == 4);
    REQUIRE(paths[0][2] == 6);
    
    predecessor = g->Dijkstras1(6); // check no paths going out of 6 is true
    paths = predecessor[1]; 
    REQUIRE(paths.size() == 0);
    paths = predecessor[2];
    REQUIRE(paths.size() == 0);
    paths = predecessor[3];
    REQUIRE(paths.size() == 0);
    paths = predecessor[4]; 
    REQUIRE(paths.size() == 0);
    paths = predecessor[5];
    REQUIRE(paths.size() == 0);
    paths = predecessor[6]; // path 6 -> 6 = {6}
    REQUIRE(paths[0][0] == 6);
}

TEST_CASE("betweeness"){
    // make graph (same as Dijkstra 1, can confirm w actual shortest paths that calculated fractions for each pair are correct)
    graph* g = new graph();
    // add dummy vertices (A = 1, B = 2, C = 3, D = 4)
    vertex *v1 = new vertex(1, 1, 1); 
    g->v_arr[v1->airport_id] = v1;
    vertex *v2 = new vertex(2, 2, 2); 
    g->v_arr[v2->airport_id] = v2;
    vertex *v3 = new vertex(3, 3, 3); 
    g->v_arr[v3->airport_id] = v3;
    vertex *v4 = new vertex(4, 4, 4); 
    g->v_arr[v4->airport_id] = v4;

    g->insert_edge(1, 2, 2);

    g->insert_edge(1, 3, 3);
    g->insert_edge(3, 1, 3);

    g->insert_edge(2, 3, 5);

    g->insert_edge(2, 4, 3);
    g->insert_edge(4, 2, 3);

    g->insert_edge(3, 4, 2);
    g->insert_edge(4, 3, 2);

    REQUIRE(g->Betweeness(1) == 0.5); // BC (0/2) + BD (0/1) + CB (1/2) + CD (0/1) + DB (0/1) + DC (0/1) = 0.5
    REQUIRE(g->Betweeness(2) == 0.5); // AC (0/1) + AD (1/2) + CA (0/1) + CD (0/1) + DA (0/1) + DC (0/1) = 0.5
    REQUIRE(g->Betweeness(3) == 2.5); // AB (0/1) + AD (1/2) + BA (2/2) + BC (0/1) + DA (1/1) + DB (0/1) = 2.5
    REQUIRE(g->Betweeness(4) == 1.5); // AB (0/1) + AC (2/2) + BA (1/2) + BC (1/2) + CA (0/1) + CB (1/2) = 1.5
}

//instance when there are unconnected pairs of source/destination even though graph is connected
TEST_CASE("betweeness2"){
    // make graph (same as Dijkstra 2, can confirm w actual shortest paths that calculated fractions for each pair are correct)
    graph* g = new graph();
    // add dummy vertices (let letters reference id for more ease: A = 1, B = 2, C = 3, D = 4, E = 5, F = 6)
    vertex *v1 = new vertex(1, 1, 1); 
    g->v_arr[v1->airport_id] = v1;
    vertex *v2 = new vertex(2, 2, 2); 
    g->v_arr[v2->airport_id] = v2;
    vertex *v3 = new vertex(3, 3, 3); 
    g->v_arr[v3->airport_id] = v3;
    vertex *v4 = new vertex(4, 4, 4); 
    g->v_arr[v4->airport_id] = v4;
    vertex *v5 = new vertex(5, 5, 5); 
    g->v_arr[v5->airport_id] = v5;
    vertex *v6 = new vertex(6, 6, 6); 
    g->v_arr[v6->airport_id] = v6;
    

    g->insert_edge(1, 2, 9);

    g->insert_edge(1, 3, 4);
    g->insert_edge(3, 2, 4);

    g->insert_edge(2, 4, 12);

    g->insert_edge(2, 5, 5);
    g->insert_edge(3, 5, 13);

    g->insert_edge(5, 4, 3);
    g->insert_edge(4, 6, 2);

    g->insert_edge(5, 6, 15); 

    // if a path doesn't exist between 2 pairs, nothing is added
    REQUIRE(g->Betweeness(1) == 0); 
    // BC (0) + BD (0/1) + BE (0/1) + BF (0/1) + CB (0/1) + CD (0/1) + CE (0/1) + CF (0/1) + 
    // DB (0) + DC (0) + DE (0) + DF (0/1) + EB (0) + EC (0) + ED (0/1) + EF (0/1) + 
    // FB (0) + FC (0) + FD (0) + FE (0) = 0
    REQUIRE(g->Betweeness(2) == 6); 
    // AC (0/1) + AD (1/1) + AE (1/1) + AF (1/1) + CA (0) + CD (1/1) + CE (1/1) + CF (1/1) + 
    // DA (0) + DC (0) + DE () + DF (0/1) + EA (0) + EC (0) + ED (0/1) + EF (0/1) + 
    // FB (0) + FC (0) + FD (0) + FE (0) = 6
    REQUIRE(g->Betweeness(3) == 4); 
    // AB (1/1) + AD (1/1) + AE (1/1) + AF (1/1) + BA (0) + BD (0/1) + BE (0/1) + BF (0/1) + 
    // DA (0/1) + DB (0/1) + DE (0/1) + DF (0/1) + EA (0) + EB (0) + ED (0/1) + EF (0/1) + 
    // FA (0) + FB (0) + FD (0) + FE (0) = 4
    REQUIRE(g->Betweeness(4) == 4); 
    // AB (0/1) + AC (1/1) + AE (0/1) + AF (1/1) + BA (0) + BC (0) + BE (0/1) + BF (1/1) + 
    // CA (0) + CB (0/1) + CE (0/1) + CF (1/1) + EA (0) + EB (0) + EC (0) + EF (1/1) + 
    // FA (0) + FB (0) + FC (0) + FE (0) = 4
    REQUIRE(g->Betweeness(5) == 6); 
    // AB (0/1) + AC (0/1) + AD (1/1) + AF (1/1) + BA (0) + BC (0) + BD (1/1) + BF (1/1) + 
    // CA (0) + CB (0/1) + CD (1/1) + CF (1/1) + DA (0) + DB (0) + DC (0) + DF (0/1) + 
    // FA (0) + FB (0) + FD (0) + FE (0) = 6
    REQUIRE(g->Betweeness(6) == 0); 
    // AB (0/1) + AC (0/1) + AD (0/1) + AE (1/1) + BA (0) + BC (0) + BD (0/1) + BE (0/1) + 
    // CA (0) + CB (0/1) + CD (0/1) + CF (0/1) + DA (0) + DB (0) + DC (0) + DE (0/1) + 
    // EA (0) + EB (0) + EC (0) + ED (0/1) = 0
}

