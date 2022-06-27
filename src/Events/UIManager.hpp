#include "Renderer.hpp"
#include "Button.hpp"
#include <memory>
#include <map>

class UIManager {
public:
    UIManager(Renderer& i_renderer) : renderer(i_renderer) {
        rightBarWidth = 100;
        topBarHeight = 100;
        buttons.push_back(Button(0, 0, 200, 100, sf::Color(200, 200, 200)));
        buttons[0].setText("Set start", "calibri", sf::Color::Black, 30);
        
        buttons.push_back(Button(200, 0, 200, 100, sf::Color(200, 200, 200)));
        buttons[1].setText("Set goal", "calibri", sf::Color::Black, 30);
    }

    vector<int> getGridScreenSpace() {
        return {0, topBarHeight, renderer.getPixelCountX() - rightBarWidth, renderer.getPixelCountY()};
    }

    void update() {
        if(buttons[0].wasPressed(renderer) == true) {
            
        }
    }

    void draw() {
        drawTopBar();
        drawRigthBar();
        for(Button b : buttons) {
            b.draw(renderer);
        }
    }

    int startNodeX, startNodeY, goalNodeX, goalNodeY;
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
    vector<Button> buttons;
};