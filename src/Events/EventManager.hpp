#include "Renderer.hpp"
#include "WorldManager.hpp"

class EventManager {
public:
    //Memory of this object has to be externally managed.
    Renderer* renderer;
    WorldManager* worldManager;

    EventManager(Renderer* i_renderer) {
        std::srand(std::time(nullptr));
        this->renderer = i_renderer;
        this->worldManager = new WorldManager();

        init();
    }

    ~EventManager() {
        delete worldManager;
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
        worldManager->draw(renderer);

    }
};