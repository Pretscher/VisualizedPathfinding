#include "EventObjects/Event.hpp"
class SelectGridNode : public Event {
public:
    SelectGridNode(Renderer& i_renderer, Grid& i_grid) : Event(i_renderer), grid(i_grid) {
        initialClickCounter = i_renderer.getClickCounter();
    }

    void update() override {
        if(initialClickCounter != renderer.getClickCounter()) {
            auto pos = renderer.getLastLeftClick();
            auto dimensions = grid.getScreenDimensions();
            auto gridNodeSize = grid.getNodeSize();

            int x = (pos.x - dimensions[0]) / gridNodeSize.x;
            int y = (pos.y - dimensions[1]) / gridNodeSize.y;
            selectedNode = grid.getNode(x, y);
            
            if(selectedNode.getFillColor() != sf::Color::Black) {
                finished = true;
            } else {
                cout << "Selected Node must be non-black. \n";
                initialClickCounter = renderer.getClickCounter();//wait for next click
            }
        }
    }

    bool isFinished() const override  {
        return finished;
    }

    void draw() const override {
        auto gridNodeSize = grid.getNodeSize();
        auto mousePos = renderer.getMousePos(true);
        //"Round down" to last gridNode position
        int x = mousePos.x / gridNodeSize.x;
        int y = mousePos.y / gridNodeSize.y;
        renderer.drawRect(x, grid.getScreenDimensions()[1], gridNodeSize.x, grid.getScreenDimensions()[3], sf::Color::Black);
        renderer.drawRect(grid.getScreenDimensions()[0], y, grid.getScreenDimensions()[2], gridNodeSize.y, sf::Color::Black);
    }

private:
    int initialClickCounter;
    bool finished;
    sf::RectangleShape selectedNode;
    Grid& grid;
};