#pragma once
#include "HeapNode.hpp"
#include "Graph.hpp"
#include <memory>
class BinaryHeap {
public:
	//needs current iteration for efficiency: heap nodes are not deleted, but if they are reinserted into the heap and they were
	//initialized in another run of the algorithm, they reinitialize themselves.
	BinaryHeap::BinaryHeap(vector<GraphNode*>& graphNodes) {
		//heap has same size as graph
		graph = graphNodes;
	}
	~BinaryHeap() {
		for(int i = 0; i < heap.size(); i++) {
			delete heap[i];
		}
	}

	int getCurrentNodeCount();
	void actualizeGraphIndex(int indexInHeap);

	void insert(int heuristic, int graphIndex);
	//extract root of heap. Memory is managed internally so this returns a copy.
	HeapNode extractMin();
	void decrease(int heapIndex, float newKey);

	int getLeftChildIndex(int indexOfNodeInHeap);
	int getRightChildIndex(int indexOfNodeInHeap);
	int getParentIndex(int indexOfNodeInHeap);

	bool isRoot(int indexOfNodeInHeap);
	bool isLeaf(int indexOfNodeInHeap);

	void bubbleUp(int indexOfNodeInHeap);

	void bubbleDown(int indexOfNodeInHeap);
	void dontReinsert(int indexInHeap);
private:
	vector<HeapNode*> heap;
	vector<GraphNode*> graph;
};
