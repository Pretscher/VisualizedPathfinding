#include "Renderer.hpp"
#include "Button.hpp"
#include <memory>
class UIManager {
public:
    UIManager(Renderer& i_renderer) : renderer(i_renderer) {
        rightBarWidth = 100;
        topBarHeight = 100;
        buttons.push_back(Button(10, 10, 100, 20, sf::Color(200, 200, 200), renderer.createText("Set Startpoint", 10, 10, 100, 20, 5, sf::Color::Black)));
    }

    vector<int> getGridScreenSpace() {
        return {0, topBarHeight, renderer.getPixelCountX() - rightBarWidth, renderer.getPixelCountY()};
    }

    void draw() {
        drawTopBar();
        drawRigthBar();
        for(Button b : buttons) {
            b.draw(renderer);
        }
    }

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