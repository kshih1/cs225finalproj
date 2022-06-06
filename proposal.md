## Final Project Proposal


**1. Leading Question:**
        Given a dataset of flight routes and airports, what is the shortest route one could take a plane from Airport A to Airport B? In this project, our goal is to utilize 
    the dataset to create a search engine that essentially allows the user to input a source and destination airport and generate shortest aircraft routes This will be done
    using algorithms such as Breadth First Traversal which finds all the paths from a starting node to all other nodes in our graph, Dijsktra's Algorithm which calculates 
    the shortest path from one node to any other node and the Betweenness centrality which measures the centrality in a given graph based on shortest paths and 
    allows us to realize which airports are most connected to shortest paths, or in other words: the most efficient to travel from. 

**2. Dataset acquisition and processing:**
	 The dataset we wish to use is from the OpenFlights database, a database that contains lots of open source flight data up until 2017. As we are intending to implement a 
    shortest route algorithm, we will be using the routes.dat subset from the OpenFlights database. In addition to this, we need to be able to find the distance between two 
    different airports. For this, we need to also use the airports.dat dataset from OpenFlights. All datasets from OpenFlights are comma-separated values with varying parts 
    depending on the dataset. routes.dat contains 67663 routes between 3321 airports on 548 airlines, and each entry is formatted with 9 different portions: The airline code, 
    a unique OpenFlights airline identifier, the source airport code, a unique OpenFlights airport identifier for the source airport, the destination airport code, a unique 
    OpenFlights airport identifier for the destination airport, a codeshare identifier (null if not assigned), the number of stops on the flight, and the types of planes used 
    on the flight. For the airports.dat dataset, each entry has 14 portions: A unique OpenFlights identifier, the name of the airport, the city name, the country where the 
    airport is located, the 3 letter IATA code (null if not assigned), the 4 letter ICAO code (null if not assigned), the latitude, the longitude, the altitude of the airport, 
    the time zone (in hours offset from UTC), the daylight savings based on the country of location, the timezone in tz (Olson) format, the type of the station (for our dataset 
    the type will always be airport), and the source of the data.
    <br/>
    <br/>
    	When parsing the input data, we wish to correct it in a few ways. For starters, both datasets have a lot of parts to each entry that we do not always need and storing
    those items would take up a lot of time when searching through the data. For airports, we only wish to keep the OpenFlights identifier airport code, the longitude, 
    and the latitude. For the routes dataset, when storing each entry, we would only store the source airport code and the destination airport code. This way we are only storing
    the minimum amount of data we need, and discarding the rest to minimize storage. In addition, if there is a "\N" or missing information in any of the data we are storing 
    in airports or routes, we would not add the respective airport or route in our data completely. After parsing through both datasets, we would also remove any unused 
    airports that are not utilitized in any routes, to minimize the memory needed to store our data.  
    <br/>
    <br/>
    We will store our airport data in a airport map,  using the airport code as the key to make it more accessible to access information like the longitude and latitude 
    and adjacent airports, which would also be a map using the airport code of adjacent airports as the key. However, information stored inside this map would be things like
    the adjacent airport id and the weight of the edge, the spherical distance between two airports which we calculate. 
    As we will be checking every index when parsing and then keeping the ones we wish and deleting the ones we do not need, the storage time would have a worst case scenario
    of O(n * v) time, n being the number of airports and v being the number of routes. 

**3. Graph algorithms:**
    BFS: We will use our BFS algorithm to find all the paths from a starting node to all other nodes in our graph. In our case, each node in the graph represents an airport 
    and each edge represents a route from one airport to another. Here, we should be able to display all the nodes that follow the edges in a BFS manner. When we analyze the 
    time complexity of this traversal, it would be O(N + E) where N represents the number of nodes and E represents the number of edges. 
	<br/>
	<br/>
    Dijkstra's Algorithm to compute shortest path: 
    This algorithm calculates the shortest path from one node to any other node, with weights calculated as the spherical distance between 2 airports. 
    This function takes a graph reference and the output should be a shortest path from a given source airport to a destination airport. 
    When we analyze the time complexity of the Dijkstra's, it would be O(N^2 * logN).  
	<br/>
    Betweenness centrality to visualize centrality in the graph:
    This algorithm measures the centrality in a given graph based on shortest paths. Essentially for any pair of nodes in a connected graph there is at least one shortest path
    from using Dijkstra's. The betweenness centrality for a target node or airport would be determined by summing for all pairs of nodes, except for pairs including the target
    as a source or destination, the ratio of shortest paths between the pair including the target node divided by the ratio of shortest paths between the pair. 
    The function input would be a connected graph reference and the output should also be a map of airports as keys, storing each airport's numerical centrality. When we 
    analyze the time complexity of of this algorithm, it would be O(N^3 * logN) to calculate the centrality of any one node, given that we are also using Dijsktra's to aid in 
    finding shortest paths but we would need to repeat Dijstkra's for nodes. 

**4. Timeline for execution of above-mentioned tasks:**
    <br/>
    Data Acquisition: April 1 
    <br/>
    Data Processing: April 8
    <br/>
    Completion of the BFS Algorithm: by Mid-Project check-in deadline, Apr 15
    <br/>
    Completion of the Dijkstra's Algorithm: April 21
    <br/>
    Completion of Betweenness Centrality Algorithm: April 29
    <br/>
    Completion of final deliverables (code base, README, report, presentation): May 3
