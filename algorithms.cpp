#include <iostream>
#include "graphDefinition.h"
#include "heapStructure.h"
#include "mufStructure.h"
#include <ctime>
#include <chrono>
#include <fstream>
#include <climits>
using namespace std;

bool new_iteration = false;
ofstream ofile("output1.txt");

void initializeArrays(char status[],int bwidth[],int dad[], int n)
{
    for(int i = 0; i < n; i++)
    {
        status[i] = 'u';
        bwidth[i] = 0;
        dad[i] = 0;
    }
}

void printMaxBWPath(int s, int t, int *bwidth, int *dad)
{
    //Print out the path
    int i = t;
    int vertexCount = 0;
    while(dad[i]!=-1)
    {
        vertexCount++;
        i = dad[i];
    }
    int pathArr[vertexCount+1];
    i = t;
    int index = vertexCount;
    pathArr[index--] = t;
    while(dad[i]!=-1)
    {
        pathArr[index--] = dad[i];
        i = dad[i];
    }
    pathArr[index] = s;
    int k = 0;
    for(k = 0; k < vertexCount; k++)
        ofile << pathArr[k] << " --> ";
    ofile << pathArr[k];
    ofile << endl;
    ofile << "Max BW = " << bwidth[t] << endl;
}

int dijkstraNoHeap(Graph &G,int n, int s, int t)
{
    char status[n];
    int bwidth[n];
    int dad[n];
    initializeArrays(status,bwidth,dad,n);

    // Initialize source node
    status[s] = 'i';
    bwidth[s] = INT_MAX;
    dad[s] = -1;

    edgeNode *iter = G.adjList[s];
    while(iter != nullptr)
    {
        status[iter->vertex] = 'f';
        bwidth[iter->vertex] = iter->weight;
        dad[iter->vertex] = s;
        iter = iter->next;
    }

    //Count the initial number of fringers
    int fringeCnt = 0;
    int maxBwFringe = -1;
    int maxBW = INT_MIN;
    for(int i = 0; i < n; i++)
    {
        if(status[i] == 'f')
        {
            //cout << i << " ";
            fringeCnt++;
            if(bwidth[i] > maxBW)
            {
                maxBW = bwidth[i];
                maxBwFringe = i;
            }
        }
    }
    while(fringeCnt > 0)
    {
        status[maxBwFringe] = 'i';
        edgeNode *iter = G.adjList[maxBwFringe];
        while(iter != nullptr)
        {
            if(status[iter->vertex] == 'u')
            {
                status[iter->vertex] = 'f';
                dad[iter->vertex] = maxBwFringe;
                bwidth[iter->vertex] = std::min(bwidth[maxBwFringe],iter->weight);
            }
            else if(status[iter->vertex] == 'f')
            {
                if(bwidth[iter->vertex] < min(bwidth[maxBwFringe],iter->weight))
                {
                    dad[iter->vertex] = maxBwFringe;
                    bwidth[iter->vertex] = min(bwidth[maxBwFringe],iter->weight);
                }
            }
            iter = iter->next;
        }
        fringeCnt = 0;
        maxBwFringe = -1;
        maxBW = INT_MIN;
        for(int i = 0; i < n; i++)
        {
            if(status[i] == 'f')
            {
                fringeCnt++;
                if(bwidth[i] > maxBW)
                {
                    maxBW = bwidth[i];
                    maxBwFringe = i;
                }
            }
        }
    }
    printMaxBWPath(s,t,bwidth,dad);
    return bwidth[t];
}

int dijkstraWithHeap(Graph &G, int n, int s, int t)
{
    char status[n];
    int bwidth[n];
    int dad[n];
    initializeArrays(status,bwidth,dad,n);

    // Initialize source node
    status[s] = 'i';
    bwidth[s] = INT_MAX;
    dad[s] = -1;

    edgeNode *iter = G.adjList[s];
    heap H(n);
    while(iter != nullptr)
    {
        status[iter->vertex] = 'f';
        bwidth[iter->vertex] = iter->weight;
        dad[iter->vertex] = s;
        H.insertHeap(iter->vertex, bwidth[iter->vertex]);
        iter = iter->next;
    }

    while(status[t]!='i')
    {
        if(H.heap_size <= 0)
            return 0;

        int maxBwFringe = H.maxNode();
        status[maxBwFringe] = 'i';
        H.deleteHeap(0);
        edgeNode *iter = G.adjList[maxBwFringe];
        while(iter != nullptr)
        {
            if(status[iter->vertex] == 'u')
            {
                status[iter->vertex] = 'f';
                dad[iter->vertex] = maxBwFringe;
                bwidth[iter->vertex] = std::min(bwidth[maxBwFringe],iter->weight);
                H.insertHeap(iter->vertex, bwidth[iter->vertex]);
            }
            else if(status[iter->vertex] == 'f')
            {
                if(bwidth[iter->vertex] < min(bwidth[maxBwFringe],iter->weight))
                {
                    H.deleteHeap(H.position[iter->vertex]);
                    dad[iter->vertex] = maxBwFringe;
                    bwidth[iter->vertex] = min(bwidth[maxBwFringe],iter->weight);
                    H.insertHeap(iter->vertex,bwidth[iter->vertex]);
                }
            }
            iter = iter->next;
        }
    }
    printMaxBWPath(s,t,bwidth,dad);
    return bwidth[t];
}

