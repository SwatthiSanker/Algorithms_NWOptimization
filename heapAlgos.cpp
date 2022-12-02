#include <iostream>
#include "heapStructure.h"
#include <climits>
using namespace std;

heap::heap(int n)
{
    heap_size = 0;
    capacity = n;
    h = new heapNode[n];
    position = new int[n];
    for(int i = 0; i < n; i++)
        position[i] = -1;
}

heapNode::heapNode(){}

heapNode::heapNode(int vertex, int bw)
{
    v = vertex;
    this->bw = bw;
}

heap::~heap()
{
    delete[] h;
    delete[] position;
}

void heap::swap(int e1, int e2)
{
    int a = h[e1].v;
    int b = h[e2].v;
    position[a] = e2;
    position[b] = e1;
    heapNode temp = h[e1];
    h[e1] = h[e2];
    h[e2] = temp;
}

void heap::insHeapify(int pos)
{
    while ((pos != 0) && (h[pos].bw > h[(pos-1)/2].bw))
    {
        swap((pos - 1) / 2, pos);
        pos = (pos - 1) / 2;
    }
}

void heap::insertHeap(int node, int bw)
{
    if(heap_size == capacity)
    {
        cout << "Overflow" << endl;
        return;
    }
    heap_size++;
    int i = heap_size - 1;
    position[node] = i;
    h[i] = heapNode(node,bw);
    insHeapify(i);
}

void heap::printHeap()
{
    for(int i = 0; i < heap_size; i++)
    {
        cout << h[i].v << " " << h[i].bw << " " << h[i].dest << endl;
    }
}

void heap::delHeapify(int pos)
{
    int l, r;
    int largest = pos;
    int len = heap_size;

    l = 2 * pos + 1;
    if (l < len)
    {
        if (h[l].bw > h[largest].bw)
        {
            largest = l;
        }
    }

    r = 2 * (1 + pos);
    if (r < len)
    {
        if (h[r].bw > h[largest].bw)
        {
            largest = r;
        }
    }

    if (largest != pos)
    {
        swap(pos, largest);
        delHeapify(largest);
    }
}

void heap::extractMax()
{
    if(heap_size <= 0)
    return;
    if(heap_size == 1)
    {
        heap_size--;
        return;
    }
    position[h[0].v] = -1;
    h[0] = h[heap_size-1];
    position[h[0].v] = 0;
    heap_size--;
    delHeapify(0);
}

void heap::increaseKey(int idx, int new_value)
{
    h[idx].bw = new_value;
    insHeapify(idx); 
}

void heap::deleteHeap(int idx)
{
    increaseKey(idx, INT_MAX);
    extractMax();
}

int heap::maxNode()
{
    if(heap_size <= 0)
        return INT_MIN;
    else
        return h[0].v;
}

edge::edge(int s, int t, int w)
{
    src = s;
    dest = t;
    wt = w;
}

void edge::createEdge(int s, int t, int w)
{
    src = s;
    dest = t;
    wt = w;
}

edge::edge(){}

edgeHeap::edgeHeap(int n)
{
    heap_size = 0;
    capacity = n;
    h = new edge[n];
}

edgeHeap::~edgeHeap()
{
    delete[] h;
}

void edgeHeap::edgeSwap(int e1, int e2)
{
    edge temp = h[e1];
    h[e1] = h[e2];
    h[e2] = temp;
}

void edgeHeap::insEdgeHeapify(int pos)
{
    while ((pos != 0) && (h[pos].wt > h[(pos-1)/2].wt))
    {
        edgeSwap((pos - 1) / 2, pos);
        pos = (pos - 1) / 2;
    }
}

void edgeHeap::insertEdgeHeap(int src, int dest, int wt)
{
    if(heap_size == capacity)
    {
        cout << "Overflow" << endl;
        return;
    }
    heap_size++;
    int i = heap_size - 1;
    h[i] = edge(src,dest,wt);
    insEdgeHeapify(i);
}

void edgeHeap::delEdgeHeapify(int pos)
{
    int l, r;
    int largest = pos;
    int len = heap_size;

    l = 2 * pos + 1;
    if (l < len)
    {
        if (h[l].wt > h[largest].wt)
        {
            largest = l;
        }
    }

    r = 2 * (1 + pos);
    if (r < len)
    {
        if (h[r].wt > h[largest].wt)
        {
            largest = r;
        }
    }

    if (largest != pos)
    {
        edgeSwap(pos, largest);
        delEdgeHeapify(largest);
    }
}

void edgeHeap::extractMaxEdge()
{
    if(heap_size <= 0)
    return;
    if(heap_size == 1)
    {
        heap_size--;
        return;
    }
    h[0] = h[heap_size-1];
    heap_size--;
    delEdgeHeapify(0);
}

void edgeHeap::deleteEdgeHeap()
{
    extractMaxEdge();
}

edge edgeHeap::maxEdgeNode()
{
    if(heap_size <= 0)
        return edge(INT_MIN,INT_MIN,INT_MIN);
    else
        return h[0];
}

edge* heapSort(edge edgeArr[], int m)
{
    edgeHeap hs(m);
    for(int i = 0; i < m; i++)
    {
        hs.insertEdgeHeap(edgeArr[i].src,edgeArr[i].dest,edgeArr[i].wt);
    }
    int idx = 0;
    while(hs.heap_size > 0)
    {
        edgeArr[idx++] = hs.maxEdgeNode();
        hs.deleteEdgeHeap();
    }
    return edgeArr;
}   