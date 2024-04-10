#pragma once

#include "CubeData.hpp"

#include "imgui.h"

class RotationDisplay
{
public:
    RotationDisplay() = default;
    ~RotationDisplay() = default;
    void init();
    void render();

    ImVec2 size = {600, 650};
    CubeScene cubeScene;

    float zoom = 0;

    bool isInit = false;
};


