#include "BinaryHeap.hpp"
#include <cmath>
#include <chrono>
#include "Algorithm.hpp"


vector<Point> Algorithm::findPath(int startX, int startY, int goalX, int goalY) {
	GraphNode* startNode = graph->getIndexFromCoords(startX, startY);
	GraphNode* goalNode = graph->getIndexFromCoords(goalX, goalY);

	if(startNode == nullptr) {
		std::cout << "The start node at coordinates x = " << startX << ", y = " << startY << " for A*-pathfinding is not existent or obstructed.\n";
		return vector<Point>();
	}
	if(goalNode == nullptr) {
		std::cout << "The goal node at coordinates x = " << goalX << ", y = " << goalY << " for A*-pathfinding is not existent or obstructed.\n";
		return vector<Point>();
	}

	int graphNodeCount = graph->nodes.size();
	BinaryHeap* heap = new BinaryHeap(graph->nodes);

	//graph may have been used in a previous algorithm and we write information to it like distanceTravelled
	//and PreviousNode, which have to be reset
	graph->reset();

	//algorithm start
	auto startTime = std::chrono::high_resolution_clock::now();

	startNode->previousNode = startNode;
	startNode->distanceTravelled = 0;
	//insert start node with the value 0
	heap->insert(getHeuristic(startNode, goalNode), startNode->indexInGraph);
	bool foundPath = false;

	while (heap->getCurrentNodeCount() > 0) {//while heap is not empty
		HeapNode helpNode(heap->extractMin());//extract best node
		GraphNode* cNode = graph->nodes[helpNode.getIndexInGraph()];//get graphIndex of best node
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
							heap->insert(heuristicOfCurrentNeighbour, cNeighbour->indexInGraph);
						}
					}
				}
			}
		}
	}
	delete heap;
	if (foundPath == true) {
		vector<Point> path = retrievePath(startNode, goalNode);

		auto timeDiff = std::chrono::high_resolution_clock::now() - startTime;
		std::cout << "Path of " <<  path.size() << " nodes found in " << (float)timeDiff.count() / 1000000 << "ms.\n";
		if (path.size() == 0) {
			cout << "\nNo path possible!-----------------------------------------------------\n\n\n";
			return vector<Point>();
		}
		return path;
	}
	cout << "\nNo path possible!-----------------------------------------------------\n\n\n";
	return vector<Point>();
}

float Algorithm::getHeuristic(GraphNode* start, GraphNode* goal) {
	float heuristics = (float)sqrt(
						  abs(goal->x - start->x) * abs(goal->x - start->x) 
						+ abs(goal->y - start->y) * abs(goal->y - start->y));
	return heuristics;
}

vector<Point> Algorithm::retrievePath(GraphNode* startNode, GraphNode* goalNode) {
	int pathLenght = goalNode->distanceTravelled;
	vector<Point> path;
	path.resize(pathLenght);//can this be repalced by reserve()?
	//put path indices into path array from end to front
	GraphNode* cNode = goalNode;
	while (true) {
		path[pathLenght].x = cNode->x;
		path[pathLenght].y = cNode->y;
		pathLenght --;
		if (cNode == startNode) {
			break;
		}
		cNode = cNode->previousNode;
	}
	//we want the path to start at the start node, not the goal node
	return path;
}