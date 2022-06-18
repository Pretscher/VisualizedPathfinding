#include "Renderer.hpp"
#include "WorldManager.hpp"
#include "UIManager.hpp"
#include <memory>
class EventManager {
public:
    //Memory of this object has to be externally managed.
    shared_ptr<Renderer> renderer;
    unique_ptr<UIManager> uiManager;

    WorldManager worldManager;
    EventManager(shared_ptr<Renderer> i_renderer) {
        std::srand(std::time(nullptr));
        this->renderer = i_renderer;
        this->uiManager = make_unique<UIManager>(renderer);
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
        auto path = worldManager.findPath(0, 0, 50, 50);
        worldManager.addPathToGrid(path);
    }

    void eventloop() {
        worldManager.update();

    }

    void drawingloop() {
        worldManager.draw(*renderer, uiManager->getGridScreenSpace());
        uiManager->draw();
    }
};