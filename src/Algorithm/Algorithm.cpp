#include "BinaryHeap.hpp"
#include <cmath>
#include <chrono>
#include "Algorithm.hpp"
#include <limits>
#include "Renderer.hpp"


bool Algorithm::findPath(vector<int>& o_pathYs, vector<int>& o_pathXs, int& o_pathLenght, int startY, int startX, int goalY, int goalX) {
	nextIteration();
	GraphNode* startNode = graph->getIndexFromCoords(startY, startX);
	GraphNode* goalNode = graph->getIndexFromCoords(goalY, goalX);

	int graphNodeCount = graph->nodes.size();
	BinaryHeap* heap = new BinaryHeap(graph, graphNodeCount, currentIteration);

	graph->reset();

	startNode->previousNode = startNode;

	//insert start node with the value 0
	heap->insert(shared_ptr<HeapNode>(new HeapNode(getHeuristic(startNode, goalNode), startNode->indexInGraph)));
	bool foundPath = false;

	while (heap->getCurrentNodeCount() > 0) {//while heap is not empty
		HeapNode helpNode(heap->extractMin());//extract best node
		GraphNode* cNode = graph->nodes[helpNode.getIndexInGraph(currentIteration)];//get graphIndex of best node
		if (cNode == goalNode) {
			foundPath = true;
			break;
		}
		for (int i = 0; i < cNode->neighbours.size(); i++) {
			float heuristics = getHeuristic(cNode->neighbours[i], cNode);
			cNode->neighbourCosts[i] = heuristics;
		}
		//we will look through graph->getNeighbourIndices() of this node
		for (int i = 0; i < cNode->neighbours.size(); i++) {
			GraphNode* cNeighbour = cNode->neighbours[i];
			if (cNeighbour->usedByMoveable == false) {//efficient method to exclude moveable colision objects from graph
				float tempDistance = cNode->distanceTravelled + cNode->neighbourCosts[i];
				if (tempDistance < cNeighbour->distanceTravelled) {
					cNeighbour->distanceTravelled = tempDistance;
					cNeighbour->previousNode = cNode;

					float heuristicOfCurrentNeighbour = tempDistance + getHeuristic(cNeighbour, goalNode);

					//if graphnode has been inserted to heap (index in heap initialized to -1)
					if (cNeighbour->heapIndex != -2) {//in that case dont insert or decrease
						if (cNeighbour->heapIndex != -1) {//-1 = not yet visited, insert
							heap->decrease(cNeighbour->heapIndex, heuristicOfCurrentNeighbour);
						}
						else {
							heap->insert(shared_ptr<HeapNode>(new HeapNode(heuristicOfCurrentNeighbour, cNeighbour->indexInGraph)));
						}
					}
				}
			}
		}
	}

	if (foundPath == true) {

		//cout << "time elapsed sind the algorithm started: " << Utils::endTimerGetTime();

		//get lenght of path array
		int pathLenght = 0;//goal pos pushed back
		GraphNode* cNode = goalNode;
		while (true) {
			cNode = cNode->previousNode;
			pathLenght++;
			if (cNode == startNode) {
				break;
			}
		}

		o_pathXs = vector<int>(pathLenght);
		o_pathYs = vector<int>(pathLenght);
		o_pathLenght = pathLenght;

		//put path indices into path array from end to front

		int indexInPath = pathLenght - 1;
		GraphNode* cNode = goalNode;
		while (true) {
			o_pathXs[indexInPath] = cNode->x;
			o_pathYs[indexInPath] = cNode->y;
			cNode = cNode->previousNode;
			indexInPath--;
			if (cNode == startNode) {
				break;
			}
		}


		if (pathLenght == 0) {
			cout << "\nNo path possible!-----------------------------------------------------\n\n\n";
			delete heap;
			return false;
		}
		o_pathXs[pathLenght - 1] = goalX;//can now be written caus pathlenght is not 0
		o_pathYs[pathLenght - 1] = goalY;
		delete heap;
		return true;
	}
	delete heap;
	cout << "\nNo path possible!-----------------------------------------------------\n\n\n";
	return false;
}

float Algorithm::getHeuristic(GraphNode* start, GraphNode* goal) {
	float heuristics = (float)sqrt(
						  abs(goal->x - start->x) * abs(goal->x - start->x) 
						+ abs(goal->y - start->y) * abs(goal->y - start->y));
	return heuristics;
}