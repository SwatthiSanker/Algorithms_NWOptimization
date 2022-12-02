#pragma once

class edgeNode{
    public:
    int vertex;
    int weight;
    edgeNode* next;
    edgeNode();
    edgeNode(int v, int w);
};

class Graph{
    public:
    int numVertices;
    edgeNode **adjList;
    Graph(int n);
    ~Graph();
    void addEdge(int src, int dest, int wt);
    void printGraph();
    void DFS(edgeNode path[], char color[], int n, int &index, int s, int t);
};

edgeNode* createNode(int dest, int wt);
void genGraphAvgDeg(Graph &G, int n, int deg);
void makeGraphConnected(Graph &G, int n);

extern bool new_iteration;