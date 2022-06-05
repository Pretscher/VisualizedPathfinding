#include <vector>
#include <limits>
using namespace std;
struct GraphNode {
public:
    //Constructor if you do not want to input the average neighbour count (consumes more memory and
    // can be slower  for graphs with many unusable nodes)
    GraphNode(int x, int y, int i_indexInGraph) {
        this->x = x;
        this->y = y;
        this->indexInGraph = i_indexInGraph;
        //in most graphs, most nodes will have around and maximally 8 neighbours. 
        neighbours.reserve(8);
        neighbourCosts.reserve(8);
    }
    //Constructor if you want to input the average neighbour count (consumes less memory and can be faster 
    //for graphs with many unusable nodes)
    GraphNode(int x, int y, int neighbourCountMean, int i_indexInUseableGraph, int i_indexInFullGraph) {
        this->x = x;
        this->y = y;
        this->indexInGraph = i_indexInUseableGraph;
        this->indexInFullGraph = i_indexInFullGraph;
        //in most graphs, most nodes will have around and maximally 8 neighbours. 
        neighbours.reserve(neighbourCountMean);
        neighbourCosts.reserve(neighbourCountMean);
    }

    inline void addNeighbour(GraphNode* node, int cost) {
        neighbours.push_back(node);
        neighbourCosts.push_back(cost);
    }


    inline void reset() {
        heapIndex = -1;
        distanceTravelled = std::numeric_limits<int>::max();
        previousNode = nullptr;
        usedByMoveable = false;
    }

public:
    int x;
    int y;
    vector<GraphNode*> neighbours;
    vector<int> neighbourCosts;

    //for actual algorithm
    //index in the the graph that includes obstructions, used for graph neighbour linking
    int indexInFullGraph = -1;
    //index in the graph of only useable nodes, used in algorithm
    int indexInGraph = -1;
    int heapIndex = -1;
    bool usedByMoveable = false;

    int distanceTravelled = numeric_limits<int>::max();
    GraphNode* previousNode;
};
