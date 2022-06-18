#include <iostream>
#include <memory>
#include "EventManager.hpp"
using namespace std;

int main() {
    shared_ptr<sf::RenderWindow> cWindow = make_shared<sf::RenderWindow>(
        sf::VideoMode(1920, 1080), "Pathfinding", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    shared_ptr<Renderer> renderer = make_shared<Renderer>(cWindow);
    EventManager eventManager(renderer);
    while(cWindow->isOpen()) {
        eventManager.update();
    }
    return 0;
}