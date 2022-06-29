#include "Renderer.hpp"
#include "Button.hpp"
#include "DragableButton.hpp"
#include <memory>
#include <map>

class UIManager {
public:
    UIManager(Renderer& i_renderer) : renderer(i_renderer) {
        rightBarWidth = 100;
        topBarHeight = 100;
        
        //hard coded buttons, ALSO ACCESSED IN EVENTMANAGER
        buttons["select start node"] = new Button(0, 0, 200, 100, sf::Color(200, 200, 200));
        buttons["select start node"]->setText("Set start", "calibri", sf::Color::Black, 30);
        
        buttons["select goal node"] = new Button(200, 0, 200, 100, sf::Color(200, 200, 200));
        buttons["select goal node"]->setText("Set goal", "calibri", sf::Color::Black, 30);

        buttons["start pathfinding"] = new Button(1670, 0, 250, 100, sf::Color(200, 200, 200));
        buttons["start pathfinding"]->setText("Start Pathfinding", "calibri", sf::Color::Black, 30);

        buttons["speed"] = new DragableButton(15, 14, 1200, 0, 250, 100, sf::Color(200, 200, 200));
        buttons["speed"]->setText("Speed", "calibri", sf::Color::Black, 30);
    }

    ~UIManager() {
        //delete Button pointers
        for (auto const& entry : buttons) {
            delete entry.second;
        }
    }

    vector<int> getGridScreenSpace() {
        return {0, topBarHeight, renderer.getPixelCountX() - rightBarWidth, renderer.getPixelCountY()};
    }

    void update() {
        for (auto const& entry : buttons) {
            entry.second->update(renderer);
        }
    }

    void draw() {
        drawTopBar();
        drawRigthBar();
        for (auto const& entry : buttons) {
            entry.second->draw(renderer);
        }
    }

    int startNodeX, startNodeY, goalNodeX, goalNodeY;

    map<string, Button*> buttons;
private:
    void drawTopBar() {
        renderer.drawRect(0, 0, renderer.getPixelCountX() - rightBarWidth, topBarHeight, sf::Color(100, 100, 100, 255));
    }

    void drawRigthBar() {
        renderer.drawRect(renderer.getPixelCountX() - rightBarWidth, 0, renderer.getPixelCountX(), renderer.getPixelCountY(), sf::Color(100, 100, 100, 255));
    }

    int rightBarWidth;
    int topBarHeight;
    Renderer& renderer;
};