#include "Algorithm.hpp"
#include "Binaryheap.hpp"

class DrawnAstar : public Algorithm {
private:
    GraphNode* startNode;
    GraphNode* goalNode;
    int graphNodeCount;
    BinaryHeap* heap;
    bool foundPath = false;
public:
  
    void initPathfinding(int startX, int startY, int goalX, int goalY) {
        clearDrawings();//clear marks of previous algorithms.
		startNode = graph.getIndexFromCoords(startX, startY);
        goalNode = graph.getIndexFromCoords(goalX, goalY);
        if(startNode == nullptr) {
            std::cout << "The start node at coordinates x = " << startX << ", y = " << startY << " for A*-pathfinding is not existent or obstructed.\n";
        }
        if(goalNode == nullptr) {
            std::cout << "The goal node at coordinates x = " << goalX << ", y = " << goalY << " for A*-pathfinding is not existent or obstructed.\n";
        }
        graphNodeCount = graph.nodes.size();
        heap = new BinaryHeap(graph);
        graph.reset();

        startNode->setPreviousNode(startNode);
        startNode->setDistanceTravelled(0);
        //insert start node with the value 0
        heap->insert(getHeuristic(startNode, goalNode), startNode->getIndexInAlgorithmGraph());
        bool foundPath = false;
    }

	DrawnAstar(Graph& i_graph, Grid& i_grid) : Algorithm (i_graph), grid(i_grid) {

	}

    bool isPathFound() {
        return foundPath;
    }

    vector<Point> tryRetrievingPath() {
        if (foundPath == true) {
            foundPath = false;
            vector<Point> path = retrievePath(startNode, goalNode);
            clearNeighbourDrawings();//can only be done here because for this to work a path has to have been found.
            if (path.size() == 0) {
                cout << "\nCould not find path!-----------------------------------------------------\n\n\n";
                return vector<Point>();
            }
            return path;
        } else {
            cout << "\nPath not found yet!-----------------------------------------------------\n\n\n";
            return vector<Point>();
        }
    }

	void updatePathFinding() {
        HeapNode helpNode(heap->extractMin());//extract best node
        GraphNode* cNode = graph.nodes[helpNode.getIndexInGraph()];//get graphIndex of best node
        if (cNode == goalNode) {
            foundPath = true;
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
                        heap->insert(heuristicOfCurrentNeighbour, cNeighbour->getIndexInAlgorithmGraph());
                    }
                    else {
                        heap->decrease(cNeighbour->getHeapIndex(), heuristicOfCurrentNeighbour);
                    }
                }
        //	}
        }
        drawAllAndSleep(cNode, cNeighbours);        
    }
    /**
     * @brief Clear marks of previously visualized Algorithm steps. This is done before a new pathfinding is started.
     *  Safe to call even if no pathfinding was previously started.
     * 
     */
    void clearDrawings() {
        for(Point p : heapPixelsToClear) {
            grid.setPixel(p.x, p.y, sf::Color::White);
        }
        heapPixelsToClear.clear();
        for(vector<Point> path : pathsToDraw) {
            for(Point p : path) {
                grid.setPixel(p.x, p.y, sf::Color::White);
            }
            path.clear();
        }
        pathsToDraw.clear();
        lastHeapPixel = Point();
    }

private:
    int sleep = 15;
    vector<vector<Point>> pathsToDraw;
    vector<Point> neighbourPixelsToClear;
    vector<Point> heapPixelsToClear;//cleared when new path is asked to be found or called
    Point lastHeapPixel;

    void drawAllAndSleep(const GraphNode* const cNode, const vector<GraphNode*>& cNeighbours) {
        clearLastFrame();
        //draw abandoned paths
        int counter = 0;
        for(int i = 0; i < pathsToDraw.size(); i++) {
            for(Point p : pathsToDraw[i]) {
                grid.setPixel(p.x, p.y, sf::Color(0, 255, 0, 100));
            }
        }

        //draw path that led to above node
        //while we haven't reached the start node, always terminates because a node cannot be in heap if the previousnodes dont lead to the startnode
        const GraphNode* iteratorNode = cNode->getPreviousNode(); 
        vector<Point> cPath;
        while (true) {
            grid.setPixel(iteratorNode->getX(), iteratorNode->getY(), sf::Color(0, 0, 255, 200));
            cPath.push_back(Point(iteratorNode->getX(), iteratorNode->getY()));
            if(iteratorNode == startNode) {//has to be in the middle of the while loop because we want to draw the startNode before leaving the loop
                break;
            }
            iteratorNode = iteratorNode->getPreviousNode();
        }
        pathsToDraw.push_back(move(cPath));
        

        //draw currently viewed neighbours
        for(GraphNode* cNeighbour : cNeighbours) {
            grid.setPixel(cNeighbour->getX(), cNeighbour->getY(), sf::Color::Blue);
            neighbourPixelsToClear.push_back(Point(cNeighbour->getX(), cNeighbour->getY()));
        }

        //draw node extracted from heap ("current best option")
        grid.setPixel(cNode->getX(), cNode->getY(), sf::Color::Red);
        lastHeapPixel = Point(cNode->getX(), cNode->getY());

        //std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
    }

    void clearLastFrame() {
        for(Point p : neighbourPixelsToClear) {
            grid.setPixel(p.x, p.y, sf::Color::White);
        }
        if(lastHeapPixel.x != -1) {//is not initialized in first iteration, but default constructor is initialized to (-1, -1)
            grid.setPixel(lastHeapPixel.x, lastHeapPixel.y, sf::Color::White);
            heapPixelsToClear.push_back(Point(lastHeapPixel.x, lastHeapPixel.y));
        }
        neighbourPixelsToClear.clear();
    }

    void clearNeighbourDrawings() {
        //draw currently viewed neighbours
        for(GraphNode* cNeighbour : goalNode->getNeighbours()) {
            grid.setPixel(cNeighbour->getX(), cNeighbour->getY(), sf::Color::White);
        }
    }

    Grid& grid;
};
