#include "Renderer.hpp"
#include <memory>
class UIManager {
public:
    UIManager(Renderer* i_renderer) {
        this->renderer = i_renderer;
        rightBarWidth = 100;
        topBarHeight = 100;
    }

    vector<int> getGridScreenSpace() {
        return {0, topBarHeight, renderer->getPixelCountX() - rightBarWidth, renderer->getPixelCountY()};
    }

    void draw() {
        drawTopBar();
        drawRigthBar();
    }

private:

    void drawTopBar() {
        renderer->drawRect(0, 0, renderer->getPixelCountX() - rightBarWidth, topBarHeight, sf::Color(100, 100, 100, 255));
    }

    void drawRigthBar() {
        renderer->drawRect(renderer->getPixelCountX() - rightBarWidth, 0, renderer->getPixelCountX(), renderer->getPixelCountY(), sf::Color(100, 100, 100, 255));
    }

    int rightBarWidth;
    int topBarHeight;
    Renderer* renderer;
};