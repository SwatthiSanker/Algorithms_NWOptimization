#include <iostream>
#include <cstdlib>
#include <time.h>
#include "graphDefinition.h"
#include <climits>
using namespace std;

#define MAX_BW 5000
#define GEN_RAND_BW (rand() % MAX_BW + 1)

edgeNode::edgeNode(){}
edgeNode::edgeNode(int v, int w)
{
    vertex = v;
    weight = w;
    next = nullptr;
}

edgeNode* createNode(int dest,int wt)
{
    edgeNode *node = new edgeNode(dest,wt);
    return node;
}

Graph::Graph(int n)
{
    numVertices = n;
    adjList = new edgeNode*[numVertices]();
    for(int i = 0; i < n; i++)
        adjList[i] = nullptr;
}

Graph::~Graph()
{
    for(int i = 0; i < numVertices; i++)
        delete[] adjList[i];
    delete[] adjList;
}

void Graph::addEdge(int src, int dest, int wt)
{
    edgeNode* newSrcNode = createNode(dest, wt);
    if(adjList[src] == nullptr)
        adjList[src] = newSrcNode;
    else
    {
        edgeNode *head = adjList[src];
        while(head->next != nullptr)
            head = head->next;
        head->next = newSrcNode;
    }
    edgeNode* newDestNode = createNode(src, wt);
    if(adjList[dest] == nullptr)
        adjList[dest] = newDestNode;
    else
    {
        edgeNode *head = adjList[dest];
        while(head->next != nullptr)
            head = head->next;
        head->next = newDestNode;
    }
}

void Graph::printGraph()
{
    int degree = 0;
    for(int i = 0; i < numVertices; i++)
    {
        edgeNode *head = adjList[i];
        while(head != nullptr)
        {
            degree++;
            cout << head->vertex << " : " << head->weight << " ";
            head = head->next;
        }
        cout << endl;
    }
    cout << "Avg deg = "<< degree/numVertices <<endl;
}

bool isNodeInGraph(Graph &G, int i, int dest)
{
    bool exists = false;
    edgeNode *curr = G.adjList[i];
    while(curr != nullptr)
    {
        if(curr->vertex == dest)
        {
            exists = true;
            break;
        }
        curr = curr->next;
    }   
    return exists;
}

void genGraphAvgDeg(Graph &G, int n, int deg)
{
    int nodeDeg[n];
    for(int i = 0; i < n; i++)
        nodeDeg[i] = 2;
    for(int i = n-1; i > 0; i--)
    {
        while(nodeDeg[i] < deg)
        {
            int dest = rand() % i;   
            if(!isNodeInGraph(G,i,dest))
            {
                G.addEdge(i,dest,GEN_RAND_BW);
            }       
            nodeDeg[i]++;
            nodeDeg[dest]++;
        }
    }
}

void makeGraphConnected(Graph &G, int n)
{
    int prev = 0;
    for(int i = 0; i < n; i++)
    {
        G.addEdge(prev,i,GEN_RAND_BW);
        prev = i;
    }
}