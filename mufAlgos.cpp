#include<iostream>
#include "mufStructure.h"
#include "graphDefinition.h"
#include "heapStructure.h"
#include <climits>
using namespace std;

muf::muf(int n)
{
    rank = new int[n];
    dad = new int[n];
    this->n = n;
}

void muf::makeSet(int v)
{
    dad[v] = -1;
    rank[v] = 0;
}

int muf::find(int v)
{
    int r = v;
    int S[n];
    int sIndex = 0;
    while(dad[r]!=-1)
    {
        S[sIndex++] = r;
        r = dad[r];
    }
    for(int i = 0; i < sIndex; i++)
    {
        int w = S[i];
        dad[w] = r;
    }
    return r;
}

void muf::Union(int r1, int r2)
{
    if(r1 == r2)
    return;
    if(rank[r1] > rank[r2])
        dad[r2] = r1;
    else if(rank[r1] < rank[r2])
        dad[r1] = r2;
    else
    {
        dad[r1] = r2;
        rank[r2]++;
    }
}
