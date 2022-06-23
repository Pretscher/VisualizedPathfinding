#include <iostream>
#include <memory>
#include "EventManager.hpp"
using namespace std;

int main() {
    sf::RenderWindow cWindow(sf::VideoMode(1920, 1080), "Pathfinding", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    Renderer renderer(cWindow);
    EventManager eventManager(renderer);
    while(cWindow.isOpen()) {
        eventManager.update();
    }
    return 0;
}