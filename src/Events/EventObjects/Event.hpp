#include "Renderer.hpp"
class Event {
public:
    Event(Renderer& i_renderer) : renderer(i_renderer) {

    }
    virtual void draw() const = 0;
    virtual void update() = 0;
    bool isFinished() {
        return finished;
    }
    /**
     * @brief Most data can somehow be transmitted through a vector of integers. A template based approach would not be useful
     * because we ant to save all Events in a single Array. If you need to implement a different function for returning the Data, 
     * you can additionally save the derived class and use the function from there.
     * @return vector<int> the Data the Event currently returns
     */
    virtual vector<int> getData() const = 0;

protected:
    Renderer& renderer;
    bool finished = false;
private:
    Event();
};