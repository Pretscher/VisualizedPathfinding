#include "Renderer.hpp"

class Grid {
private:
    vector<sf::RectangleShape> nodes;
    int cols, rows;
    sf::Color defaultColor;
    Grid(){}
    vector<int> screenDimensions;
public:

    //create plain colored grid
    Grid(int cols, int rows, vector<int>&& screenDimensions, sf::Color defaultColor, const Renderer& renderer) {
        this->cols = cols;
        this->rows = rows;

        this->defaultColor = defaultColor;
        this->screenDimensions = screenDimensions;
        initDrawing(renderer);
    }

    void initDrawing(const Renderer& renderer) {
        nodes.reserve(cols * rows);
        float nodeWidth = (float) screenDimensions[2] / this->cols;
        float nodeHeight = (float) screenDimensions[3] / this->rows;

        for(int y = 0; y < rows; y ++) {
            for(int x = 0; x < cols; x++) {
                float nodeX = screenDimensions[0] + x * nodeWidth;
                float nodeY = screenDimensions[1] + y * nodeHeight;

                nodes.push_back(renderer.createRect(nodeX, nodeY, nodeWidth, nodeHeight, defaultColor));
            }
        }
    }

    void draw(const Renderer& renderer) {

        renderer.drawRect(screenDimensions[0], screenDimensions[1],  screenDimensions[2], screenDimensions[3], sf::Color::White);//background
        for(int i = 0; i < nodes.size(); i++) {
            renderer.currentWindow->draw(nodes[i]);
        }
    }


    inline sf::Color getPixelColor(int x, int y) const {
        return nodes[y * cols + x].getFillColor();
    }

    inline void setPixel(int x, int y, sf::Color rgb) {
        calculateWhiteCounter(nodes[y * cols + x].getFillColor(), rgb);
        nodes[y * cols + x].setFillColor(rgb);
    }

    inline int getCols() const {
        return cols;
    }

    inline int getRows() const {
        return rows;
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