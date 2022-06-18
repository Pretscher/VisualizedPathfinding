#pragma once
#include <iostream>
using namespace std;
#include <limits>

class HeapNode {
public:
    HeapNode(float key, int indexInGraph) {
        myKey = key;
        myIndexInGraph = indexInGraph;
    }

    inline float getKey() const {
        return myKey;
    }

    inline int getIndexInGraph() const {
        return myIndexInGraph;
    }

    inline void setKey(float key) {
        myKey = key;
    }
private:
    float myKey;
    int myIndexInGraph;
};