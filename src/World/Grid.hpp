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


    void draw(Renderer* renderer) const {
        int windowWidth = renderer->currentWindow->getSize().x;
        int windowHeight = renderer->currentWindow->getSize().y;

        int nodeWidth = windowWidth / this->width;
        int nodeHeight = windowHeight / this->height;
        for(int x = 0; x < this->width; x++) {
            for(int y = 0; y < this->height; y++) {
                int nodeX = x * nodeWidth, nodeY = y * nodeHeight;
                renderer->drawRect(nodeX, nodeY, nodeWidth, nodeHeight, nodes[y * this->height + x]);
            }
        }
    }


    inline sf::Color getPixel(int x, int y) const {
        return nodes[y * height + x];
    }

    inline void setPixel(int x, int y, sf::Color rgb) {
        calculateWhiteCounter(nodes[y * height + x], rgb);
        nodes[y * height + x] = rgb;
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