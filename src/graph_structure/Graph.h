// Original code by Gonçalo Leão
// Updated by DA 2024/2025 Team

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "./MutablePriorityQueue.h" // not needed for now

template<class T>
class Edge;

#define INF std::numeric_limits<double>::max()

/**
 * @class Vertex
 * @brief Represents a vertex in a graph data structure
 * 
 * This class contains vertex information, outgoing and incoming edges,
 * and various properties used by graph algorithms.
 * 
 * @tparam T The type of information stored in the vertex
 */
template<class T>
class Vertex {
public:
    /**
     * @brief Constructs a vertex with the given information
     * @param in The information to store in the vertex
     * @complexity O(1)
     */
    Vertex(T in);

    /**
     * @brief Comparison operator for priority queue operations
     * @param vertex The vertex to compare with
     * @return True if this vertex has lower priority than the given vertex
     * @complexity O(1)
     */
    bool operator<(Vertex<T> &vertex) const; // // required by MutablePriorityQueue

    /**
     * @brief Gets the information stored in the vertex
     * @return The vertex information
     * @complexity O(1)
     */
    T getInfo() const;

    /**
     * @brief Gets all outgoing edges from this vertex
     * @return Vector of pointers to outgoing edges
     * @complexity O(1)
     */
    std::vector<Edge<T> *> getAdj() const;

    /**
     * @brief Checks if the vertex has been visited in a traversal
     * @return True if the vertex has been visited
     * @complexity O(1)
     */
    bool isVisited() const;

    /**
     * @brief Checks if the vertex is currently being processed
     * @return True if the vertex is being processed
     * @complexity O(1)
     */
    bool isProcessing() const;

    /**
     * @brief Gets the number of incoming edges to this vertex
     * @return The indegree count
     * @complexity O(1)
     */
    unsigned int getIndegree() const;

    /**
     * @brief Gets the current distance value (used in shortest path algorithms)
     * @return The distance value
     * @complexity O(1)
     */
    double getDist() const;

    /**
     * @brief Gets the edge that leads to this vertex in a path
     * @return Pointer to the path edge
     * @complexity O(1)
     */
    Edge<T> *getPath() const;

    /**
     * @brief Gets all incoming edges to this vertex
     * @return Vector of pointers to incoming edges
     * @complexity O(1)
     */
    std::vector<Edge<T> *> getIncoming() const;

    /**
     * @brief Sets the information stored in the vertex
     * @param info The new information
     * @complexity O(1)
     */
    void setInfo(T info);

    /**
     * @brief Sets the visited status of the vertex
     * @param visited The new visited status
     * @complexity O(1)
     */
    void setVisited(bool visited);

    /**
     * @brief Sets the processing status of the vertex
     * @param processing The new processing status
     * @complexity O(1)
     */
    void setProcessing(bool processing);

    /**
     * @brief Gets the low value (used in strongly connected components algorithms)
     * @return The low value
     * @complexity O(1)
     */
    int getLow() const;

    /**
     * @brief Sets the low value
     * @param value The new low value
     * @complexity O(1)
     */
    void setLow(int value);

    /**
     * @brief Gets the num value (used in strongly connected components algorithms)
     * @return The num value
     * @complexity O(1)
     */
    int getNum() const;

    /**
     * @brief Sets the num value
     * @param value The new num value
     * @complexity O(1)
     */
    void setNum(int value);

    /**
     * @brief Sets the indegree count
     * @param indegree The new indegree count
     * @complexity O(1)
     */
    void setIndegree(unsigned int indegree);

    /**
     * @brief Sets the distance value
     * @param dist The new distance value
     * @complexity O(1)
     */
    void setDist(double dist);

    /**
     * @brief Sets the path edge
     * @param path The new path edge
     * @complexity O(1)
     */
    void setPath(Edge<T> *path);

    /**
     * @brief Adds an outgoing edge to another vertex
     * @param dest The destination vertex
     * @param w The edge weight
     * @return Pointer to the newly created edge
     * @complexity O(1)
     */
    Edge<T> *addEdge(Vertex<T> *dest, double w);

