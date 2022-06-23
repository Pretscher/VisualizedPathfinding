#include "Renderer.hpp"

class Button {
public:
    Button(int x, int y, int width, int height, sf::Color color, sf::Text i_text) : rect(sf::Vector2f(width, height)), text(i_text) {
        setX(x); 
        setY(y);
        setColor(color);
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
    void setColor(sf::Color rgba) {
        rect.setFillColor(rgba);
    }
    void setText(sf::Text&& text) {
        this->text = text;
    }

    void draw(Renderer& renderer) {
        renderer.currentWindow.draw(rect);
        renderer.currentWindow.draw(text);
    }

private:
    sf::RectangleShape rect;
    sf::Text text;
};