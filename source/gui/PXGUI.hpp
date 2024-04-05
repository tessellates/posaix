#pragma once

#include "InteractiveDisplay.hpp"

class PXGUI
{
public:
    PXGUI() = default;
    ~PXGUI() = default;
    void render();

    InteractiveDisplay idisplay;
};