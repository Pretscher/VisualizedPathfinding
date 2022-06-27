#include "Renderer.hpp"
class Event {
public:
    Event(Renderer& i_renderer) : renderer(i_renderer) {

    }
    virtual void draw() const;
    virtual void update();
    virtual bool isFinished() const;
protected:
    Renderer& renderer;
private:
    Event();
};