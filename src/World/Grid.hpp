#include "Renderer.hpp"

class Grid {
private:
    vector<sf::RectangleShape> nodes;
    int cols, rows;
    sf::Color defaultColor;
    vector<int> screenDimensions;
    Renderer& renderer;
public:
    //create plain colored grid
    Grid(int cols, int rows, vector<int>&& screenDimensions, sf::Color defaultColor, Renderer& i_renderer) : renderer(i_renderer) {
        this->cols = cols;
        this->rows = rows;

        this->defaultColor = defaultColor;
        this->screenDimensions = screenDimensions;
        initDrawing();
    }

    void initDrawing() {
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

    void draw() {
        renderer.drawRect(screenDimensions[0], screenDimensions[1],  screenDimensions[2], screenDimensions[3], sf::Color::White);//background
        for(int i = 0; i < nodes.size(); i++) {
            renderer.currentWindow.draw(nodes[i]);
        }
    }


    inline sf::Color getPixelColor(int x, int y) const {
        return nodes[y * cols + x].getFillColor();
    }

    inline void setPixel(int x, int y, sf::Color rgba) {
        calculateWhiteCounter(nodes[y * cols + x].getFillColor(), rgba);
        nodes[y * cols + x].setFillColor(rgba);
    }

    inline void addPixelColor(int x, int y, sf::Color rgba) {
        sf::Color oldColor = nodes[y * cols + x].getFillColor();
        sf::Color combinedColor;
        if(oldColor == sf::Color::White) {
            combinedColor = rgba;
        } 
        else {
           combinedColor = sf::Color(oldColor.r + rgba.r, oldColor.g + rgba.g, oldColor.b + rgba.b, rgba.a);
        }
        calculateWhiteCounter(oldColor, combinedColor);
        nodes[y * cols + x].setFillColor(combinedColor);
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

    inline sf::Vector2f getNodeSize() {
        return sf::Vector2f((float) screenDimensions[2] / this->cols, (float) screenDimensions[3] / this->rows);
    }

    inline vector<int>& getScreenDimensions() {
        return screenDimensions;
    }

    sf::RectangleShape& getNode(int col, int row) {
        return nodes[row * cols + col];
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