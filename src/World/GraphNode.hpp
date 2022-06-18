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
        heapIndex = -1;
        indexInFullGraph = -1;
        //in most graphs, most nodes will have around and maximally 8 neighbours. 
        neighbours.reserve(8);
        neighbourCosts.reserve(8);
    }
    //Constructor if you want to input the average neighbour count (consumes less memory and can be faster 
    //for graphs with many unusable nodes)
    GraphNode(int x, int y, int neighbourCountMean, int i_indexInUseableGraph, int i_indexInFullGraph) {
        this->x = x;
        this->y = y;
        heapIndex = -1;
        this->indexInGraph = i_indexInUseableGraph;
        this->indexInFullGraph = i_indexInFullGraph;
        //in most graphs, most nodes will have around and maximally 8 neighbours. 
        neighbours.reserve(neighbourCountMean);
        neighbourCosts.reserve(neighbourCountMean);
    }

    GraphNode() {

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

    inline void setHeapIndex(int heapIndex) {
        this->heapIndex = heapIndex;
    }

    inline int getHeapIndex() const {
        return heapIndex;
    }

    inline int getX() const {
        return x;
    }

    inline int getY() const {
        return y;
    }


    inline const GraphNode* const getPreviousNode() const {
        return previousNode;
    }

    inline void setPreviousNode(GraphNode* previousNode) {
        this->previousNode = previousNode;
    }

    inline int getDistanceTravelled() const {
        return distanceTravelled;
    }

    inline void setDistanceTravelled(int distanceTravelled) {
        this->distanceTravelled = distanceTravelled;
    }  
    /**
     * @brief Get the Index In the Graph that only contains nodes that are marked as "walkable" for the pathfinding
     * 
     * @return int 
     */
    inline int getIndexInAlgorithmGraph() const {
        return indexInGraph;
    }

         /**
     * @brief Get the Index In the Graph that also contains nodes that are not walkable for the algorithm
     *  (they are most probably not properly initialized to save performance though)
     * 
     * @return int 
     */
    inline int getIndexInFullGraph() const {
        return indexInFullGraph;
    }

    inline const vector<GraphNode*>& getNeighbours() const {
        return neighbours;
    }

    inline int getNeighbourCosts(int neighbourIndex) {
       return neighbourCosts[neighbourIndex];
    }

    inline void setNeighbourCosts(int neighbourIndex, int costs) {
        neighbourCosts[neighbourIndex] = costs;
    }

    inline bool isUsedByMoveable() {
        return usedByMoveable;
    }

    inline void setUsedByMoveable(bool usedByMoveable) {
        this->usedByMoveable = usedByMoveable;
    }
private:
    int x;
    int y;
    vector<GraphNode*> neighbours;
    vector<int> neighbourCosts;

    //for actual algorithm
    //index in the the graph that includes obstructions, used for graph neighbour linking
    int indexInFullGraph;
    //index in the graph of only useable nodes, used in algorithm
    int indexInGraph;
    int heapIndex;
    bool usedByMoveable = false;

    int distanceTravelled = numeric_limits<int>::max();
    GraphNode* previousNode;
};
