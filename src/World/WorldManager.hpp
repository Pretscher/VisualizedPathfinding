#include "Graph.hpp"
#include "Algorithm.hpp"
#include <memory>
class WorldManager {
public:
    Grid grid;
    unique_ptr<Graph> g;
    unique_ptr<Algorithm> algorithm;
    WorldManager(Grid i_grid) : grid(i_grid) {
        g = make_unique<Graph>();
        createTestGrid(100);
        g->generateFromGrid(grid);

        algorithm = make_unique<Algorithm>(*g);
    }

    void update() {
        
    }

    vector<Point> findPath(int startX, int startY, int goalX, int goalY) {
        return algorithm->findPath(startX, startY, goalX, goalY);
    }

    void addPathToGrid(vector<Point> path) {
        for(int i = 0; i < path.size(); i++) {
            sf::Color lightGreen = sf::Color(40, 100, 3);
            grid.setPixel(path[i].x, path[i].y, lightGreen);
        }
    }
    void removePathFromGrid(vector<Point> path) {
        for(int i = 0; i < path.size(); i++) {
            grid.setPixel(path[i].x, path[i].y, sf::Color::White);
        }
    }

    void draw(vector<int>&& gridScreenSpace) {
        grid.draw();
    }

private:
    void createTestGrid(int randomDegree) {
        for(int y = 0; y < grid.getRows(); y ++) {
            for(int x = 0; x < grid.getCols(); x ++) {
                int random_variable = std::rand() % randomDegree;         
                    if(random_variable == 0) {
                        grid.setPixel(x, y, sf::Color::Black);
                    }
                }
            }
        }
};