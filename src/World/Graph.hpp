#pragma once
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

#include "GraphNode.hpp"
#include "Grid.hpp"

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

        this->width =  grid.getCols();
        this->height = grid.getRows();

        int nodeCount = width * height;
        int useableNodeCount = nodeCount - grid.getNonWhiteCount();

        //In this vector, all nodes that could be useable exist as nullpointers at first, and if they are indeed useable they are initialized and pushed back to the real graph.
        //Because we cannot calculate indices of neighbours in our real graph, we need this graph to check if a node is useable.

        fullGraph.resize(nodeCount);


        //"nodes" has the same size as the entire gird nodes minus the amount of nodes that are non-white
        nodes.reserve(useableNodeCount);
        //calculate how much memory has to be reserved for every node's neighbours on average
        int neighbourCountMean = 8 * ((float)useableNodeCount / nodeCount);
        for(int y = 0; y < this->height; y ++) {
            for(int x = 0; x < this->width; x ++) {
                int nodeIndex = x + y * this->width;
                //Current definition of a useable node: it is white in the grid object
                bool isUseable = grid.getPixelColor(x, y) == sf::Color::White;
                if(isUseable) {
                    GraphNode* cNode = new GraphNode(x, y, neighbourCountMean, nodes.size(), nodeIndex);
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

    inline void reset() {
        for (int i = 0; i < nodes.size(); i++) {
            nodes[i]->reset();
        }
    }

    inline GraphNode* getIndexFromCoords(int x, int y) {
        return fullGraph[y * width + x];
    }

    void setIndexInHeap(int graphNodeIndex, int heapNodeIndex) {
        nodes[graphNodeIndex]->setHeapIndex(heapNodeIndex);
    }
    
private:
    inline void linkNeighbours(GraphNode* node) {
        int nodeIndex = node->getIndexInFullGraph();
        int newIndex =  nodeIndex - 1;
        if(node->getX() > 0 && newIndex >= 0) {
            trylinkingGraphNodes(node, fullGraph[newIndex]);//left
        }
        newIndex = nodeIndex - width;
        if(node->getX() > 0 && newIndex >= 0) {
            trylinkingGraphNodes(node, fullGraph[newIndex]);//top
        }
        newIndex = nodeIndex - width - 1;
        if(node->getX() > 0 && newIndex >= 0) {
            //prevent diagonal links if there is a line of nodes between the two nodes to link
            if(fullGraph[newIndex + 1] != nullptr//above node, right to newnode
            && fullGraph[nodeIndex - 1] != nullptr) {//left to node, under newnode
                trylinkingGraphNodes(node, fullGraph[newIndex]);//top left
            }
        }
        newIndex = nodeIndex - width + 1;
        if(newIndex >= 0 && ((newIndex / width) == ((nodeIndex / width) - 1))) {//check if not in the same row
            //prevent diagonal links if there is a line of nodes between the two nodes to link
            if(fullGraph[newIndex - 1] != nullptr//above node, left to newnode
            && fullGraph[nodeIndex + 1] != nullptr) {//right to node, under newnode
                trylinkingGraphNodes(node, fullGraph[newIndex]);//top left
            }
        }
    }

    /**
     * @brief Can be called with nullptr nodes, it is actually part of the algorithm to do that.
     * 
     * @param node1 
     * @param node2 
     */
    inline void trylinkingGraphNodes(GraphNode* node1, GraphNode* node2) {
        //this is not a safety measure, this case WILL OCCUR because if the node is a nullptr it is not useable.
        if(node1 != nullptr && node2 != nullptr) {
            node1->addNeighbour(node2, 1);
            node2->addNeighbour(node1, 1);
        }
    }

};