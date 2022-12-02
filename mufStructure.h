#include <iostream>
#include "graphDefinition.h"
using namespace std;

#pragma once

class muf{
    public:
    int *rank;
    int *dad;
    int n;
    muf(int n);
    void makeSet(int v);
    int find(int v);
    void Union(int e1, int e2);
};