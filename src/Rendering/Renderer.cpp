#include "Renderer.hpp"
#include <iostream>
#include <filesystem>
#include <math.h>
//Call this-----------------------------------------------------------------------------------------------------------

Renderer::Renderer(sf::RenderWindow& window) : currentWindow(window) {
    xPixels = currentWindow.getSize().x;
    yPixels = currentWindow.getSize().y;
    currentWindow.setFramerateLimit(60);
}

//Drawing functions-------------------------------------------------------------------------------------------------------

void Renderer::drawRect(float x, float y, float width, float height, sf::Color c) const {
    sf::RectangleShape square(sf::Vector2f(width, height));

    square.setFillColor(c);
    transform(square, x, y);
    currentWindow.draw(square);
}

sf::RectangleShape Renderer::createRect(float x, float y, float width, float height, sf::Color c) const {
    sf::RectangleShape square(sf::Vector2f(width, height));
    square.setFillColor(c);
    transform(square, x, y);
    return square;
}

void Renderer::drawRectOutline(int x, int y, int width, int height, sf::Color c, int thickness) const {
    int unusedHelp = 0;//we dont need to write 2 values but only have functions for 2 values (lazyness)

    sf::RectangleShape square(sf::Vector2f(width - thickness, height -  2 * thickness));
    square.setOutlineColor(c);
    square.setFillColor(sf::Color(0, 0, 0, 0));
    square.setOutlineThickness(thickness);

    transform(square, x, y);

    currentWindow.draw(square);
}

void Renderer::drawCircle(int x, int y, int radius, sf::Color c, bool fill, int outlineThickness) const {
    int unusedHelp = 0;//we dont need to write 2 values but only have functions for 2 values (lazyness)

    sf::CircleShape circle(radius);
    if (fill == true) {
        circle.setFillColor(c);
    }
    else {
        circle.setOutlineColor(c);
        circle.setOutlineThickness(outlineThickness);
        circle.setFillColor(sf::Color(0, 0, 0, 0));
    }
    transform(circle, x, y);
    currentWindow.draw(circle);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2, sf::Color c, int thickness) const {
    float dX = x2 - x1;
    float dY = y2 - y1;
    int ht = thickness / 2;
    float rot = atan2(dY, dX) * 57.2958f;
    sf::RectangleShape line = sf::RectangleShape(sf::Vector2f(sqrt(abs(dX) * abs(dX) + abs(dY) * abs(dY)), ht * 2));

    line.setOrigin(0, ht);
    transform(line, x1, y1);
    line.setPosition(x1, y1);
    line.setRotation(rot);
    line.setFillColor(c);

    currentWindow.draw(line);
}
/**
 * @brief returns the current mouse position
*  SIDE EFFECTS: actualizes the lastMousePos field if called with "factorInBorders = true"
 * 
 * @param factorInBorders if the mouse leaves the window, the last mouse pos that was called with "factorInBordes = true" will be returned
 * @return sf::Vector2i 
 */
 sf::Vector2i Renderer::getMousePos(bool factorInBorders) {
    auto pos = sf::Mouse::getPosition(currentWindow);
    //although SFML handles resizing for us, it somehow does not handle the mouse position after a resize properly.
    pos.x = xPixels * pos.x / currentWindow.getSize().x;//xPixels is the initial size of the window, which is used by SFML to scale the window.
    pos.y = yPixels * pos.y / currentWindow.getSize().y;

    if (factorInBorders == true) {
        if (pos.x < xPixels && pos.y < yPixels) {
            lastMousePos = sf::Vector2i(pos.x, pos.y);//if the mouse leaves the window, the last mouse pos that was called with "factorInBordes = true" will be returned
            return sf::Vector2i(pos.x, pos.y);
        }
        else {
            return lastMousePos; 
        }
    }
    else {
        return sf::Vector2i(pos.x, pos.y);
    }
}

sf::Texture Renderer::loadTexture(string path, bool repeat) {
    sf::Texture texture;
    if (!texture.loadFromFile(path))
    {
        cout << "failed to load texture of path '" << path << "'";
        exit(0);
    }
    if (repeat == true) {
        sf::Image img = texture.copyToImage();
        if (texture.loadFromImage(img) == false) {
            cout << "failed to load texture image of path '" << path << "'";
            exit(0);
        }

        texture.setRepeated(true);
    }
    return texture;
}


void Renderer::drawRectWithTexture(int x, int y, int width, int height, sf::Texture texture) const {
    sf::RectangleShape square(sf::Vector2f(width, height));
    
    square.setTexture(&texture);
    transform(square, x, y);
    
    if (texture.isRepeated() == true) {
        sf::IntRect rect = sf::IntRect(x, y, width, height);
        square.setTextureRect(rect);
    }//else stretch

    currentWindow.draw(square);
}

sf::Font& Renderer::getFont(string fontName) {
    if(loadedFonts.count(fontName) == 0) {//if font not already in set, load it
        sf::Font font;
        if (!font.loadFromFile("C:/Users/Julian/source/repos/Pretscher/VisualizedPathfinding/recources/Fonts/" + fontName + ".ttf")) {
            cout << "ERROR loading font. Text will not be displayed. \n";
            //DO NOT select a font if there was an error loading, will cause in crash when trying to draw the text
            std::exit(-1);
        }
        else {
            loadedFonts[fontName] = font;
            return loadedFonts[fontName];//retrieve from map so that the reference persists
        }
    }
    else {
        return loadedFonts[fontName];
    }
}

#include <sstream>
void Renderer::drawText(string i_text, string fontName, int x, int y, int width, int height, int charSize, sf::Color color) {
    sf::Text text;
    sf::Font& font = getFont(fontName);
    text.setFont(font);
    text.setString(i_text);
    text.setFillColor(color);
    text.setCharacterSize(charSize);

    string temp = text.getString().toAnsiString();
    bool bold = (text.getStyle() == sf::Text::Bold);

    int maxHeight = 0;
    for (int x = 0; x < text.getString().getSize(); x++)
    {
        sf::Uint32 Character = temp.at(x);
        const sf::Glyph& CurrentGlyph = font.getGlyph(Character, charSize, bold);

        int Height = CurrentGlyph.bounds.height;

        if (maxHeight < Height) {
            maxHeight = Height;
        }
    }

    sf::FloatRect rect = text.getGlobalBounds();

    rect.left = ((float)width / 2) - (rect.width / 2);
    rect.top = ((float)height / 2) - ((float)maxHeight / 2) - (rect.height - maxHeight) + (((float)rect.height - (charSize * 1.5))) / 2;

    text.setPosition(rect.left + x, rect.top  + y);

   // text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    currentWindow.draw(text);
}

void Renderer::transform(sf::Transformable& tranformable, float x, float y) const {
    tranformable.setPosition(x, y);
}

