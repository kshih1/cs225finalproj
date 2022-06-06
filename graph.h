#pragma once
#include <vector>
#include <map>
#include<string>

#include "Airlines.h"

class graph{
public:

    map<int, vertex*> v_arr; // given a valid open flight airport id, maps a pointer to a vertex reprsenting that airport

    //see graph.cpp for comments explaining the details explaining each of the functions
     ~graph();
     graph(); 
    void insert_vertex(vertex *v);
    void insert_edge(int source, int destination, int weight);
    void read_airports(string filename);
    void read_routes(string filename);
    void print_v_arr();
    void BFS(int portID);
    map<int, vector<vector<int>>> Dijkstras1(int source_id); 
    double Betweeness(int source_id); 
    void graphCleaning(); 
};
