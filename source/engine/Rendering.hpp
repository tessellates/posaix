#pragma once

#include "TranslationData.hpp"
#include "UpdatableData.hpp"

#include <SDL.h>

struct TextureRenderData
{
    SDL_Point sceneSize = {1024, 1024};
    SDL_Texture* scene;

    SDL_Point activeSize;
    SDL_Texture* active;

    UpdatableData<TranslationData> uData;

    SDL_Surface* sceneSurface;
};

void updateTRD(TextureRenderData* renderData);
void initTRD(TextureRenderData* renderData);
void loadTRD(TextureRenderData* renderData, SDL_Texture* loaded);
void destroyTRD(TextureRenderData* renderData);
void renderScene(TextureRenderData* renderData);
void updateSurface(TextureRenderData* renderData);