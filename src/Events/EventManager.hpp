#include "Renderer.hpp"
#include "WorldManager.hpp"
#include "UIManager.hpp"
#include <memory>
class EventManager {
public:
    //Memory of this object has to be externally managed.
    Renderer* renderer;
    UIManager* uiManager;
    WorldManager* worldManager;
    EventManager(Renderer* i_renderer) {
        std::srand(std::time(nullptr));
        this->renderer = i_renderer;
        this->uiManager = new UIManager(renderer);
        Grid* g = new Grid(192, 108, uiManager->getGridScreenSpace(), sf::Color::White, *renderer);
        this->worldManager = new WorldManager(g);
        init();
    }

    void update() {
        sf::Event events;
        auto window = renderer->currentWindow;
        while (window->pollEvent(events)) {
            if (events.type == sf::Event::Closed) {
                window->close();
                exit(0);
            }
        }
        window->clear();//clear with every iteration

        //draw here-------------------------------------------------------
        this->eventloop();
        this->drawingloop();

        //\draw here------------------------------------------------------
        window->display();//display things drawn since clear() was called
    }

    void init() {
        auto path = worldManager->findPath(0, 0, 50, 50);
        worldManager->addPathToGrid(path);
    }

    void eventloop() {
        worldManager->update();

    }

    void drawingloop() {
        worldManager->draw(*renderer, uiManager->getGridScreenSpace());
        uiManager->draw();
    }
};