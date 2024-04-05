#pragma once

#include "imgui.h"

#include "TranslationData.hpp"
#include "UpdatableData.hpp"
#include "Rendering.hpp"

class TranslationDisplay
{
public:
    TranslationDisplay() = default;
    ~TranslationDisplay() = default;
    void init();
    void execute();
    void render();
    void reset();

    ImTextureID texture;
    bool isDragging = false;
    ImVec2 dragStartPos;
    ImVec2 size = {512, 512};
    UpdatableData<TranslationData>* offset = nullptr;
    ImGuiWindowFlags flags = (ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    TextureRenderData tdr;

    bool isInit = false;
};


