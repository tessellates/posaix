#include "CubeData.hpp"
#include "PXApplication.hpp"
#include "Projection.hpp"

#include <SDL_image.h>

void CubeScene::init()//TextureRenderData* trd)
{
    SDL_SetRenderDrawColor(PXApplication::renderer, 0, 0, 0, 0);
    scene = SDL_CreateTexture(PXApplication::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sceneSize.x, sceneSize.y);
    projection = SDL_CreateTexture(PXApplication::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, sceneSize.x, sceneSize.y);
    SDL_SetTextureBlendMode(projection, SDL_BLENDMODE_BLEND);
    cube.data = Cube::create({512,512,512}, 256);
    // subscribe to the data;
    cube.dependencies.push_back([&]() {this->update();});
    SDL_Surface* presurface = IMG_Load("assets/test_square.png");
    surface = SDL_ConvertSurfaceFormat(presurface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(presurface);
}

void CubeScene::init(TextureRenderData* trd)
{
    SDL_SetRenderDrawColor(PXApplication::renderer, 0, 0, 0, 0);
    scene = SDL_CreateTexture(PXApplication::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sceneSize.x, sceneSize.y);
    projection = SDL_CreateTexture(PXApplication::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, sceneSize.x, sceneSize.y);
    SDL_SetTextureBlendMode(projection, SDL_BLENDMODE_BLEND);
    cube.data = Cube::create({512,512,0}, 256);
    // subscribe to the data;
    this->trd = trd;
    cube.dependencies.push_back([&]() {this->update();});
    this->trd->uData.dependencies.push_back([&]() {this->update();});
    surface = trd->sceneSurface;
}

void CubeScene::update()
{
    renderScene();
}

void CubeScene::renderScene()
{
    testP(PXApplication::renderer, projection, surface, cube.data);

    SDL_SetRenderTarget(PXApplication::renderer, scene);
    SDL_SetRenderDrawColor(PXApplication::renderer, 50, 0, 50, 255);
    SDL_RenderClear(PXApplication::renderer);

    SDL_RenderCopy(PXApplication::renderer, projection, nullptr, nullptr);

    SDL_SetRenderDrawColor(PXApplication::renderer, 255, 0, 0, 255);

    drawCube(PXApplication::renderer, cube.data.project2D(), cube.data.culling);

    SDL_SetRenderTarget(PXApplication::renderer, NULL);
}