int kruskalBW(Graph &G, edge edgeArr[], int m, int n, int s, int t)
{
    Graph kruskalAnswer(n);
    edge* sortedEdges = heapSort(edgeArr,m);
    muf dsu(n);
    for(int i = 0; i < n; i++)
        dsu.makeSet(i);
    for(int i = 0; i < m; i++)
    {
        int parentSrc = dsu.find(sortedEdges[i].src);
        int parentDest = dsu.find(sortedEdges[i].dest);
        if(parentSrc != parentDest)
        {
            kruskalAnswer.addEdge(sortedEdges[i].src,sortedEdges[i].dest,sortedEdges[i].wt);
            dsu.Union(parentSrc,parentDest);
        }
    }
    //Print the maxBw path
    edgeNode path[n];
    int index = -1;
    char color[n];
    for(int i = 0; i < n; i++)
    color[i] = 'w';

    kruskalAnswer.DFS(path,color,n,index,s,t);
    
    int maxBW = INT_MAX;
    if(index >= 0)
    {
        ofile << s << " --> ";
        int i = 0;
        for(i = 0; i < index; i++)
        {
            ofile << path[i].vertex << " --> ";
            maxBW = min(maxBW,path[i].weight);
        }
        ofile << path[i].vertex;
        maxBW = min(maxBW,path[i].weight);
    }
    return maxBW;
}

void Graph::DFS(edgeNode path[], char color[], int n, int &index, int s, int t)
{
    static bool pathExists = false;
    if (new_iteration)
    {
        for(int i = 0; i < n; i++)
        color[i] = 'w';
        pathExists = false;
        new_iteration = false;
    }
    if(s == t)
    {
        pathExists = true;
        return;
    }
    else
    {
        color[s] = 'g';
        edgeNode *list = this->adjList[s];
        while(list != nullptr)
        {
            if(color[list->vertex] == 'w')
            {
                path[++index].vertex = list->vertex;
                path[index].weight = list->weight;
                DFS(path,color,n,index,list->vertex,t);
                if(pathExists)
                    return;
            }
            list = list->next;
        }
        color[s] = 'b';
        if(index >= 0)
        {
            index--;
        }
    }
}

void runAlgos(int deg)
{
    if(deg == 6)
        ofile << "------- Spare graph results ----------";
    else
        ofile << "------- Dense graph results ----------";
    ofile << endl << endl;
    srand(time(NULL));
    int num_iterations = 5;
    for(int i = 0; i < num_iterations; i++)
    {
        int n = 5000;
        Graph g(n);
        ofile << "-------------   Graph " << i+1 << " -------------" << endl;
        auto startTime_gn = std::chrono::high_resolution_clock::now();
        if(deg == 6)
            makeGraphConnected(g,n);
        genGraphAvgDeg(g,n,deg);
        auto endTime_gn = std::chrono::high_resolution_clock::now();
        auto timeTaken_gn = (endTime_gn - startTime_gn);
        ofile << "Time taken to generate Graph " << i+1 << " = " << timeTaken_gn/std::chrono::milliseconds(1) << " milliseconds" << endl;

        for(int j = 0; j < num_iterations; j++)
        {
            int s = rand() % n + 1;
            int t = rand() % n + 1; 
            ofile << endl << "Source = " << s << endl;
            ofile << "Destination = " << t << endl;
            
            int bw1, bw2, bw3;
            ofile << endl;
            ofile << "Dijkstra without heap" << endl;
            auto startTime1 = std::chrono::high_resolution_clock::now();
            bw1 = dijkstraNoHeap(g,n,s,t);
            auto endTime1 = std::chrono::high_resolution_clock::now();
            auto timeTaken1 = (endTime1 - startTime1);
            ofile << endl << "Time elapsed = " << timeTaken1/std::chrono::milliseconds(1) << " milliseconds" << endl;

            ofile << endl << "Dijkstra with heap" << endl;
            auto startTime2 = std::chrono::high_resolution_clock::now();
            bw2 = dijkstraWithHeap(g,n,s,t);
            auto endTime2 = std::chrono::high_resolution_clock::now();
            auto timeTaken2 = (endTime2 - startTime2);
            ofile << endl << "Time elapsed = " << timeTaken2/std::chrono::milliseconds(1) << " milliseconds" << endl;

            ofile << endl << "Kruskal algorithm " << endl;
            int numEdges = 0;
            edgeNode **list = g.adjList;
            for(int i = 0; i < n; i++)
            {
                edgeNode *head = list[i];
                while(head != nullptr)
                {
                    if(head->vertex > i)
                        numEdges++;
                    head = head->next;
                }
            }
            edge* edgeArr = new edge[numEdges]();
            int idx = 0;
            edgeNode **list1 = g.adjList;
            for(int i = 0; i < n; i++)
            {
                edgeNode *head = list1[i];
                while(head != nullptr)
                {
                    if(head->vertex > i)
                    {
                        edgeArr[idx++].createEdge(i,head->vertex,head->weight);
                    }
                    head = head->next;
                }
            }
            new_iteration = true;
            auto startTime3 = std::chrono::high_resolution_clock::now();
            bw3 = kruskalBW(g,edgeArr,numEdges,n,s,t);
            auto endTime3 = std::chrono::high_resolution_clock::now();
            ofile << endl << "Max BW = " << bw3 << endl;
            auto timeTaken3 = (endTime3 - startTime3);
            ofile << endl << "Time elapsed = " << timeTaken3/std::chrono::milliseconds(1) << " milliseconds" << endl;
            delete[] edgeArr;
            ofile << "-------------------- Iteration over --------------------" << endl;
        }
    }
}


int main()
{
    runAlgos(6);
    runAlgos(1300);
    ofile.close();
    return 0;
}