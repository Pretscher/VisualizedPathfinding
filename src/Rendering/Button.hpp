#include "Renderer.hpp"

class Button {
public:
    Button(int x, int y, int width, int height, sf::Color color) : rect(sf::Vector2f(width, height)) {
        setX(x); 
        setY(y);
        setColor(color);
    }

    int getX() const {
        return rect.getPosition().x;
    }
    int getY() const {
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
    void setText(string text, string fontName, sf::Color textColor, int fontSize) {
        this->text = text; this->fontName = fontName; this->textColor = textColor; this->fontSize = fontSize;
    }

    void draw(Renderer& renderer) const {
        renderer.draw(rect);
        renderer.drawText(text, fontName, rect.getPosition().x, rect.getPosition().y, rect.getSize().x, rect.getSize().y, fontSize, textColor);
    }

    /**
     * @brief Returns true if the Button is currently pressed.
     * 
     * @param renderer Needs the renderer and it's implemented mouse tracking functions
     * @return true 
     * @return false 
     */
    bool isPressed(Renderer& renderer) const {
        auto mousePos = renderer.getMousePos(true);
        auto pos = rect.getPosition();
        auto size = rect.getSize();
        if(mousePos.x > pos.x && mousePos.x < pos.x + size.x 
            && mousePos.y > pos.y && mousePos.y < pos.y + size.y
            && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            return true;
        }
        return false;
    }

    /**
     * @brief Returns true if the Button was pressed since THE LAST CALL OF THIS FUNCTION
     * SIDE EFFECTS: Changes lastClickedCounter (counter used to distinguish different clicks)
     * @param renderer Needs the renderer and it's implemented mouse tracking functions
     * @return true 
     * @return false 
     */
    bool wasPressed(Renderer& renderer) {
        auto mousePos = renderer.getLastLeftClick();
        auto pos = rect.getPosition();
        auto size = rect.getSize();
        if(mousePos.x > pos.x && mousePos.x < pos.x + size.x 
                && mousePos.y > pos.y && mousePos.y < pos.y + size.y
                && lastClickedCounter != renderer.getClickCounter()) {
            lastClickedCounter = renderer.getClickCounter();
            return true;
        }
        return false;
    }
private:
    int lastClickedCounter = -1;//click counter of last click that pressed this button
    sf::RectangleShape rect;
    
    string text, fontName;
    sf::Color textColor;
    int fontSize;

};