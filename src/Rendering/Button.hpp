#include "Renderer.hpp"

class Button {
public:
    Button(int x, int y, int width, int height) : rect(sf::Vector2f(width, height)) {
        setX(x); setY(y);
    }

    int getX() {
        return rect.getPosition().x;
    }

    int getY() {
        return rect.getPosition().y;
    }

    void setX(int x) {
        rect.setPosition(sf::Vector2f(x, getY()));
    }
    void setY(int y) {
        rect.setPosition(sf::Vector2f(getX(), y));
    }

private:
    sf::RectangleShape rect = sf::RectangleShape();
};