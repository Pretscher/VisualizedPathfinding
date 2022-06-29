#include "Algorithm.hpp"
#include "DrawnAstar.hpp"
class PathfindingManager {
public:
    /**
     * @param i_graph used in algorithm. Edges have to be defined in Graph.
     * @param i_grid only used for drawing 
     */
    PathfindingManager(Graph& i_graph, Grid& i_grid) : graph(i_graph), grid(i_grid), aStar(i_graph), drawnAstar(i_graph, i_grid){

    }

    void update() {
        if(drawingPath == true) {
            int exitState = drawnAstar.getExitState();
            if(exitState == 0) {
                drawingPath = false;
                auto path = drawnAstar.tryRetrievingPath();
                swapPaths(path);//from here on out the path is drawn in drawPath()
            } else if(exitState == -1) {
                drawingPath = false;
            }
        }
    }

    /**
     * @brief For efficiency if you want to skip frames
     * 
     */
    void updatePathAnimationWithoutDrawing() {
        if(drawingPath == true) {
            drawnAstar.updatePathFinding(false);
        }
    }

    void updatePathAnimation() {
        if(drawingPath == true) {
            drawnAstar.updatePathFinding(true);
        }
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
            if(drawAstar == false) {
                swapPaths(aStar.findPath(startX, startY, goalX, goalY));//find this path and instantly draw it completely in drawPath()
            } else {
                drawnAstar.initPathfinding(startX, startY, goalX, goalY);//has to be periodically called because this is an animation
                drawingPath = true;
            }
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

    void swapPaths(vector<Point> newPath) {
        lastPath = move(currentPath);//move because we reassign currentPath now
        currentPath = newPath;
        changedPaths = true;
    }
    Graph& graph;
    Grid& grid;
    Algorithm aStar;

    bool drawAstar = true;
    DrawnAstar drawnAstar;
    bool drawingPath = false;

    vector<Point> currentPath;
    vector<Point> lastPath;
    bool changedPaths = false;

    int startX = -1, startY = -1, goalX = -1, goalY = -1;
    bool newCoordinates = false;
};