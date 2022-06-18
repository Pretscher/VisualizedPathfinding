#include "HeapNode.hpp"
#include "BinaryHeap.hpp"


	int BinaryHeap::getCurrentNodeCount() {
		return heap.size();
	}

	void BinaryHeap::actualizeGraphIndex(int indexInHeap) {
		int graphIndex = heap[indexInHeap].getIndexInGraph();
		graph[graphIndex]->setHeapIndex(indexInHeap);
	}

	void BinaryHeap::dontReinsert(int indexInHeap) {
		int graphIndex = heap[indexInHeap].getIndexInGraph();
		graph[graphIndex]->setHeapIndex(-2);
	}
	//-------------------------------------------
	//Heap methods

	void BinaryHeap::insert(int heuristic, int graphIndex) {
		HeapNode node(heuristic, graphIndex);
		int tempSize = heap.size();
		heap.push_back(node);
		actualizeGraphIndex(tempSize);
		bubbleUp(tempSize);
	}

	HeapNode BinaryHeap::extractMin() {
		HeapNode copy = heap[0];
		//delete heap[0];
		//overwrite with last leaf of heap
		dontReinsert(0);
		if (heap.size() > 1) {
			heap[0] = heap[heap.size() - 1];//copy pointer
			heap.pop_back();//one of the two identical shared pointers in vector goes out of scope
			actualizeGraphIndex(0);
			bubbleDown(0);
		}
		else {
			heap.pop_back();
		}
		return copy;
	}

	void BinaryHeap::decrease(int heapIndex, float newKey) {
		//key is better than before so change the key
		heap[heapIndex].setKey(newKey);
		bubbleUp(heapIndex);
	}

	//-------------------------------------------
	//Help heap methods

	//in these methods we do not check for array bounds for the sake of performance
	int BinaryHeap::getLeftChildIndex(int indexOfNodeInHeap) {
		int childIndex = 2 * indexOfNodeInHeap + 1;
		return childIndex;
	}

	int BinaryHeap::getRightChildIndex(int indexOfNodeInHeap) {
		int childIndex = 2 * (indexOfNodeInHeap + 1);
		return childIndex;
	}

	int BinaryHeap::getParentIndex(int indexOfNodeInHeap) {
		//if rightchild: automatically floors to same number
		int parentIndex = (indexOfNodeInHeap - 1) / 2;
		return parentIndex;
	}

	bool BinaryHeap::isRoot(int indexOfNodeInHeap) {
		return (indexOfNodeInHeap == 0);
	}

	bool BinaryHeap::isLeaf(int indexOfNodeInHeap) {
		if (indexOfNodeInHeap + 1 > heap.size() / 2) {
			return true;
		}
		else {
			return false;
		}
	}

	void BinaryHeap::bubbleUp(int indexOfNodeInHeap) {
		if (isRoot(indexOfNodeInHeap) == true) {
			return;
		}
		int tempParentIndex = getParentIndex(indexOfNodeInHeap);
		HeapNode& parent = heap[tempParentIndex];
		int tempIndex = indexOfNodeInHeap;
		HeapNode& tempNode = heap[indexOfNodeInHeap];

		while (isRoot(tempIndex) == false && parent.getKey() > tempNode.getKey()) {
			//swap node and parent
			heap[tempParentIndex] = tempNode;
			actualizeGraphIndex(tempParentIndex);
			heap[tempIndex] = parent;
			actualizeGraphIndex(tempIndex);
			//Set index of node to new place
			tempIndex = tempParentIndex;
			//get new parent index + HeapNode for the while condition
			tempParentIndex = getParentIndex(tempIndex);
			parent = heap[tempParentIndex];
		}
	}

	void BinaryHeap::bubbleDown(int indexOfNodeInHeap) {
		int tempChildIndex;
		int tempIndex = indexOfNodeInHeap;

		while (isLeaf(tempIndex) == false) {

			//select child to swap with
			tempChildIndex = getLeftChildIndex(tempIndex);

			int rightChildIndex = getRightChildIndex(tempIndex);
			//if leftKey > rightKey swap
			//there does not need to be a right child for every non-leaf => check if there is
			if (rightChildIndex < heap.size()) {
				if (heap[tempChildIndex].getKey() > heap[rightChildIndex].getKey()) {
					tempChildIndex = rightChildIndex;
				}
			}
			//tempChildIndex can be the left or the right Childindex now
			//if heap has the right structure end this method
			if (heap[tempChildIndex].getKey() >= heap[tempIndex].getKey()) {
				break;
			}
			//swap node and parent
			HeapNode tempChild = heap[tempChildIndex];//copy child
			heap[tempChildIndex] = heap[tempIndex];//overwrite child
			heap[tempIndex] = tempChild;//reinsert child at parents index

			actualizeGraphIndex(tempChildIndex);
			actualizeGraphIndex(tempIndex);
			//Set index of node to new place
			tempIndex = tempChildIndex;
		}
	}
