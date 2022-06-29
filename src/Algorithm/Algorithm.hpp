#pragma once
#include "Graph.hpp"
#include <mutex>
#include <memory>
#include <vector>

struct Point {
public:
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}

	Point() {
		x = -1;
		y = -1;
	}
	int x, y;
};

class Algorithm {
public:
	Algorithm(Graph& i_graph) {
		this->graph = i_graph;
	}

	vector<Point> findPath(int startX, int startY, int goalX, int goalY);
protected:
	Graph graph;
	float getHeuristic(GraphNode* start, GraphNode* goal);
	vector<Point> retrievePath(GraphNode* startNode, GraphNode* goalNode);
};
