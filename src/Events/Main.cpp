#include <iostream>
#include "EventManager.hpp"
using namespace std;

int main() {

    sf::RenderWindow* cWindow = new sf::RenderWindow(sf::VideoMode(sf::VideoMode::getFullscreenModes().at(0).width,
            sf::VideoMode::getFullscreenModes().at(0).height), "Pathfinding", sf::Style::Titlebar | sf::Style::Close);
    cWindow->setPosition(sf::Vector2i(0, 0));
    Renderer* renderer = new Renderer(cWindow);
    EventManager eventManager = EventManager(renderer);
    while(cWindow->isOpen()) {
        eventManager.update();
    }
    
    delete cWindow;
    delete renderer;
    return 0;
}