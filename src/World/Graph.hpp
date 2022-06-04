#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
#include "Grid.hpp"

struct GraphNode {
public:
    //Constructor if you do not want to input the average neighbour count (consumes more memory and
    // can be slower  for graphs with many unusable nodes)
    GraphNode(int x, int y) {
        this->x = x;
        this->y = y;
        //in most graphs, most nodes will have around and maximally 8 neighbours. 
        neighbours.reserve(8);
        neighbourCosts.reserve(8);
    }
    //Constructor if you want to input the average neighbour count (consumes less memory and can be faster 
    //for graphs with many unusable nodes)
    GraphNode(int x, int y, int neighbourCountMean) {
        this->x = x;
        this->y = y;
        //in most graphs, most nodes will have around and maximally 8 neighbours. 
        neighbours.reserve(neighbourCountMean);
        neighbourCosts.reserve(neighbourCountMean);
    }

    int getX() const {
        return x;
    }
    int getY() const {
        return y;
    }
    void setX(int x) {
        this->x = x;
    }
    void setY(int y) {
        this->y = y;
    }

    void addNeighbour(int index, int cost) {
        neighbours.push_back(index);
        neighbourCosts.push_back(cost);
    }
private:
    int x;
    int y;
    vector<int> neighbours;
    vector<int> neighbourCosts;
};

class Graph {
private:
    //We want to create a full array of the useable nodes so we do not have to loop over gNodes after creating the graph
    vector<GraphNode*> useableNodes;
    int width, height;
public:
    Graph() {

    }

    void generateFromGrid(Grid& grid) {
        auto startTime = std::chrono::high_resolution_clock::now();

        this->width =  grid.getWidth();
        this->height = grid.getHeight();

        int nodeCount = width * height;
        int useableNodeCount = nodeCount - grid.getNonWhiteCount();

        //In this vector, all nodes that could be useable exist as nullpointers at first, and if they are indeed useable they are initialized and pushed back to the real graph.
        //Because we cannot calculate indices of neighbours in our real graph, we need this graph to check if a node is useable.
        vector<GraphNode*> fullGraph;
        fullGraph.resize(nodeCount);


        //Useable nodes has the same size as the entire gird nodes minus the amount of nodes that are non-white
        useableNodes.reserve(useableNodeCount);
        //calculate how much memory has to be reserved for every node's neighbours on average
        int neighbourCountMean = 8 * ((float)useableNodeCount / nodeCount);

        for(int x = 0; x < this->width; x ++) {
            for(int y = 0; y < this->height; y ++) {
                int nodeIndex = y * this->height + x;
                //Current definition of a useable node: it is white in the grid object
                bool isUseable = grid.getPixel(x, y) == sf::Color::White;
                if(isUseable) {
                    GraphNode* cNode = new GraphNode(x, y, neighbourCountMean);
                    //only put node into array if useable, else nullptr
                    fullGraph[nodeIndex] = cNode;
                    //We want to create a full array of the useable nodes so we do not have to loop over gNodes after creating the graph
                    useableNodes.push_back(cNode);

                    linkNeighbours(nodeIndex, fullGraph);
                }
                //(don't bother finding neighbours for unsuseable nodes because they will not be part of the graph)
            }
        }

        auto timeDiff = std::chrono::high_resolution_clock::now() - startTime;
        std::cout << "Graph of " << useableNodes.size() << " Nodes created in " << (float)timeDiff.count() / 1000000 << "ms.\n";
    }
private:
    inline void linkNeighbours(int nodeIndex, vector<GraphNode*>& fullGraph) {
        trylinkingGraphNodes(nodeIndex, nodeIndex - 1, fullGraph);//left
        trylinkingGraphNodes(nodeIndex, nodeIndex - width, fullGraph);//top
        trylinkingGraphNodes(nodeIndex, nodeIndex - width - 1, fullGraph);//top left
    }

    inline void trylinkingGraphNodes(int node1, int node2, vector<GraphNode*>& fullGraph) {
        if(node1 > 0 && node1 < width * height
                && node2 > 0 && node2 < width * height 
                && fullGraph[node1] != nullptr && fullGraph[node2] != nullptr) {
            fullGraph[node1]->addNeighbour(node2, 1);
            fullGraph[node2]->addNeighbour(node1, 1);
        }
    }
};