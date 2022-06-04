#pragma once
#include <iostream>
#include <memory>
using namespace std;
#include <SFML/Graphics.hpp>
class Renderer {
public:
    //Memory of this object has to be externally managed.
    sf::RenderWindow* currentWindow;
    Renderer(sf::RenderWindow* window);

    void drawRect(int x, int y, int width, int height, sf::Color c);
    void drawRectOutline(int x, int y, int width, int height, sf::Color c, int thickness);
    void drawCircle(int x, int y, int radius, sf::Color c, bool fill, int outlineThickness);
    void drawLine(int x1, int y1, int x2, int y2, sf::Color c, int thickness);
  
    void getMousePos(int& o_xs, int& o_ys, bool factorInBorders);

    void drawRectWithTexture(int x, int y, int width, int height, sf::Texture texture);
    sf::Texture loadTexture(string path, bool repeat);

    void drawText(string text, int x, int y, int width, int height, int charSize, sf::Color color);

protected:
    virtual void transform(sf::Transformable& tranformable, int x, int y);

    const int normalResXs = 1920;
    const int normalResYs = 1080;

    void fromYX(int& ioY, int& ioX) {
        auto size = Renderer::currentWindow->getSize();
        ioY = ((float) ioY / normalResYs) * size.y;
        ioX = ((float) ioX / normalResXs) * size.x;
    }

    void fromYXBounds(int& ioW, int& ioH) {
        auto size = Renderer::currentWindow->getSize();
        ioW = ((float) ioW / normalResXs) * size.x;
        ioH = ((float) ioH / normalResYs) * size.y;
    }

    void toYX(int& ioY, int& ioX) {
        auto size = Renderer::currentWindow->getSize();
        ioY = ((float)ioY / size.y) * normalResYs;
        ioX = ((float)ioX / size.x) * normalResXs;
    }

    void toYXBounds(int& ioW, int& ioH) {
        auto size = Renderer::currentWindow->getSize();
        ioW = ((float)ioW / normalResXs) * size.x;
        ioH = ((float)ioH / normalResYs) * size.y;
    }
};