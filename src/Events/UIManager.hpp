#include "Renderer.hpp"

class UIManager {
public:
    UIManager(Renderer* i_renderer) {
        this->renderer = i_renderer;
    }

    vector<int> getGridScreenSpace() {
        return {0, topBarY, rightBarX, renderer->getPixelCountY()};
    }

    void draw(Renderer* renderer) {

    }

private:
    int rightBarX = 1900;
    int topBarY = 100;
    Renderer* renderer;
};