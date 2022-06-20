#include "Renderer.hpp"

class Button {
public:
    Button(int x, int y) {
        rect = make_unique<sf::RectangleShape>(x, y);
    }

private:
    unique_ptr<sf::RectangleShape> rect;
};