    /**
     * @brief Removes an outgoing edge to a vertex with the given information
     * @param in The information of the destination vertex
     * @return True if the edge was successfully removed
     * @complexity O(E) where E is the number of outgoing edges
     */
    bool removeEdge(T in);

    /**
     * @brief Removes all outgoing edges from this vertex
     * @complexity O(E) where E is the number of outgoing edges
     */
    void removeOutgoingEdges();

    friend class MutablePriorityQueue<Vertex>;

protected:
    T info; // info node
    std::vector<Edge<T> *> adj; // outgoing edges

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    bool processing = false; // used by isDAG (in addition to the visited attribute)
    int low = -1, num = -1; // used by SCC Tarjan
    unsigned int indegree; // used by topsort
    double dist = 0;
    Edge<T> *path = nullptr;

    std::vector<Edge<T> *> incoming; // incoming edges

    int queueIndex = 0; // required by MutablePriorityQueue and UFDS

    /**
     * @brief Deletes an edge
     * @param edge The edge to delete
     * @complexity O(1)
     */
    void deleteEdge(Edge<T> *edge);
};

/**
 * @class Edge
 * @brief Represents an edge in a graph
 * 
 * This class connects two vertices and has properties like weight, 
 * type (driving/walking), and flow values.
 * 
 * @tparam T The type of information stored in the connected vertices
 */
template<class T>
class Edge {
public:
    /**
     * @brief Enumeration of edge types
     */
    enum class EdgeType {
        DEFAULT,   /**< Default edge type */
        DRIVING,   /**< Edge representing a driving connection */
        WALKING    /**< Edge representing a walking connection */
    };

    /**
     * @brief Constructs an edge between two vertices
     * @param orig The origin vertex
     * @param dest The destination vertex
     * @param w The edge weight
     * @complexity O(1)
     */
    Edge(Vertex<T> *orig, Vertex<T> *dest, double w);

    /**
     * @brief Virtual destructor for proper polymorphic behavior
     */
    virtual ~Edge() = default;

    /**
     * @brief Gets the destination vertex
     * @return Pointer to the destination vertex
     * @complexity O(1)
     */
    Vertex<T> *getDest() const;

    /**
     * @brief Gets the edge weight
     * @return The edge weight
     * @complexity O(1)
     */
    double getWeight() const;

    /**
     * @brief Checks if the edge is selected
     * @return True if the edge is selected
     * @complexity O(1)
     */
    bool isSelected() const;

    /**
     * @brief Gets the origin vertex
     * @return Pointer to the origin vertex
     * @complexity O(1)
     */
    Vertex<T> *getOrig() const;

    /**
     * @brief Gets the reverse edge
     * @return Pointer to the reverse edge
     * @complexity O(1)
     */
    Edge<T> *getReverse() const;

    /**
     * @brief Gets the flow value
     * @return The flow value
     * @complexity O(1)
     */
    double getFlow() const;

    /**
     * @brief Gets the edge type
     * @return The edge type
     * @complexity O(1)
     */
    EdgeType getType() const;

    /**
     * @brief Sets the selected status
     * @param selected The new selected status
     * @complexity O(1)
     */
    void setSelected(bool selected);

    /**
     * @brief Sets the reverse edge
     * @param reverse Pointer to the reverse edge
     * @complexity O(1)
     */
    void setReverse(Edge<T> *reverse);

    /**
     * @brief Sets the flow value
     * @param flow The new flow value
     * @complexity O(1)
     */
    void setFlow(double flow);

    /**
     * @brief Sets the edge type
     * @param type The new edge type
     * @complexity O(1)
     */
    void setType(EdgeType type);

    /**
     * @brief Gets the edge type as a string
     * @return String representation of the edge type
     * @complexity O(1)
     */
    virtual std::string getTypeString() const;

protected:
    Vertex<T> *dest; // destination vertex
    double weight; // edge weight, can also be used for capacity

    // auxiliary fields
    bool selected = false;
    EdgeType type = EdgeType::DEFAULT; // The edge type (driving/walking)

    // used for bidirectional edges
    Vertex<T> *orig;
    Edge<T> *reverse = nullptr;

    double flow; // for flow-related problems
};

