#include "Renderer.hpp"
#include "WorldManager.hpp"
#include "UIManager.hpp"
#include "PathfindingManager.hpp"
#include <memory>
#include "EventObjects/SelectGridNode.hpp"
#include <map>

class EventManager {
public:
    //Memory of this object has to be externally managed.
    Renderer& renderer;
    UIManager uiManager;
    WorldManager worldManager;
    PathfindingManager pathfindingManager;
    EventManager(Renderer& i_renderer) : renderer(i_renderer),
            uiManager(renderer),
            worldManager(Grid(192, 108, uiManager.getGridScreenSpace(), sf::Color::White, renderer)),
            pathfindingManager(worldManager.getGraph(), worldManager.getGrid()) {
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

    }

    void eventloop() {
        worldManager.update();
        uiManager.update();
        handleEventStarts();

        //update events
        for (auto cEvent = events.cbegin(); cEvent != events.cend()/* not hoisted */; /* no increment */) {
            cEvent->second->update();
            if (cEvent->second->isFinished()) {
                handleEventExits(cEvent);
                delete cEvent->second;
                events.erase(cEvent++);
            } else {
                cEvent++;
            }
        }
    }

    void handleEventStarts() {
        if(uiManager.buttons["select start node"]->wasPressed(renderer)) {
            events["select start node"] = new SelectGridNode(renderer, worldManager.getGrid());
        }
        if(uiManager.buttons["select goal node"]->wasPressed(renderer)) {
            events["select goal node"] = new SelectGridNode(renderer, worldManager.getGrid());
        }
        if(uiManager.buttons["start pathfinding"]->wasPressed(renderer)) {
            pathfindingManager.tryFindingPath();
        }
    }


    void handleEventExits(std::map<std::string, Event *>::const_iterator& exitingEvent) {
        if(exitingEvent->first == "select start node") {
            pathfindingManager.setStartPoint(exitingEvent->second->getData()[0], exitingEvent->second->getData()[1]);
        }
        else if(exitingEvent->first == "select goal node") {
            pathfindingManager.setGoalPoint(exitingEvent->second->getData()[0], exitingEvent->second->getData()[1]);
        }
    }

    void drawingloop() {
        worldManager.draw(uiManager.getGridScreenSpace());
        uiManager.draw();
        for(auto const& cEvent : events) {
            cEvent.second->draw();
        }
        pathfindingManager.drawPath();//if there is a path, draw it
    }
};