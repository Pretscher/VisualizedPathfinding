#pragma once
#include "Renderer.hpp"

class VsRenderer : public Renderer {
public:
    int getWorldYs();
    int getWorldXs();

    void getMousePos(int& o_xs, int& o_ys, bool factorInViewspace, bool factorInBorders);
    void updateViewSpace();
    void linkViewSpace(shared_ptr<int[]> io_viewSpace, shared_ptr<const int[]> io_viewspaceLimits);
private:
    shared_ptr<int[]> viewSpace;
    shared_ptr<const int[]> viewSpaceLimits;

    /**
     * @brief Overloaded function from Renderer base class that takes the view space into consideration.
     *  Called every time a drawable object is moved.
     * 
     * @param drawable object to be moved, will be changed in this method
     * @param x 
     * @param y 
     */
    void transform(sf::Transformable& tranformable, float x, float y) override;
};