#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
#include "Grid.hpp"

struct GraphNode {
public:
    //Constructor if you do not want to input the average neighbour count (consumes more memory and
    // can be slower  for graphs with many unusable nodes)
    GraphNode(int x, int y, int indexInGraph) {
        this->x = x;
        this->y = y;
        this->indexInGraph = indexInGraph;
        //in most graphs, most nodes will have around and maximally 8 neighbours. 
        neighbours.reserve(8);
        neighbourCosts.reserve(8);
    }
    //Constructor if you want to input the average neighbour count (consumes less memory and can be faster 
    //for graphs with many unusable nodes)
    GraphNode(int x, int y, int neighbourCountMean, int indexInGraph) {
        this->x = x;
        this->y = y;
        this->indexInGraph = indexInGraph;
        //in most graphs, most nodes will have around and maximally 8 neighbours. 
        neighbours.reserve(neighbourCountMean);
        neighbourCosts.reserve(neighbourCountMean);
    }

    inline void addNeighbour(GraphNode* node, int cost) {
        neighbours.push_back(node);
        neighbourCosts.push_back(cost);
    }


    inline void reset() {
        indexInGraph = -1;
        heapIndex = -1;
        distanceTravelled = 0;
        previousNode = nullptr;
        usedByMoveable = false;
    }

public:
    int x;
    int y;
    vector<GraphNode*> neighbours;
    vector<int> neighbourCosts;

    //for actual algorithm
    int indexInGraph = -1;
    int heapIndex = -1;
    bool usedByMoveable = false;

    int distanceTravelled = 0;
    GraphNode* previousNode;
};

class Graph {
private:
    //We want to create a full array of the useable nodes so we do not have to loop over gNodes after creating the graph
    vector<GraphNode*> fullGraph;
    int width, height;
public:
    vector<GraphNode*> nodes;
    Graph() {
        
    }
    ~Graph() {
        //delete useableNodes (all other nodes stayed nullptrs in "fullGraph")
        for(int i = 0; i < nodes.size(); i++) {
            delete nodes[i];
        }
    }
    void generateFromGrid(Grid& grid) {
        auto startTime = std::chrono::high_resolution_clock::now();

        this->width =  grid.getWidth();
        this->height = grid.getHeight();

        int nodeCount = width * height;
        int useableNodeCount = nodeCount - grid.getNonWhiteCount();

        //In this vector, all nodes that could be useable exist as nullpointers at first, and if they are indeed useable they are initialized and pushed back to the real graph.
        //Because we cannot calculate indices of neighbours in our real graph, we need this graph to check if a node is useable.

        fullGraph.resize(nodeCount);


        //"nodes" has the same size as the entire gird nodes minus the amount of nodes that are non-white
        nodes.reserve(useableNodeCount);
        //calculate how much memory has to be reserved for every node's neighbours on average
        int neighbourCountMean = 8 * ((float)useableNodeCount / nodeCount);

        for(int x = 0; x < this->width; x ++) {
            for(int y = 0; y < this->height; y ++) {
                int nodeIndex = y * this->height + x;
                //Current definition of a useable node: it is white in the grid object
                bool isUseable = grid.getPixel(x, y) == sf::Color::White;
                if(isUseable) {
                    GraphNode* cNode = new GraphNode(x, y, neighbourCountMean, nodes.size());
                    //only put node into array if useable, else nullptr
                    fullGraph[nodeIndex] = cNode;
                    //We want to create a full array of the useable nodes so we do not have to loop over gNodes after creating the graph
                    nodes.push_back(cNode);
                    linkNeighbours(cNode);
                }
                //(don't bother finding neighbours for unsuseable nodes because they will not be part of the graph)
            }
        }

        auto timeDiff = std::chrono::high_resolution_clock::now() - startTime;
        std::cout << "Graph of " << nodes.size() << " Nodes created in " << (float)timeDiff.count() / 1000000 << "ms.\n";
    }

    inline void setHeapIndex(int graphNodeIndex, int heapIndex) {
        nodes[graphNodeIndex]->heapIndex = heapIndex;
    }

    inline int getHeapIndex(int graphNodeIndex) {
        return nodes[graphNodeIndex]->heapIndex;
    }

    inline void reset() {
        for (int i = 0; i < nodes.size(); i++) {
            nodes[i]->reset();
        }
    }

    inline GraphNode* getIndexFromCoords(int x, int y) {
        GraphNode* node = fullGraph[y * width + x];
        #if DEBUG
        if(node == nullptr) {
            std::cout << "Error in 'getIndexFromCoords': GraphNode at
                coordinates x = " << x << ", y = " << y << " does not exist.\n";
                std::exit(0);
        }
        #endif
        return node;
    }


private:
    inline void linkNeighbours(GraphNode* node) {
        int nodeIndex = node->indexInGraph;
        if(nodeIndex - 1 >= 0) {
            trylinkingGraphNodes(node, fullGraph[nodeIndex - 1]);//left
        }
        if(nodeIndex - width >= 0) {
            trylinkingGraphNodes(node, fullGraph[nodeIndex - width]);//top
        }
        if(nodeIndex - width - 1 >= 0) {
            trylinkingGraphNodes(node, fullGraph[nodeIndex - width - 1]);//top left
        }
    }

    inline void trylinkingGraphNodes(GraphNode* node1, GraphNode* node2) {
        //this is not a safety measure, this case WILL OCCUR because if the node is a nullptr it is not useable.
        if(node1 != nullptr && node2 != nullptr) {
            node1->addNeighbour(node2, 1);
            node2->addNeighbour(node1, 1);
        }
    }
};