/**
 * @class Graph
 * @brief Represents a graph data structure
 * 
 * This class implements a graph using an adjacency list representation,
 * with operations to add/remove vertices and edges.
 * 
 * @tparam T The type of information stored in vertices
 */
template<class T>
class Graph {
public:
    /**
     * @brief Destructor
     * @complexity O(V + E) where V is the number of vertices and E is the number of edges
     */
    ~Graph();

    /**
     * @brief Finds a vertex with the given information
     * @param in The information to search for
     * @return Pointer to the vertex if found, nullptr otherwise
     * @complexity O(V) where V is the number of vertices
     */
    Vertex<T> *findVertex(const T &in) const;

    /**
     * @brief Adds a vertex with the given information
     * @param in The information for the new vertex
     * @return True if successful, false if a vertex with that information already exists
     * @complexity O(V) where V is the number of vertices
     */
    bool addVertex(const T &in);

    /**
     * @brief Removes a vertex with the given information
     * @param in The information of the vertex to remove
     * @return True if successful, false if the vertex doesn't exist
     * @complexity O(V + E) where V is the number of vertices and E is the number of edges
     */
    bool removeVertex(const T &in);

    /**
     * @brief Adds an edge between two vertices
     * @param sourc The information of the source vertex
     * @param dest The information of the destination vertex
     * @param w The edge weight
     * @return True if successful, false if either vertex doesn't exist
     * @complexity O(V) where V is the number of vertices
     */
    bool addEdge(const T &sourc, const T &dest, double w);

    /**
     * @brief Removes an edge between two vertices
     * @param source The information of the source vertex
     * @param dest The information of the destination vertex
     * @return True if successful, false if the edge doesn't exist
     * @complexity O(V + E) where V is the number of vertices and E is the number of edges
     */
    bool removeEdge(const T &source, const T &dest);

    /**
     * @brief Adds a bidirectional edge between two vertices
     * @param sourc The information of the first vertex
     * @param dest The information of the second vertex
     * @param w The edge weight
     * @return True if successful, false if either vertex doesn't exist
     * @complexity O(V) where V is the number of vertices
     */
    bool addBidirectionalEdge(const T &sourc, const T &dest, double w);

    /**
     * @brief Gets the number of vertices in the graph
     * @return The number of vertices
     * @complexity O(1)
     */
    int getNumVertex() const;

    /**
     * @brief Gets all vertices in the graph
     * @return Vector of pointers to all vertices
     * @complexity O(1)
     */
    std::vector<Vertex<T> *> getVertexSet() const;

protected:
    std::vector<Vertex<T> *> vertexSet; // vertex set

    double **distMatrix = nullptr; // dist matrix for Floyd-Warshall
    int **pathMatrix = nullptr; // path matrix for Floyd-Warshall

    /**
     * @brief Finds the index of a vertex with the given information
     * @param in The information to search for
     * @return The index of the vertex if found, -1 otherwise
     * @complexity O(V) where V is the number of vertices
     */
    int findVertexIdx(const T &in) const;
};

/**
 * @brief Deletes a matrix of integers
 * @param m The matrix to delete
 * @param n The number of rows in the matrix
 * @complexity O(n)
 */
void deleteMatrix(int **m, int n);

/**
 * @brief Deletes a matrix of doubles
 * @param m The matrix to delete
 * @param n The number of rows in the matrix
 * @complexity O(n)
 */
void deleteMatrix(double **m, int n);

/************************* Vertex  **************************/

template<class T>
Vertex<T>::Vertex(T in) : info(in) {
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template<class T>
Edge<T> *Vertex<T>::addEdge(Vertex<T> *d, double w) {
    auto newEdge = new Edge<T>(this, d, w);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template<class T>
bool Vertex<T>::removeEdge(T in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex<T> *dest = edge->getDest();
        if (dest->getInfo() == in) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        } else {
            it++;
        }
    }
    return removedEdge;
}

/*
 * Auxiliary function to remove an outgoing edge of a vertex.
 */
template<class T>
void Vertex<T>::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

template<class T>
bool Vertex<T>::operator<(Vertex<T> &vertex) const {
    return this->dist < vertex.dist;
}

