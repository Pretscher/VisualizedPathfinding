#include "Algorithm.hpp"
class PathfindingManager {
public:
    /**
     * @param i_graph used in algorithm. Edges have to be defined in Graph.
     * @param i_grid only used for drawing 
     */
    PathfindingManager(Graph& i_graph, Grid& i_grid) : graph(i_graph), algorithm(i_graph), grid(i_grid) {

    }

    void drawPath() {
        //if path was changed, erase last path and insert new path
        if(changedPaths == true) {
            removePathFromGrid(lastPath);
            changedPaths = false;
            addPathToGrid(currentPath);
        }
    }

    void addPathToGrid(vector<Point>& path) {
        for(int i = 0; i < path.size(); i++) {
            sf::Color lightGreen = sf::Color(40, 100, 3);
            grid.setPixel(path[i].x, path[i].y, lightGreen);
        }
    }

    void removePathFromGrid(vector<Point>& path) {
        for(int i = 0; i < path.size(); i++) {
            grid.setPixel(path[i].x, path[i].y, sf::Color::White);
        }
    }

    void tryFindingPath() {
        if(startX != -1 && goalX != -1 && newCoordinates == true) {
            newCoordinates = false;//dont find the same path twice
            lastPath = move(currentPath);//move because we reassign currentPath now
            changedPaths = true;
            currentPath = algorithm.findPath(startX, startY, goalX, goalY);
        }
    }

    void setStartPoint(int x, int y) {
        //reset color of old pixel
        if(startX != -1) {
            grid.setPixel(startX, startY, sf::Color::White);
        }
        startX = x; startY = y;
        newCoordinates = true;
        //set color of new pixel
        grid.setPixel(x, y, sf::Color::Green);
    }

    void setGoalPoint(int x, int y) {
        //reset color of old pixel
        if(goalX != -1) {
            grid.setPixel(goalX, goalY, sf::Color::White);
        }
        goalX = x; goalY = y;
        newCoordinates = true;
        grid.setPixel(x, y, sf::Color::Red);
    }
private:
    Graph& graph;
    Grid& grid;
    Algorithm algorithm;
    vector<Point> currentPath;
    
    vector<Point> lastPath;
    bool changedPaths = false;

    int startX = -1, startY = -1, goalX = -1, goalY = -1;
    bool newCoordinates = false;
};