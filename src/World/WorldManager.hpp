#include "Graph.hpp"
#include "Algorithm.hpp"
class WorldManager {
public:
    Grid* grid;
    Graph* g;
    Algorithm* algorithm;
    WorldManager() {
        grid = new Grid(192, 108, sf::Color::White);
        g = new Graph();
        createTestGrid(100);
        g->generateFromGrid(*grid);

        algorithm = new Algorithm(g);
    }
    ~WorldManager() {
        delete grid;
        delete g;
        delete algorithm;
    }

    void update() {
        
    }

    vector<Point> findPath(int startX, int startY, int goalX, int goalY) {
        return algorithm->findPath(startX, startY, goalX, goalY);
    }

    void addPathToGrid(vector<Point> path) {
        for(int i = 0; i < path.size(); i++) {
            sf::Color lightGreen = sf::Color(40, 100, 3);
            grid->setPixel(path[i].x, path[i].y, lightGreen);
        }
    }
    void removePathFromGrid(vector<Point> path) {
        for(int i = 0; i < path.size(); i++) {
            grid->setPixel(path[i].x, path[i].y, sf::Color::White);
        }
    }

    void draw(Renderer* renderer, vector<int>&& gridScreenSpace) {
        grid->draw(renderer, move(gridScreenSpace));
    }

private:
    void createTestGrid(int randomDegree) {
        for(int x = 0; x < grid->getWidth(); x ++) {
            for(int y = 0; y < grid->getHeight(); y ++) {
                int random_variable = std::rand() % randomDegree;         
                if(random_variable == 0) {
                    grid->setPixel(x, y, sf::Color::Black);
                }
            }
        }
    }
};