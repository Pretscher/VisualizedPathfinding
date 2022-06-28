#include "Graph.hpp"
#include "Algorithm.hpp"
#include <memory>
class WorldManager {
private:
    Grid grid;
    Graph graph;
public:
    WorldManager(Grid i_grid) : grid(i_grid) {
        createTestGrid(20);
        graph.generateFromGrid(grid);
    }


    void update() {
        
    }

    Graph& getGraph() {
        return graph;
    }

    Grid& getGrid() {
        return grid;
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