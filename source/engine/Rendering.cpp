#include "Rendering.hpp"
#include "PXApplication.hpp"

void initTRD(TextureRenderData* renderData)
{
    SDL_SetRenderDrawColor(PXApplication::renderer, 0, 0, 0, 0);
    renderData->scene = SDL_CreateTexture(PXApplication::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, renderData->sceneSize.x, renderData->sceneSize.y);
    renderData->uData.data.translation = {0, 0};
    renderData->uData.data.scale = 1;
    renderData->uData.dependencies.push_back([=]() {updateTRD(renderData);});
}

void updateTRD(TextureRenderData* renderData)
{
    renderScene(renderData);
}

void loadTRD(TextureRenderData* renderData, SDL_Texture* loaded)
{
    SDL_DestroyTexture(renderData->active);
    renderData->active = loaded;
    SDL_QueryTexture(renderData->active, nullptr, nullptr, &renderData->activeSize.x, &renderData->activeSize.y);
}

void destroyTRD(TextureRenderData* renderData)
{
    SDL_DestroyTexture(renderData->active);
    SDL_DestroyTexture(renderData->scene);
}

void renderScene(TextureRenderData* renderData)
{
    const auto& data = renderData->uData.data;
    const auto& activeSize = renderData->activeSize;
    const auto& sceneSize = renderData->sceneSize;

    SDL_SetRenderDrawColor(PXApplication::renderer, 0, 0, 0, 0);
    SDL_SetRenderTarget(PXApplication::renderer, renderData->scene);
    SDL_RenderClear(PXApplication::renderer);

    SDL_Point appliedSize = {(int)(activeSize.x*data.scale), (int)(activeSize.y*data.scale)};
    SDL_Point offset = {sceneSize.x/2 - appliedSize.x/2 + (int)data.translation.x, sceneSize.y/2 - appliedSize.y/2 + (int)data.translation.y};

    SDL_Rect clip = {offset.x, offset.y, appliedSize.x, appliedSize.y};

    SDL_RenderCopy(PXApplication::renderer, renderData->active, nullptr, &clip);

    SDL_SetRenderTarget(PXApplication::renderer, NULL);
}