template<class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template<class T>
int Vertex<T>::getLow() const {
    return this->low;
}

template<class T>
void Vertex<T>::setLow(int value) {
    this->low = value;
}

template<class T>
int Vertex<T>::getNum() const {
    return this->num;
}

template<class T>
void Vertex<T>::setNum(int value) {
    this->num = value;
}

template<class T>
std::vector<Edge<T> *> Vertex<T>::getAdj() const {
    return this->adj;
}

template<class T>
bool Vertex<T>::isVisited() const {
    return this->visited;
}

template<class T>
bool Vertex<T>::isProcessing() const {
    return this->processing;
}

template<class T>
unsigned int Vertex<T>::getIndegree() const {
    return this->indegree;
}

template<class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template<class T>
Edge<T> *Vertex<T>::getPath() const {
    return this->path;
}

template<class T>
std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

template<class T>
void Vertex<T>::setInfo(T in) {
    this->info = in;
}

template<class T>
void Vertex<T>::setVisited(bool visited) {
    this->visited = visited;
}

template<class T>
void Vertex<T>::setProcessing(bool processing) {
    this->processing = processing;
}

template<class T>
void Vertex<T>::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

template<class T>
void Vertex<T>::setDist(double dist) {
    this->dist = dist;
}

template<class T>
void Vertex<T>::setPath(Edge<T> *path) {
    this->path = path;
}

template<class T>
void Vertex<T>::deleteEdge(Edge<T> *edge) {
    Vertex<T> *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getInfo() == info) {
            it = dest->incoming.erase(it);
        } else {
            it++;
        }
    }
    delete edge;
}

/********************** Edge  ****************************/

template<class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double w) : orig(orig), dest(dest), weight(w) {
}

template<class T>
Vertex<T> *Edge<T>::getDest() const {
    return this->dest;
}

template<class T>
double Edge<T>::getWeight() const {
    return this->weight;
}

template<class T>
Vertex<T> *Edge<T>::getOrig() const {
    return this->orig;
}

template<class T>
Edge<T> *Edge<T>::getReverse() const {
    return this->reverse;
}

template<class T>
bool Edge<T>::isSelected() const {
    return this->selected;
}

template<class T>
double Edge<T>::getFlow() const {
    return flow;
}

template<class T>
typename Edge<T>::EdgeType Edge<T>::getType() const {
    return this->type;
}

template<class T>
void Edge<T>::setType(EdgeType type) {
    this->type = type;
}

template<class T>
std::string Edge<T>::getTypeString() const {
    switch (this->type) {
        case EdgeType::DRIVING:
            return "Driving";
        case EdgeType::WALKING:
            return "Walking";
        default:
            return "Default";
    }
}

template<class T>
void Edge<T>::setSelected(bool selected) {
    this->selected = selected;
}

template<class T>
void Edge<T>::setReverse(Edge<T> *reverse) {
    this->reverse = reverse;
}

template<class T>
void Edge<T>::setFlow(double flow) {
    this->flow = flow;
}

/********************** Graph  ****************************/

template<class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template<class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template<class T>
Vertex<T> *Graph<T>::findVertex(const T &in) const {
    for (auto v: vertexSet)
        if (v->getInfo() == in)
            return v;
    return nullptr;
}

/*
 * Finds the index of the vertex with a given content.
 */
template<class T>
int Graph<T>::findVertexIdx(const T &in) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getInfo() == in)
            return i;
    return -1;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template<class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != nullptr)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template<class T>
bool Graph<T>::removeVertex(const T &in) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getInfo() == in) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u: vertexSet) {
                u->removeEdge(v->getInfo());
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template<class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);
    return true;
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template<class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> *srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

template<class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w);
    auto e2 = v2->addEdge(v1, w);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

inline void deleteMatrix(int **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete[] m[i];
        delete[] m;
    }
}

inline void deleteMatrix(double **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete[] m[i];
        delete[] m;
    }
}

template<class T>
Graph<T>::~Graph() {
    deleteMatrix(distMatrix, vertexSet.size());
    deleteMatrix(pathMatrix, vertexSet.size());
}

#endif /* DA_TP_CLASSES_GRAPH */
