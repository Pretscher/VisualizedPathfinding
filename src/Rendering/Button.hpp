#pragma once
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

    int getWidth() const {
        return rect.getSize().x;
    }

    int getHeight() const {
        return rect.getSize().y;
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

    virtual void draw(Renderer& renderer) const {
        renderer.draw(rect);
        renderer.drawText(text, fontName, rect.getPosition().x, rect.getPosition().y, rect.getSize().x, rect.getSize().y, fontSize, textColor);
    }

    /**
     * @brief 
     * 
     * @param renderer needed for mouse Events
     */
    virtual void update(Renderer& renderer) {
        
    }
    /**
     * @brief Can be overridden to return some derived-class specific data. Base class just returns an empty vector.
     * 
     */
    virtual vector<int> getData() {
        return {};
    }

    /**
     * @brief Returns true if the Button is currently pressed.
     * 
     * @param renderer Needs the renderer and it's implemented mouse tracking functions
     * @return true 
     * @return false 
     */
    virtual bool isPressed(Renderer& renderer) {
        auto mousePos = renderer.getMousePos(true);
        auto pos = rect.getPosition();
        auto size = rect.getSize();
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) 
            && mousePos.x > pos.x && mousePos.x < pos.x + size.x 
            && mousePos.y > pos.y && mousePos.y < pos.y + size.y
            ) {
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
    virtual bool wasPressed(Renderer& renderer) {
        auto mousePos = renderer.getLastFinishedLeftClick();
        auto pos = rect.getPosition();
        auto size = rect.getSize();
        if(mousePos.x > pos.x && mousePos.x < pos.x + size.x 
                && mousePos.y > pos.y && mousePos.y < pos.y + size.y
                && lastClickedCounter != renderer.getFinishedClickCounter()) {
            lastClickedCounter = renderer.getFinishedClickCounter();
            return true;
        }
        return false;
    }
protected:
    int lastClickedCounter = -1;//click counter of last click that pressed this button
    sf::RectangleShape rect;
    
    string text, fontName;
    sf::Color textColor;
    int fontSize;

};