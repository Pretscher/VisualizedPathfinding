#include "Renderer.hpp"

class Grid {
private:
    vector<sf::Color> nodes;
    int width, height;
public:

    //create plain colored grid
    Grid(int width, int height, sf::Color defaultColor) {
        this->width = width; this->height = height;
        //create an appropriately sized vector with only white colored pixels only
        nodes.reserve(width * height);
        for(int i = 0; i < width * height; i++) {
            nodes.push_back(defaultColor);
        }
    }


    void draw(Renderer* renderer, vector<int>&& gridScreenSpace) const {
        int screenWidth = gridScreenSpace[2] - gridScreenSpace[0];
        int screenHeight = gridScreenSpace[3] - gridScreenSpace[1];
        int xOffset = gridScreenSpace[0];
        int yOffset = gridScreenSpace[1];

        renderer->drawRect(xOffset, yOffset, screenWidth, screenHeight, sf::Color::White);
        float nodeWidth = (float) screenWidth / this->width;
        float nodeHeight = (float) screenHeight / this->height;
        for(int y = 0; y < this->height; y++) {
            for(int x = 0; x < this->width; x++) {
                int nodeX = xOffset + x * nodeWidth, nodeY = yOffset + y * nodeHeight;
                renderer->drawRect(nodeX, nodeY, nodeWidth, nodeHeight, nodes[y * this->width + x]);
            }
        }
    }


    inline sf::Color getPixel(int x, int y) const {
        return nodes[y * width + x];
    }

    inline void setPixel(int x, int y, sf::Color rgb) {
        calculateWhiteCounter(nodes[y * width + x], rgb);
        nodes[y * width + x] = rgb;
    }

    inline int getWidth() const {
        return width;
    }

    inline int getHeight() const {
        return height;
    }

    inline int getNonWhiteCount() const {
        return nonWhiteCount;
    }
private:
    int nonWhiteCount = 0;
    inline void calculateWhiteCounter(sf::Color oldRgb, sf::Color newRgb) {
        //We do this branchless because it will be called quite often: 
        //if non-white pixel is colored white, substract 1
        int substract = (newRgb == sf::Color::White) && (oldRgb != sf::Color::White);
        //if white pixel is colored non-white, add 1
        int add = (newRgb != sf::Color::White) && (oldRgb == sf::Color::White);
        //add those two (neither is true if oldRgb and newRgb are white thus add (false =) 0)
        nonWhiteCount += add - substract;
    }
};