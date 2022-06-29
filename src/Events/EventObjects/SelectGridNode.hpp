#include "EventObjects/Event.hpp"
class SelectGridNode : public Event {
public:
    SelectGridNode(Renderer& i_renderer, Grid& i_grid) : Event(i_renderer), grid(i_grid) {
        initialClickCounter = i_renderer.getFinishedClickCounter();
    }

    void update() override {
        if(initialClickCounter != renderer.getFinishedClickCounter()) {
            auto pos = renderer.getLastFinishedLeftClick();
            auto dimensions = grid.getScreenDimensions();//[0] := xOffset, [1] := yOffset, [2] := width, [3] := height
            auto gridNodeSize = grid.getNodeSize();
            //if click is actually in gird
            if(pos.x > grid.getScreenDimensions()[0] && pos.x < grid.getScreenDimensions()[0] + grid.getScreenDimensions()[2]
            && pos.y > grid.getScreenDimensions()[1] && pos.y < grid.getScreenDimensions()[1] + grid.getScreenDimensions()[3]) {
                //get position in grid
                int x = (pos.x - dimensions[0]) / gridNodeSize.x;
                int y = (pos.y - dimensions[1]) / gridNodeSize.y;
                
                if(grid.getNode(x, y).getFillColor() != sf::Color::Black) { //check if grid node is useable
                    finished = true;
                    selectedNodeX = x;
                    selectedNodeY = y;
                } else {
                    cout << "Selected Node must be non-black. \n";
                    initialClickCounter = renderer.getFinishedClickCounter();//wait for next click
                }
            }
        }
    }

    void draw() const override {
        auto gridNodeSize = grid.getNodeSize();
        auto mousePos = renderer.getMousePos(true);
        auto dimensions = grid.getScreenDimensions();
        //"Round down" to last gridNode position
        if(mousePos.x > grid.getScreenDimensions()[0] && mousePos.x < grid.getScreenDimensions()[0] + grid.getScreenDimensions()[2]
        && mousePos.y > grid.getScreenDimensions()[1] && mousePos.y < grid.getScreenDimensions()[1] + grid.getScreenDimensions()[3]) {
            int x = mousePos.x / gridNodeSize.x;
            int y = mousePos.y / gridNodeSize.y;
            renderer.drawRect(x * gridNodeSize.x, grid.getScreenDimensions()[1], gridNodeSize.x, grid.getScreenDimensions()[3], sf::Color(0, 0, 0, 100));
            renderer.drawRect(grid.getScreenDimensions()[0], y * gridNodeSize.y, grid.getScreenDimensions()[2], gridNodeSize.y, sf::Color(0, 0, 0, 100));
        }
    }

    vector<int> getData() const override {
        return {selectedNodeX, selectedNodeY};
    }

private:
    int initialClickCounter;
    int selectedNodeX = -1, selectedNodeY = -1;
    Grid& grid;
};