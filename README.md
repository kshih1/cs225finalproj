## CS225 SP22 Final Project: OpenFlights Airports & Routes
**Group Members:** Anushka Pachuary (ap39), Rithvik Kopparapu (rithvik9), Kelly Shih (kshih7)

## Introduction
Given a dataset of flight routes and airports, what is the shortest route one could take a plane from Airport A to Airport B? In this project, our goal is to utilize 
the dataset to create a search engine that essentially allows the user to input a source and destination airport and generate shortest aircraft routes This will be done 
using algorithms such as Breadth First Traversal which finds all the paths from a starting node to all other nodes in our graph, Dijsktra's Algorithm which calculates the shortest path from one node to any other node and the Betweenness centrality which measures the centrality in a given graph based on shortest paths and allows us to realize which airports are most connected to shortest paths, or in other words: the most efficient to travel from.

## GitHub Organization 
All important code/files are in the main branch unless otherwise specified 
* Report (see Written report final project .pdf)
* Presentation Video (see presentation_video.txt or https://youtu.be/APS_AhyvIww)
* Input Files: 
  * full dataset: airports.txt, routes.txt
  * subsets of dataset: routes_subset.txt, limited_routes.txt (advised for running betweeness centrality due to the long run times on full dataset) 
* Tests: all test cases in tests.cpp and sample test input files are stored in the tests folder
  * additional files used to run CATCH in test cases are borrowed from previous CS225 labs/mps, and are stored in the cs225 folder
* Algorithms & Data Parsing: the bulk of the parsing and algorithm code is done in graphs.cpp, graphs.h, with helper classes & functions
  in Airlines.cpp, Airlines.h
* Main: main.cpp parses the input airport and route files to create a graph, then runs the desired algorithms on the graph and return their meaningful data.
* Output files: 
  * BFS.txt: saves the number of edges of all connected vertices from a given source airport (of most recent running BFS)
  * dijkstra.txt: saves ALL paths from a given source airport (of the most recent running Dijkstra's & choosing to print ALL shortest paths)
  * betweenness.txt: saves all betweeness centrality calculations ran on the most recent time the project was run

## Running the Project
To run the project at scale, please type:
```
make 
./final_proj
```

From then on, you can simply follow along the instructions in the command line to choose the appropriate input files for airports and routes to
construct a weighted directed graph, and execute the rest of the final project. You will be then be given the choice to type in 
appropriate numbers/letters(case sensitive) inputs to execute various functions or to exit. Individual functions may have additional user 
inputs (ex: selecting a source OpenFlights airport id) and may also print out meaningful data to the terminal/save data to an output file after 
its completion. 

## Running the Tests
To run our tests, please type:
```
make test
./test
```

* Parsing & Data proccessing: we have 3 simple test cases which ensure the airports and routes are read correctly and that the data is cleaned, to ensure that when
  getting data from the full dataset, the data is being read correctly. 
* BFS: we have simple test cases involving various types of graphs such a a simple path, a circular cyclic path, a tree, and a K4 dense graph among others. In all of these cases, we would test that only destination nodes that are connected to the source node are set to be visited and that the edges of the graph are correctly set to be undiscovered, discovery edges, and cross edges. We also made sure that the expected distance from the source node from destination node matched as expected.
* Dijkstra's: To test our implementation of Dijkstra’s Algorithm, we created a couple simple tests to make sure that all appropriate shortest paths were saved, including one with that made sure that if there are multiple shortest paths from a specific source and destination node, that both paths would be correctly saved. In addition, if a source and destination node were not connected at all by a path we also created a test case which made sure to test the the mapped vector of paths would have a size of 0 for the destination node. 
* Betweeness Centrality: Using the same graphs we used in the test cases for Dijkstra's we also made sure that we were able to correctly calculate the betweeness centrality for each node in the graph correctly. For the graph that involved nodes in which the source node may not necessarily have a path to all potential destination nodes, we made sure that if the path between nodes did not exist, we would simply not use it in our centrality calculation (so there is no divide by 0 error). 
