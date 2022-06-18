#include "Renderer.hpp"
#include <iostream>
#include <filesystem>
#include <math.h>
//Call this-----------------------------------------------------------------------------------------------------------

Renderer::Renderer(shared_ptr<sf::RenderWindow> window) {
    currentWindow = window;
    //SFML always uses the dimensions of window creation, thus we can set them here and never change them again.
    xPixels = currentWindow->getSize().x;
    yPixels = currentWindow->getSize().y;
    currentWindow->setFramerateLimit(60);
}

//Drawing functions-------------------------------------------------------------------------------------------------------

void Renderer::drawRect(float x, float y, float width, float height, sf::Color c) const {
    sf::RectangleShape square(sf::Vector2f(width, height));

    square.setFillColor(c);
    transform(square, x, y);
    currentWindow->draw(square);
}


void Renderer::drawRectOutline(int x, int y, int width, int height, sf::Color c, int thickness) const {
    int unusedHelp = 0;//we dont need to write 2 values but only have functions for 2 values (lazyness)

    sf::RectangleShape square(sf::Vector2f(width - thickness, height -  2 * thickness));
    square.setOutlineColor(c);
    square.setFillColor(sf::Color(0, 0, 0, 0));
    square.setOutlineThickness(thickness);

    transform(square, x, y);

    currentWindow->draw(square);
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
    currentWindow->draw(circle);
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

    currentWindow->draw(line);
}

void Renderer::getMousePos(int& o_xs, int& o_ys, bool factorInBorders) const {
    auto pos = sf::Mouse::getPosition(*currentWindow);
    int x = pos.x;
    int y = pos.y;

    if (factorInBorders == true) {
        if (x < xPixels && y < yPixels) {
                o_xs = x;
                o_ys = y;
        }
    }
    else {
        o_xs = x;
        o_ys = y;
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

    currentWindow->draw(square);
}

void Renderer::drawText(string i_text, int x, int y, int width, int height, int charSize, sf::Color color) const {
    sf::Text text;

    sf::Font font;
    if (!font.loadFromFile("myRecources/Calibri Regular.ttf"))
    {
        cout << "error loading font.";
    }

    // select the font
    text.setFont(font); // font is a sf::Font
    text.setString(i_text);


    int unusedHelp = 0;
    text.setFillColor(color);
    text.setCharacterSize(charSize);

    size_t CharacterSize = text.getCharacterSize();


    string String = text.getString().toAnsiString();
    bool bold = (text.getStyle() == sf::Text::Bold);
    size_t MaxHeight = 0;

    for (size_t x = 0; x < text.getString().getSize(); x++)
    {
        sf::Uint32 Character = String.at(x);

        const sf::Glyph& CurrentGlyph = font.getGlyph(Character, CharacterSize, bold);

        size_t Height = CurrentGlyph.bounds.height;

        if (MaxHeight < Height)
            MaxHeight = Height;
    }

    sf::FloatRect rect = text.getGlobalBounds();

    rect.left = ((float)width / 2.0f) - (rect.width / 2.0f);
    rect.top = ((float)height / 2.0f) - ((float)MaxHeight / 2.0f) - (rect.height - MaxHeight) + ((rect.height - CharacterSize) / 2.0f);

    int l = rect.left - (currentWindow->getSize().x / 385);
    int t = rect.top - (currentWindow->getSize().y / 72);

    text.setPosition(l + x, t + y);

   // text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    currentWindow->draw(text);
}

void Renderer::transform(sf::Transformable& tranformable, float x, float y) const {
    tranformable.setPosition(x, y);
}

