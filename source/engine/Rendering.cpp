#include "Rendering.hpp"
#include "PXApplication.hpp"

void initTRD(TextureRenderData* renderData)
{
    SDL_SetRenderDrawColor(PXApplication::renderer, 0, 0, 0, 0);
    renderData->scene = SDL_CreateTexture(PXApplication::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, renderData->sceneSize.x, renderData->sceneSize.y);
    renderData->uData.data.translation = {0, 0};
    renderData->uData.data.scale = 1;
    // subscribe to the data;
    renderData->uData.dependencies.push_back([=]() {updateTRD(renderData);});

    renderData->sceneSurface = SDL_CreateRGBSurface(0, renderData->sceneSize.x, renderData->sceneSize.y, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
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
    SDL_FreeSurface(renderData->sceneSurface);
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

    updateSurface(renderData);

    SDL_SetRenderTarget(PXApplication::renderer, NULL);
}

void updateSurface(TextureRenderData* renderData)
{
    const auto& sceneSize = renderData->sceneSize;
    int width = sceneSize.x, height = sceneSize.y;

    if (SDL_MUSTLOCK(renderData->sceneSurface)) {
        SDL_LockSurface(renderData->sceneSurface);
    }

    SDL_memset(renderData->sceneSurface->pixels, 0, renderData->sceneSurface->h * renderData->sceneSurface->pitch);
    SDL_RenderReadPixels(PXApplication::renderer, NULL, SDL_PIXELFORMAT_RGBA8888, renderData->sceneSurface->pixels, renderData->sceneSurface->pitch);

    if (SDL_MUSTLOCK(renderData->sceneSurface)) {
        SDL_UnlockSurface(renderData->sceneSurface);
    }
}