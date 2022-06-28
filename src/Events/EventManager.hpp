#include "Renderer.hpp"
#include "WorldManager.hpp"
#include "UIManager.hpp"
#include <memory>
#include "EventObjects/SelectGridNode.hpp"
#include <map>

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
    /**
     * @brief All Events inherit from the base class "Event" with an Update, Draw and isFinished function. They can be easily managed in this class
     * but the actual logic is outsourced into the derived classes from Event. 
     * Because the class is partially (pure) virtual, it has to be stored in a pointer.
     */
    map<string, Event*> events;
    void init() {
        auto path = worldManager.findPath(0, 0, 50, 50);
        worldManager.addPathToGrid(path);
    }


    void eventloop() {
        worldManager.update();
        uiManager.update();
        handleEventStarts();

        for (auto cEvent = events.cbegin(); cEvent != events.cend()/* not hoisted */; /* no increment */) {
            cEvent->second->update();
            if (cEvent->second->isFinished()) {
                handleEventExits(cEvent);
                events.erase(cEvent++);
            } else {
                cEvent++;
            }
        }
    }

    void handleEventStarts() {
        if(uiManager.buttons["Start"]->wasPressed(renderer)) {
            events["Select start node"] = new SelectGridNode(renderer, worldManager.grid);
        }
         if(uiManager.buttons["Goal"]->wasPressed(renderer)) {
            events["Select goal node"] = new SelectGridNode(renderer, worldManager.grid);
        }
    }

    void handleEventExits(std::map<std::string, Event *>::const_iterator& exitingEvent) {
        if(exitingEvent->first == "Select start node") {
            cout << "Selected start node successfully.\n";
        }
        else if(exitingEvent->first == "Select goal node") {
            cout << "Selected goal node successfully.\n";
        }
        delete exitingEvent->second;
    }

    void drawingloop() {
        worldManager.draw(uiManager.getGridScreenSpace());
        uiManager.draw();
        for(auto const& cEvent : events) {
            cEvent.second->draw();
        }
    }
};