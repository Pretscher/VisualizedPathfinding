#include "VsRenderer.hpp"
#include <iostream>
#include <filesystem>
#include <math.h>

void VsRenderer::linkViewSpace(shared_ptr<int[]> io_viewSpace, shared_ptr<const int[]> io_viewspaceLimits) {
    viewSpace = io_viewSpace;
    viewSpaceLimits = io_viewspaceLimits;
}

int VsRenderer::getWorldXs() {
    return currentWindow->getSize().x + viewSpaceLimits[1];
}

int VsRenderer::getWorldYs() {
    return currentWindow->getSize().y + viewSpaceLimits[0];
}

void VsRenderer::getMousePos(int& o_xs, int& o_ys, bool factorInViewspace, bool factorInBorders) {
    auto pos = sf::Mouse::getPosition(*currentWindow);
    int x = pos.x;
    int y = pos.y;
    toYX(y, x);

    int helpVsX = viewSpace[1];
    int helpVsY = viewSpace[0];
    toYX(helpVsX, helpVsY);

    if (factorInBorders == true) {
        if (x < normalResXs && y < normalResYs) {
            if (factorInViewspace == true) {
                o_xs = x + helpVsX;
                o_ys = y + helpVsY;
            }
            else {
                o_xs = x;
                o_ys = y;
            }
        }
    }
    else {
        if (factorInViewspace == true) {
            o_xs = x + helpVsX;
            o_ys = y + helpVsY;
        }
        else {
            o_xs = x;
            o_ys = y;
        }
    }
}

void VsRenderer::updateViewSpace() {
    auto size = currentWindow->getSize();

    int moveSpeed = 30;
    
    int mouseX = -1, mouseY = -1;
    getMousePos(mouseX, mouseY, false, true);
    int helpViewSpace[2];//temp viewspace, maybe going to be written into real viewspace
    helpViewSpace[0] = viewSpace[0];
    helpViewSpace[1] = viewSpace[1];


    int vsLeft = viewSpaceLimits[0];
    int vsRight = viewSpaceLimits[1];
    int vsTop = viewSpaceLimits[2];
    int vsBottom = viewSpaceLimits[3];
    toYXBounds(vsTop, vsLeft);
    toYXBounds(vsBottom, vsRight);
    if (mouseY != -1) {
        int localY = mouseY;
        int localX = mouseX;
        fromYX(mouseY, mouseX);

        if (localY < normalResYs / 10) {
            if (helpViewSpace[0] - moveSpeed > vsTop) {
                helpViewSpace[0] -= moveSpeed;
            }
            else {
                helpViewSpace[0] = vsTop;
            }
        }
        if (localY > normalResYs * 0.9) {
            if (helpViewSpace[0] + moveSpeed < vsBottom) {
                helpViewSpace[0] += moveSpeed;
            }
            else {
                helpViewSpace[0] = vsBottom;
            }
        }
        if (localX < normalResXs / 10) {
            if (helpViewSpace[1] - moveSpeed > vsLeft) {
                helpViewSpace[1] -= moveSpeed;
            }
            else {
                helpViewSpace[1] = vsLeft;
            }
        }
        if (localX > normalResXs * 0.9f) {
            if (helpViewSpace[1] + moveSpeed < vsRight) {
                helpViewSpace[1] += moveSpeed;
            }
            else {
                helpViewSpace[1] = vsRight;
            }
        }
    }
    viewSpace[0] = helpViewSpace[0];
    viewSpace[1] = helpViewSpace[1];
}

void VsRenderer::transform(sf::Transformable& tranformable, int x, int y) {
    tranformable.move(x - viewSpace[1], y - viewSpace[0]);
}