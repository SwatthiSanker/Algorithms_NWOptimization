#pragma once

class edge{
    public:
    int src;
    int dest;
    int wt;
    edge(int s, int t, int w);
    edge();
    void createEdge(int s, int t, int w);
};

class heapNode{
    public:
    int v;
    int bw;
    int dest;
    heapNode();
    heapNode(int v, int bw);
};

class heap{
    public:
    heapNode *h;
    int *position;
    int capacity;
    int heap_size;
    heap(int n);
    ~heap();
    void insertHeap(int node, int bw);
    void insHeapify(int pos);
    void swap(int e1, int e2);
    void printHeap();
    void deleteHeap(int node);
    void delHeapify(int pos);
    void increaseKey(int i, int new_value);
    void extractMax();
    int maxNode();
};

class edgeHeap{
    public:
    edge *h;
    int capacity;
    int heap_size;
    edgeHeap(int n);
    ~edgeHeap();
    void insertEdgeHeap(int src, int dest, int wt);
    void insEdgeHeapify(int pos);
    void edgeSwap(int e1, int e2);
    void deleteEdgeHeap();
    void delEdgeHeapify(int pos);
    void extractMaxEdge();
    edge maxEdgeNode();
};

edge* heapSort(edge edgeArr[], int m);