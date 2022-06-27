#pragma once
#include <iostream>
#include <memory>
using namespace std;
#include <SFML/Graphics.hpp>
#include <map>
class Renderer {
public:
    sf::RenderWindow& currentWindow;
    Renderer(sf::RenderWindow& window);
    void drawRect(float x, float y, float width, float height, sf::Color c) const;
    void drawRectOutline(int x, int y, int width, int height, sf::Color c, int thickness) const;
    void drawCircle(int x, int y, int radius, sf::Color c, bool fill, int outlineThickness) const;
    void drawLine(int x1, int y1, int x2, int y2, sf::Color c, int thickness) const;
  
    sf::Vector2i getMousePos(bool factorInBorders);

    void drawRectWithTexture(int x, int y, int width, int height, sf::Texture texture) const;
    sf::Texture loadTexture(string path, bool repeat);

    void drawText(string text, string font, int x, int y, int width, int height, int charSize, sf::Color color);

    //SFML always uses the dimensions of window creation, thus this is the amount of pixels your drawing space has on the x axis
    int getPixelCountX() const {
        return xPixels;
    }
    //SFML always uses the dimensions of window creation, thus this is the amount of pixels your drawing space has on the y axis
    int getPixelCountY() const {
        return yPixels;
    }

    sf::RectangleShape createRect(float x, float y, float width, float height, sf::Color c) const;
    void draw(sf::Drawable& drawable) {
        currentWindow.draw(drawable);
    }
    
    void update() {
        updateMouseState();
    }

    sf::Vector2i getLastLeftClick() {
        return lastClickPos;
    }

    /**
     * @brief Every external object that wants to only be clicked once per mouseclick has to save the last clickCounter 
    and ignore the mouse click event if the click counter is the same.
     * 
     * @return int 
     */
    int getClickCounter() {
        return clickCounter;
    }

protected:
    void updateMouseState() {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) == true && clickComplete == true) { //mouse currently pressed => current click is not complete
            clickComplete = false;
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) == false && clickComplete == false) {
            clickComplete = true;
            clickCounter ++;
            lastClickPos = getMousePos(true);//save mouse coords to last click positions
        }
    }
    sf::Vector2i lastClickPos;
    sf::Vector2i lastMousePos;//for if the mouse leaves the window
    bool clickComplete = true;
    int clickCounter = 0;
    virtual void transform(sf::Transformable& tranformable, float x, float y) const;


    //SFML always uses the dimensions of window creation, which means we only have to save these once in the constructor.
    int xPixels;
    int yPixels;

    sf::Font& getFont(string fontName);
    map<string, sf::Font> loadedFonts;
};