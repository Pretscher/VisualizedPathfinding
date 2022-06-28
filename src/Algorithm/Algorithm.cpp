#include "BinaryHeap.hpp"
#include <cmath>
#include <chrono>
#include "Algorithm.hpp"


vector<Point> Algorithm::findPath(int startX, int startY, int goalX, int goalY) {
	GraphNode* startNode = graph.getIndexFromCoords(startX, startY);
	GraphNode* goalNode = graph.getIndexFromCoords(goalX, goalY);

	if(startNode == nullptr) {
		std::cout << "The start node at coordinates x = " << startX << ", y = " << startY << " for A*-pathfinding is not existent or obstructed.\n";
		return vector<Point>();
	}
	if(goalNode == nullptr) {
		std::cout << "The goal node at coordinates x = " << goalX << ", y = " << goalY << " for A*-pathfinding is not existent or obstructed.\n";
		return vector<Point>();
	}

	int graphNodeCount = graph.nodes.size();
	BinaryHeap heap(graph);

	//graph may have been used in a previous algorithm and we write information to it like distanceTravelled
	//and PreviousNode, which have to be reset
	graph.reset();

	//algorithm start
	auto startTime = std::chrono::high_resolution_clock::now();

	startNode->setPreviousNode(startNode);
	startNode->setDistanceTravelled(0);
	//insert start node with the value 0
	heap.insert(getHeuristic(startNode, goalNode), startNode->getIndexInAlgorithmGraph());
	bool foundPath = false;

	while (heap.getCurrentNodeCount() > 0) {//while heap is not empty
		HeapNode helpNode(heap.extractMin());//extract best node
		GraphNode* cNode = graph.nodes[helpNode.getIndexInGraph()];//get graphIndex of best node
		if (cNode == goalNode) {
			foundPath = true;
			break;
		}
		const vector<GraphNode*>& cNeighbours = cNode->getNeighbours();
		for (int i = 0; i < cNode->getNeighbours().size(); i++) {
			float heuristics = getHeuristic(cNeighbours[i], cNode);
			cNode->setNeighbourCosts(i, heuristics);
		}
		//we will look through graph->getNeighbourIndices() of this node
		for (int i = 0; i < cNeighbours.size(); i++) {
			GraphNode* cNeighbour = cNeighbours[i];
		//	if (cNeighbour->isUsedByMoveable() == false) {//efficient method to exclude moveable colision objects from graph
				float tempDistance = cNode->getDistanceTravelled() + cNode->getNeighbourCosts(i);
				if (tempDistance < cNeighbour->getDistanceTravelled()) {
					cNeighbour->setDistanceTravelled(tempDistance);
					cNeighbour->setPreviousNode(cNode);

					float heuristicOfCurrentNeighbour = tempDistance + getHeuristic(cNeighbour, goalNode);
					//if graphnode has been inserted to heap (index in heap initialized to -1)
					if (cNeighbour->getHeapIndex() == -1) {//-1 = not yet visited, insert
						heap.insert(heuristicOfCurrentNeighbour, cNeighbour->getIndexInAlgorithmGraph());
					}
					else {
						heap.decrease(cNeighbour->getHeapIndex(), heuristicOfCurrentNeighbour);
					}
				}
		//	}
		}
	}
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
						  abs(goal->getX() - start->getX()) * abs(goal->getX() - start->getX()) 
						+ abs(goal->getY() - start->getY()) * abs(goal->getY() - start->getY()));
	return heuristics;
}

vector<Point> Algorithm::retrievePath(GraphNode* startNode, GraphNode* goalNode) {
	int pathLenght = goalNode->getDistanceTravelled();
	vector<Point> path;
	path.resize(pathLenght + 1);//can this be repalced by reserve()?
	//put path indices into path array from end to front
	const GraphNode* cNode = goalNode;
	while (true) {
		path[pathLenght].x = cNode->getX();
		path[pathLenght].y = cNode->getY();
		pathLenght --;
		if (cNode == startNode) {
			break;
		}
		cNode = cNode->getPreviousNode();
	}
	//we want the path to start at the start node, not the goal node
	return path;
}