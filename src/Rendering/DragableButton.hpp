#include "Button.hpp"
class DragableButton : public Button {
public:
    DragableButton(int degreesOfFreedom, int initialStateIndex, int x, int y, int width, int height, sf::Color color) : Button(x, y, width, height, color) {
        this->degreesOfFreedom = degreesOfFreedom;
        this->state = initialStateIndex;
        //hardcoded bounds for dragable rect
        int dragableXoffset = getWidth() / 10;
        int dragableYoffset = getHeight() / 2;
        int dragableWidth = getWidth() - dragableXoffset;
        int dragableHeight = getHeight() / 2;
        dragablePanel = sf::RectangleShape(sf::Vector2f(dragableWidth, dragableHeight));
        dragablePanel.setPosition(sf::Vector2f(getX() + dragableXoffset, getY() + dragableYoffset));

        barCount = degreesOfFreedom - 1;
        lineWidth = dragablePanel.getSize().x / (barCount * 5);// divide width by an extra number so that the rects do not touch
        dragableHeightByLineHeight = 0.8f;//factor between box height and line height
        lineHeight = dragablePanel.getSize().y * dragableHeightByLineHeight;
        lineXoffset = (getX() - dragablePanel.getPosition().x) / 2;
        lineYPos = dragablePanel.getPosition().y + (lineHeight * (1 - dragableHeightByLineHeight) / 2);
        for (int i = 0; i < degreesOfFreedom; i++) {
            float lineXPos = dragablePanel.getPosition().x + lineXoffset + ((i * dragablePanel.getSize().x) / barCount) - lineWidth / 2;
            sf::RectangleShape line(sf::Vector2f(lineWidth, lineHeight));
            line.setPosition(lineXPos, lineYPos);

            line.setFillColor(sf::Color::Black);
            lines.push_back(move(line));
        }

        slider = sf::RectangleShape(sf::Vector2f(10, lineHeight));
        slider.setFillColor(sf::Color::Black);
        slider.setPosition(sf::Vector2f(getSliderX(initialStateIndex), lineYPos));

        //difference between lineXpos calculation for lines of 1 and 0.
        spaceBetweenLines = dragablePanel.getPosition().x + lineXoffset + ((dragablePanel.getSize().x) / barCount) - lineWidth / 2 // i = 1 
        - (dragablePanel.getPosition().x + lineXoffset - lineWidth / 2);//i = 0
    }

    void update(Renderer& renderer) override {
        if(wasPressed(renderer)) {
            mouseReleasedAfterPress = true;
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) == false) {
            mouseReleasedAfterPress = false;
        }
        if(mouseReleasedAfterPress == true) {
            changeSliderPosition(renderer.getMousePos(true));
        }
    }

    void changeSliderPosition(sf::Vector2i mousePos) {
        int sliderPos = slider.getPosition().x;
        while(mousePos.x < sliderPos - spaceBetweenLines + (lineWidth / 2)) {
            if(state > 0) {
                state --;
                sliderPos -= spaceBetweenLines;
            } else {
                break;
            }
        }
        while(mousePos.x > sliderPos + spaceBetweenLines - (lineWidth / 2)) {
            if(state < barCount) {
                state ++;
                sliderPos += spaceBetweenLines;
            } else {
                break;
            }
        }
        slider.setPosition(sf::Vector2f(getSliderX(state), lineYPos));
    }

    /**
     * @brief Returns true if the Button was pressed since THE LAST CALL OF THIS FUNCTION
     * SIDE EFFECTS: Changes lastClickedCounter (counter used to distinguish different clicks)
     * @param renderer Needs the renderer and it's implemented mouse tracking functions
     * @return true 
     * @return false 
     */
    bool wasPressed(Renderer& renderer) override {
        auto mousePos = renderer.getLastLeftClick();
        auto size = slider.getSize();
        size.x *= 3;
        size.y *= 3;
        auto pos = slider.getPosition();
        pos.x -= size.x / 3;
        pos.y -= size.y / 3;

        if(mousePos.x > pos.x && mousePos.x < pos.x + size.x 
                && mousePos.y > pos.y && mousePos.y < pos.y + size.y
                && lastClickedCounter != renderer.getClickCounter()) {
            lastClickedCounter = renderer.getClickCounter();
            return true;
        }
        return false;
    }

    void draw(Renderer& renderer) const override {
        renderer.draw(rect);
        renderer.draw(slider);
        renderer.drawText(text, fontName, rect.getPosition().x, rect.getPosition().y, rect.getSize().x, rect.getSize().y / 2, fontSize, textColor);

        for(sf::RectangleShape line : lines) {
            renderer.draw(line);
        }
    }

    /**
     * @brief The relevant output to EventManager is just the current state of the slider.
     * 
     * @return vector<int> 
     */
    vector<int> getData() override {
        return {state};
    }

private:
    float getSliderX(int state) {
        float lineXPos = dragablePanel.getPosition().x + lineXoffset + ((state * dragablePanel.getSize().x) / barCount) - lineWidth / 2;
        //remove offset of line xPos (substracted above for readability) and add own offset
        float sliderXPos = lineXPos + lineWidth / 2 - slider.getSize().x / 2;
        return sliderXPos;
    }
    int state = -1;
    int degreesOfFreedom;

    sf::RectangleShape dragablePanel;
    sf::RectangleShape slider;
    vector<sf::RectangleShape> lines;

    bool mouseReleasedAfterPress = false;
    int barCount;
    float lineWidth, dragableHeightByLineHeight, lineHeight, lineXoffset, lineYPos;
    float spaceBetweenLines;
};