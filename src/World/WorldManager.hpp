#include "Graph.hpp"
class WorldManager {
public:
    Grid* grid;
    Graph* g;
    WorldManager() {
        grid = new Grid(1920, 1080, sf::Color::White);
        g = new Graph();
        createTestGrid();
        g->generateFromGrid(*grid);
    }
    ~WorldManager() {
        delete grid;
        delete g;
    }

    void update() {
        
    }

    void draw(Renderer* renderer) {
        grid->draw(renderer);
    }

private:
    void createTestGrid() {
        for(int x = 0; x < grid->getWidth(); x ++) {
            for(int y = 0; y < grid->getHeight(); y ++) {
                int random_variable = std::rand() % 2;         
                if(random_variable == 1) {
                    grid->setPixel(x, y, sf::Color::Black);
                }
            }
        }
    }
};