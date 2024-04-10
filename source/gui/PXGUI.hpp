#pragma once

#include "InteractiveDisplay.hpp"
#include "RotationDisplay.hpp"

class PXGUI
{
public:
    PXGUI() = default;
    ~PXGUI() = default;
    void render();

    InteractiveDisplay idisplay;
    RotationDisplay rdisplay;

    bool isInit = false;
};