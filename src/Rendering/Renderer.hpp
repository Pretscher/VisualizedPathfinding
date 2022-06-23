#pragma once
#include <iostream>
#include <memory>
using namespace std;
#include <SFML/Graphics.hpp>
class Renderer {
public:
    //Memory of this object has to be externally managed.
    sf::RenderWindow& currentWindow;
    Renderer(sf::RenderWindow& window);
    void drawRect(float x, float y, float width, float height, sf::Color c) const;
    void drawRectOutline(int x, int y, int width, int height, sf::Color c, int thickness) const;
    void drawCircle(int x, int y, int radius, sf::Color c, bool fill, int outlineThickness) const;
    void drawLine(int x1, int y1, int x2, int y2, sf::Color c, int thickness) const;
  
    void getMousePos(int& o_xs, int& o_ys, bool factorInBorders) const;

    void drawRectWithTexture(int x, int y, int width, int height, sf::Texture texture) const;
    sf::Texture loadTexture(string path, bool repeat);

    void drawText(string text, int x, int y, int width, int height, int charSize, sf::Color color) const;

    //SFML always uses the dimensions of window creation, thus this is the amount of pixels your drawing space has on the x axis
    int getPixelCountX() const {
        return xPixels;
    }
    //SFML always uses the dimensions of window creation, thus this is the amount of pixels your drawing space has on the y axis
    int getPixelCountY() const {
        return yPixels;
    }

    sf::RectangleShape createRect(float x, float y, float width, float height, sf::Color c) const;
protected:
    virtual void transform(sf::Transformable& tranformable, float x, float y) const;


     //SFML always uses the dimensions of window creation, which means we only have to save these once in the constructor.
    int xPixels;
    int yPixels;
};