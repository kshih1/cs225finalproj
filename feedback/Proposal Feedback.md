### Grade
Partly Competent (80%)

### Comments

**Data** 
You have not mentioned what will the weights between the graph nodes. If the nodes are airports, will the weights be the distance between airports. If so be careful to use Spherical distances to calculate distrances between longitudes and latitudes.

**Algorithms**
You said you'll be using Floyd-Warshall Algorithm as one of the algorithms. Is there any reason you can not use Dijkstra's instead? Floyd-Warshall Algorithm is specifically useful when you have negative weights and no net-negative cycles. However if your weights are distances, they can't be negative and hence Floyd-Warshall Algorithm is overkill and Dijkstra's may suffice.
