#include <iostream>
#include <memory>
#include "EventManager.hpp"
using namespace std;

int main() {
    sf::RenderWindow* cWindow = new sf::RenderWindow(
        sf::VideoMode(1920, 1080), "Pathfinding", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    Renderer* renderer = new Renderer(cWindow);
    EventManager eventManager(renderer);
    while(cWindow->isOpen()) {
        eventManager.update();
    }
    delete cWindow;
    delete renderer;
    return 0;
}