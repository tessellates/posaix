#pragma once

#include "TranslationDisplay.hpp"

#include "imgui.h"

class InteractiveDisplay
{
public:
    InteractiveDisplay() = default;
    ~InteractiveDisplay() = default;
    void init();
    void render();

    TranslationDisplay tdisplay;
    ImVec2 size = {600, 600};
};


