import java.text.DecimalFormat;
import java.util.*;


public class Graph{
    private int numCities;
    private int numRoutes;
    private HashMap<String, Edge> adjList;
    private int id = 0;
    private String[] cities;

    public Graph(int cities, int routes) {
        adjList = new HashMap<>();
        this.numCities = cities;
        this.numRoutes = routes;
        this.cities = new String[numCities];
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PART 1////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * This method adds the passed in route from city1 to city2 (and vice versa) to the adjacency list.
     * @param city1 first city being travelled to/from
     * @param city2 second city being travelled to/from
     * @param weight cost of travel between city1 and city2
     */
    public void addRoute(String city1, String city2, double weight) {
        //Adds new edge to city1's adjacency list
        if (adjList.get(city1) == null) {                                   //if the city hasn't been hashed to the table at all, add it
            adjList.put(city1, new Edge (city1, city2, weight, null, id));
            cities[id] = city1;
            id++;
        }
        else if (!adjListContains(city1, city2, weight)) {                  //if the entry is already in the hash map, and the edge isn't already in the adjacency list, put the edge in.
            Edge currentEdge = adjList.get(city1);
            while (currentEdge.next != null) {                              //we know currentEdge will never start out as null because of the previous if statement
                currentEdge = currentEdge.next;
            }
            currentEdge.next = new Edge(city1, city2, weight, null, currentEdge.id);                                    //appends the new Edge to the adjacency list
        }

        //adds new edge to city2's adjacency list
        if (adjList.get(city2) == null) {                                   //if the city hasn't been hashed to the table at all, add it
            adjList.put(city2, new Edge(city2, city1, weight, null, id));
            cities[id] = city2;
            id++;
        }
        else if (!adjListContains(city2, city1, weight)) {                  //if the entry is already in the hash map, and the edge isn't already in the adjacency list, put the edge in.
            Edge currentEdge = adjList.get(city2);
            while (currentEdge.next != null) {                              //we know currentEdge will never start out as null because of the previous if statement
                currentEdge = currentEdge.next;
            }
            currentEdge.next = new Edge(city2, city1, weight, null, currentEdge.id);                                    //appends the new Edge to the adjacency list
        }
    } //addRoute()

    /**
     * This method iteratively uses DFS and finds connected components and separation edges
     * @return a string in the format of connected components, number of separation edges, and separation edges in alphabetical order
     */
    public String analyzeGraph() {
        int numComponents = 0;
        int numSeparation = 0;
        ArrayList<String> separationEdges = new ArrayList<>();
        Stack<String> stack = new Stack<>();                            //stack
        boolean[] visited = new boolean[numCities];                     //whether or not the vertex has been visited or not. Helps with finding number of connected components
        boolean[] end = new boolean[numCities];                         //whether or not the vertex is a leaf
        int[] disc = new int[numCities];                                //discovery time of a vertex
        int[] eval = new int[numCities];                                //minimum of disc[v], min(v child's eval function), min(disc[w], where w is a back edge from v)
        int[] prev = new int[numCities];                                //at the current vertex's id in the array, another id will be present, indicating the vertex it came from. This will also represent the TREE EDGE of a vertex

        //Initialize each array
        for (int i = 0; i < numCities; i++) {
            visited[i] = false;
            disc[i] = -1;
            eval[i] = -1;
            prev[i] = -1;
            end[i] = false;
        }

        ////////////////////////DFS
        for (String key : adjList.keySet()) {
            int discIndex = 1;
            if (!visited[adjList.get(key).id]) {                        //for every city in the adjacency list, do DFS if the vertex hasn't already been visited
                numComponents++;                                        //if we find a vertex that hasn't been visited, there is at least one more connected component
                stack.push(key);
                while (!stack.isEmpty()) {
                    String parent = stack.pop();
                    int parentID = adjList.get(parent).id;
                    if (visited[parentID]) {                            //if the current vertex has already been visited, we've already dealt with it, so skip it
                        continue;
                    }

                    Edge currentEdge = adjList.get(parent);
                    int currentID;

                    disc[parentID] = discIndex++;                       //when we pop a vertex, that hasn't been visited, updated the discovery time
                    eval[parentID] = disc[parentID];                    //no vertex will get to this point having an eval value that's not -1, and one of the 3 parameters in the eval function is discovery time, so update the eval value to its discovery time
                    visited[parentID] = true;                           //mark as visited so we don't overwrite any of the data

                    while(currentEdge != null) {                        //iterate through current vertex's adjacency list
                        currentID = adjList.get(currentEdge.city2).id;
                        end[currentID] = true;                          //start by marking the vertex as an end (leaf) vertex, since we don't know if it has any children at this point
                        if (!visited[currentID]) {                      //if the current edge that's adjacent hasn't been visited, push it onto the stack since we need to work with it later
                            stack.push(currentEdge.city2);
                            prev[currentID] = parentID;                 //if it hasn't been visited already, it's not a back edge, and is in need of a parent
                            end[currentID] = false;
                        }
                        else if (prev[parentID] != currentID) {         //back edge
                            if (disc[currentID] < eval[parentID]) {     //if this is true, the parent's eval value must be updated for us to determine separation edges in the future
                                eval[parentID] = disc[currentID];
                            }
                        }
                        currentEdge = currentEdge.next;
                    }
                    visited[parentID] = true;
                }
            }
        }

        //updates the evaluation value for each vertex
        for (int i = 0; i < end.length; i++) {
            if (!end[i]) {                                              //if it's not an leaf vertex (meaning it has children), it is eligible to be updated
                int vertex = i;                                         //one of the possible values for eval is the minimum of the eval function for any of its children, so we have to check this
                while(prev[vertex] != -1) {
                    if (eval[vertex] < eval[prev[vertex]]) {
                        eval[prev[vertex]] = eval[vertex];
                    }
                    vertex = prev[vertex];
                }
            }
        }
        ////////////////////////DFS

        for (int i = 0; i < numCities; i++) {
            System.out.println(eval[i]);
        }

        //adds all cities to the separation edge list - makes sure each edge is in alphabetical order
        for (int i = 0; i < numCities; i++) {
            if (prev[i] != -1 && disc[prev[i]] < eval[i]) {     //this is how you determine if it's a separation edge
                numSeparation++;
                if (cities[prev[i]].compareTo(cities[i]) > 0) {
                    separationEdges.add(cities[i] + " " + cities[prev[i]]);
                }
                else {
                    separationEdges.add(cities[prev[i]] + " " + cities[i]);
                }
            }
        }

        Collections.sort(separationEdges);
        StringBuilder sepEdgeString = new StringBuilder();
        for (int i = 0; i < separationEdges.size(); i++) {
            if (i == separationEdges.size() - 1) {
                sepEdgeString.append(separationEdges.get(i));
            }
            else {
                sepEdgeString.append(separationEdges.get(i) + "\n");
            }
        }
        String returnString = numComponents + "\n" + numSeparation;
        if (numSeparation > 0) {
            returnString += "\n" + sepEdgeString;
        }
        return returnString;
    } //analyzeGraph()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PART 2////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * Uses Dijkstra's algorithm to determine the shortest way to get from source to destination.
     * @param source Starting vertex
     * @param destination Vertex that you want to end up at
     * @return A string representation of the path taken to get from source to destination, followed by the total weight.
     */
    public String findCost(String source, String destination) {
        PriorityQueue<Vertex> priorityQueue = new PriorityQueue<Vertex>(numCities, new Vertex());
        double[] distFromSrc = new double[numCities];                   //array of the distances from the source for every vertex in the graph
        String[] prevVertex = new String[numCities];                    //array of previous vertices of the current vertices as we find the shortest path from source to destination
        int srcID = adjList.get(source).id;
        int dstID = adjList.get(destination).id;

        //initializes distToVertices and prevVertex accordingly
        for (int i = 0; i < numCities; i++) {
            distFromSrc[i] = Double.POSITIVE_INFINITY;               //we initialize every vertex's distance from start to infinity because we don't know if there's a valid path yet
            prevVertex[i] = null;                                    //no previous vertices have been established yet
        }
        distFromSrc[srcID] = 0.0;

        //adds each vertex, with its distance to the start, to the priority queue for easy extraction of the current minimum path
        int id;
        for (String key : adjList.keySet()) {
            id = adjList.get(key).id;
            priorityQueue.add(new Vertex(key, distFromSrc[id], id));
        }

        Vertex currentVertex;
        while (priorityQueue.size() != 0) {
            currentVertex = priorityQueue.poll();                                                   //currentVertex is the city with the smallest distance from the start vertex in the priority queue
            Edge adjToVertex = adjList.get(currentVertex.currentCity);                              //Linked List of Edges, which are all adjacent vertices to currentVertex
            double newDist = 0.0;
            double prevDist;
            int newID;

            while (adjToVertex != null) {                                                           //Traverses the adjacency list for currentVertex
                newID = adjList.get(adjToVertex.city2).id;
                prevDist = distFromSrc[currentVertex.id];
                    newDist = prevDist + adjToVertex.weight;                                        //calculates the new distance: current distance from start plus the distance between the current vertex and new vertex (adjToVertex.weight is the distance between the two)
                    if (newDist < distFromSrc[newID]) {                                             //if the new distance calculated is less than the previous known way to get to that new vertex, update the distance
                        distFromSrc[newID] = newDist;                                               //(adjList.get(adjToVertex.city2).id) is the index of where the previous distance from src to the new vertex is
                        prevVertex[newID] = currentVertex.currentCity;
                        priorityQueue.remove(new Vertex(adjToVertex.city2, prevDist, newID));       //removes the vertex to update it
                        priorityQueue.add(new Vertex(adjToVertex.city2, newDist, newID));           //re-adds vertex with updated value
                    }
                adjToVertex = adjToVertex.next;                                                     //goes to the next vertex in the adjacency list
            }
        }
        return printDijkstraPath(prevVertex, destination, distFromSrc[dstID]);
    } //findCost()

    /**
     * This method traverses through prevVertex[] to find the path taken using dijkstra's algorithm from source to destination
     * @param prevVertex array of vertices that keep track of the Vertex that came directly before it in dijkstra's path
     * @param destination the key of the Vertex to end at
     * @param finalWeight the weight of Dijkstra's path
     * @return A string representation of the path taken (source ... intermediateVertices ... destination), and the total weight of the path at the end.
     */
    public String printDijkstraPath(String[] prevVertex, String destination, double finalWeight) {
        if (finalWeight == Double.POSITIVE_INFINITY) {              //if the final weight is positive infinity, that means it's impossible to get to the destination from the source
            return "not possible";
        }

        int vertexID = adjList.get(destination).id;                 //this is the index (id) that we'll use to traverse the array. Starts at destination vertex.
        StringBuilder sb = new StringBuilder();

        sb.insert(0, destination + " ");
        String currentVertex = prevVertex[vertexID];
        while (currentVertex != null) {
            sb.insert(0, currentVertex + " ");            //prepends every new vertex found in the path
            vertexID = adjList.get(currentVertex).id;               //updates the vertexID to be the id of the vertex currently being looked at (the previous vertex)
            currentVertex = prevVertex[vertexID];
        }

        DecimalFormat df = new DecimalFormat("0.00");       //Makes sure weight value truncates to 2 decimal places
        sb.append(df.format(finalWeight));
        return sb.toString();
    } //dijkstraPath()


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PART 3////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * This method finds a Eulerian Tour of the MST of the graph represented by adjList.
     * @param root the root of the MST
     */
    public String eulerianTour(String root) {
        ArrayList<Edge> MST = kruskalMST();
        if (MST == null || MST.size() < numCities - 1) {                  //if the MST is null, there is no possible way to visit all vertices, so return "not possible"
            return "not possible";
        }
        return findEulerianTour(MST, root, null);
    } //eulerianTour()

    /**
     * This method recursively traverses the MST using a eulerian tour.
     * @param MST minimum spanning tree to traverse
     * @param root root of the MST
     * @return a String of the traversal of the MST using a eulerian tour
     */
    public String findEulerianTour(ArrayList<Edge> MST, String root, String parent) {
        String tour = root;
        PriorityQueue<String> children = new PriorityQueue<>();
        //Add all children to a priority queue
        for (Edge e : MST) {
            if ((e.city1.equals(root) && !e.city2.equals(parent)) || (!e.city1.equals(parent) && e.city2.equals(root))) {
                if (!e.city1.equals(root)) {
                    children.add(e.city1);
                }
                else {
                    children.add(e.city2);
                }
            }
        }
        while (!children.isEmpty()) {
            String currentChild = children.poll();                      //take lowest child (alphabetically)
            tour += "\n" + findEulerianTour(MST, currentChild, root);
        }
        return tour;
    }
    /**
     * This method uses Kruskal's algorithm to find a MST that spans the graph represented by adjList
     * @return the MST of the graph represented by adjList
     */
    public ArrayList<Edge> kruskalMST() {
        PriorityQueue<Edge> priorityQueue = new PriorityQueue<Edge>(numCities, new Edge());       //this priority queue keeps track of the shortest edge so that we can add them one at a time
        ArrayList<Edge> MST = new ArrayList<>();               //store the resulting MST here

        //ADD EVERY EDGE TO THE PRIORITY QUEUE
        for (String key : adjList.keySet()) {
            Edge currentEdge = adjList.get(key);
            while (currentEdge!= null) {
                if (!priorityQueue.contains(currentEdge) && !priorityQueue.contains(new Edge(currentEdge.city2, currentEdge.city1, currentEdge.weight, currentEdge.next, currentEdge.id))) {    //makes sure the currentEdge and its reciprocal are NOT in the priority queue already
                    priorityQueue.add(currentEdge);
                }
                currentEdge = currentEdge.next;
            }
        }

        //MAKE V SETS TO START
        String[] set = new String[numCities];
        for (String vertex : adjList.keySet()) {
            set[adjList.get(vertex).id] = vertex;
        }

        int numConnected = 0;
        Edge currentEdge;
        while (numConnected < numCities - 1) {
            if (priorityQueue.isEmpty()) {                  //if the priority queue is ever empty and we are still haven't connected every city, it's not possible to have a MST, return null
                return null;
            }
            currentEdge = priorityQueue.poll();             //currentEdge with be the current minimum
            String set1 = findParent(set, currentEdge.city1);
            String set2 = findParent(set, currentEdge.city2);
            if (!set1.equals(set2)) {                       //if parent1 and parent2 aren't equal, the 2 cities aren't in the same set and can be connected
                MST.add(currentEdge);                       //since adding this currentEdge won't make a cycle, add it to the MST
                joinSets(set, currentEdge.city1, currentEdge.city2);                  //since city1 and city2 are now connected, they're in the same set
                numConnected++;
            }
        }
        return MST;
    } //kruskalMST()

    public void printSet(String[] set) {
        for (int i = 0; i < set.length; i++) {
            System.out.print("[" + set[i] + "]");
            if (i == set.length - 1) {
                System.out.println();
            }
        }
    } //printSet()

    /**
     * This method prints the MST passed into argument 1.
     * @param MST minimum spanning tree to print
     */
    public void printMST(ArrayList<Edge> MST) {
        System.out.println("=====MINIMUM SPANNING TREE=====");
        for (Edge e : MST) {
            System.out.println(e.city1 + " " + e.city2 + " " + e.weight);
        }
        System.out.println("===============================");
    } //printMST()

    /**
     * Finds the parent of 'city' in its set.
     * @param set the set to recursively search
     * @param city find the parent of this city
     * @return the name of the vertex that is the parent of 'city'
     */
    public String findParent(String[] set, String city) {
        if (!set[adjList.get(city).id].equals(city)) {
            return findParent(set, set[adjList.get(city).id]);
        }
        return city;
    } //findParent()

    /**
     * This method joins the 2 sets that contain cities city1 and city2
     * @param set the set array to search
     * @param city1 the first city to join
     * @param city2 the second city to join
     */
    public void joinSets(String[] set, String city1, String city2) {
        String city1Parent = findParent(set, city1);
        String city2Parent = findParent(set, city2);
        set[adjList.get(city2Parent).id] = city1Parent;
    } //joinSets()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//OTHER STUFF///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * This method prints the global hash map to the console.
     */
    public void printHashMap() {
        Edge currentEdge;
        for (String p : adjList.keySet()) {
            currentEdge = adjList.get(p);
            System.out.println("====================" + currentEdge.city1 + "====================");
            while (currentEdge != null) {
                System.out.println("Adjacent City = " + currentEdge.city2);
                System.out.println("Weight = " + currentEdge.weight);
                System.out.println("---------------------");
                currentEdge = currentEdge.next;
            }
            System.out.println("===========================================");
        }
    } //printHashMap()

    /**
     * This method traverses through the adjacency list of city1 to determine if the edge is already in the adjacency list.
     * @param city1 the city whose adjacency list we will traverse through
     * @param city2 the city that city1 connects to on the edge
     * @param weight the weight of the edge between city1 and city2
     * @return a boolean value that tells you whether or not the edge from city1 to city2 is already in the adjacency list
     */
    public boolean adjListContains(String city1, String city2, double weight) {
        Edge currentEdge = adjList.get(city1);
        while (currentEdge != null) {
            if (currentEdge.city1.equals(city1) && currentEdge.city2.equals(city2) && currentEdge.weight == weight) {
                return true;
            }
            currentEdge = currentEdge.next;
        }
        return false;
    } //adjListContains()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIORITY QUEUE STUFF//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * PRIORITY QUEUE STUFF FOR DIJKSTRA
     * This is to keep track of each vertex in the graph and the weight of the path from the start.
     */
    class Vertex implements Comparator<Vertex> {
        public String currentCity;
        public double weightToStart;
        public int id;

        public Vertex() {}

        public Vertex (String currentCity, double weightToStart, int id) {
            this.currentCity = currentCity;
            this.weightToStart = weightToStart;
            this.id = id;
        } //Vertex Constructor

        /**
         * This method compares v1's weight to v2's weight, and returns the difference with respect to v1.
         * @param v1 first Vertex to compare
         * @param v2 second Vertex to compare
         * @return 1 if v1 > v2, -1 if v1 < v2, 0 if equal
         */
        public int compare(Vertex v1, Vertex v2) {
            if (v1.weightToStart > v2.weightToStart) {
                return 1;
            }
            else if (v1.weightToStart < v2.weightToStart) {
                return -1;
            }
            else {
                return 0;
            }
        } //compare()

        /**
         * This method first makes sure neither Vertex is null, then checks to see whether the ids are the same or not
         * @param v1 first Vertex to compare
         * @param v2 second Vertex to compare
         * @return true if the ids are the same, false if not
         */
        public boolean equals(Vertex v1, Vertex v2) {
            return (v1 != null && v2 != null) && v1.id == v2.id;
        } //equals()
    } //Vertex Class

    /**
     * PRIORITY QUEUE STUFF FOR KRUSKAL
     * This is to keep track of each edge in the graph, the vertices included, and the weight.
     */
    class Edge implements Comparator<Edge> {

        public String city1;
        public String city2;
        public double weight;
        public Edge next;
        public int id;                  //id is used to eliminate array access and loops - just use the id as the index

        public Edge() {}

        public Edge(String city1, String city2, double weight, Edge next, int id) {
            this.city1 = city1;
            this.city2 = city2;
            this.weight = weight;
            this.next = next;
            this.id = id;
        } //Edge Constructor

        /**
         * This method compares e1's weight to e2's weight, and returns the difference with respect to e1.
         * @param e1 first Edge to compare
         * @param e2 second Edge to compare
         * @return 1 if e1 > e2, -1 if e1 < e2, 0 if equal
         */
        public int compare(Edge e1, Edge e2) {
            if (e1.weight > e2.weight) {
                return 1;
            }
            else if (e1.weight < e2.weight) {
                return -1;
            }
            else {
                return 0;
            }
        } //compare()

        /**
         * This method compares each Edge's cities to see if they're equal.
         * @param e1 first Edge to compare
         * @param e2 second Edge to compare
         * @return true if any combination of the cities match up
         */
        public boolean equals(Edge e1, Edge e2) {
            if (e1 == null || e2 == null) {
                return false;
            }
            else if (e1.city1.equals(e2.city1) && e1.city2.equals(e2.city2)) {
                return true;
            }
            else if (e1.city1.equals(e2.city2) && e1.city2.equals(e2.city1)) {
                return true;
            }
            return false;
        } //equals()
    } //Edge Class
} //Graph Class
