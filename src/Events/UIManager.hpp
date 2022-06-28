#include "Renderer.hpp"
#include "Button.hpp"
#include <memory>
#include <map>

class UIManager {
public:
    UIManager(Renderer& i_renderer) : renderer(i_renderer) {
        rightBarWidth = 100;
        topBarHeight = 100;
        
        //hard coded buttons, ALSO ACCESSED IN EVENTMANAGER
        buttons["Start"] = new Button(0, 0, 200, 100, sf::Color(200, 200, 200));
        buttons["Start"]->setText("Set start", "calibri", sf::Color::Black, 30);
        
        buttons["Goal"] = new Button(200, 0, 200, 100, sf::Color(200, 200, 200));
        buttons["Goal"]->setText("Set goal", "calibri", sf::Color::Black, 30);
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