#include "Renderer.hpp"
#include "WorldManager.hpp"
#include "UIManager.hpp"
#include <memory>
#include "EventObjects/SelectGridNode.hpp"

class EventManager {
public:
    //Memory of this object has to be externally managed.
    Renderer& renderer;
    UIManager uiManager;
    WorldManager worldManager;
    EventManager(Renderer& i_renderer) : renderer(i_renderer), uiManager(renderer), worldManager(Grid(192, 108, uiManager.getGridScreenSpace(), sf::Color::White, renderer)) {
        std::srand(std::time(nullptr));
        init();
    }

    void update() {
        sf::Event events;
        while (renderer.currentWindow.pollEvent(events)) {
            if (events.type == sf::Event::Closed) {
                renderer.currentWindow.close();
                exit(0);
            }
        }
        renderer.currentWindow.clear();//clear with every iteration

        //draw here-------------------------------------------------------
        this->eventloop();
        renderer.update();
        this->drawingloop();

        //\draw here------------------------------------------------------
        renderer.currentWindow.display();//display things drawn since clear() was called
    }

    vector<Event*> events;
    void init() {
        auto path = worldManager.findPath(0, 0, 50, 50);
        worldManager.addPathToGrid(path);
    }

    void eventloop() {
        worldManager.update();
        for(int i = 0; i < events.size(); i++) {
            events[i]->update();
            if(events[i]->isFinished()) {
                delete events[i];
                events.erase(events.begin() + i, events.begin() + i);//remove event from vector
            }
        }
    }

    void drawingloop() {
        worldManager.draw(uiManager.getGridScreenSpace());
        uiManager.draw();
        for(int i = 0; i < events.size(); i++) {
            events[i]->draw();
        }
    }
};