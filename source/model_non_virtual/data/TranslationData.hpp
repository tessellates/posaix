#pragma once

#include "TriggerUpdate.hpp"

#include "imgui.h"

struct TranslationData
{
    ImVec2 translation;
    float scale;
};

GENERATE_CODE_MAP(TranslationData, 1);
