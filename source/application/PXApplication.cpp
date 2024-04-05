#include "PXApplication.hpp"
#include "PXGUI.hpp"
#include "SDLUserEvents.hpp"

#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <SDL_image.h>
#include <iostream>

PXApplication::PXApplication()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        // Handle error
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        SDL_Quit(); // Clean up SDL before exiting
    }
    // From 2.0.18: Enable native IME.
    #ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
}

int PXApplication::run()
{
    while(isRunning)
    {
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);
        loop();
    }
    return 0;
}

void PXApplication::loop()
{
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            isRunning = false;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            isRunning = false;
        if (event.type == PX_DATA_CHANGE)
            triggerUpdate(event.user.data1, event.user.code);
    }

    clock.update();
    PXApplication::deltaTime = clock.getDeltaTime();

    if (engine)
    {

    }

    if (gui)
    {

        gui->render();
    }

    frc.update();
    if (PXApplication::showFrameRate)
        frc.render();
    SDL_RenderPresent(renderer);
}

void PXApplication::init(bool test)
{
    if (isInit)
    {
        return;
    }

    isInit = true;


    if (SDL_GetDisplayBounds(0, &display) != 0)
    {
        std::cout <<  SDL_GetError()  << std::endl;
    }
    
    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags =  (SDL_WindowFlags)(SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("POSAIX", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, display.w, display.h, window_flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


    const std::vector<std::pair<int, int>> commonResolutions = {
        {800, 600}, {1024, 768}, {1280, 720}, {1680, 945}, {1920, 1080}, {2048,1152}
    };

    for (const auto& res : commonResolutions) {
        if (res.first < display.w && res.second < display.h) {
            resolutions.push_back(res);
        }
    }

    PXApplication::currentResolutionIndex = resolutions.size() - 1;

    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->IniFilename = NULL;
    io->LogFilename = NULL;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
    //io->FontGlobalScale = 3; // Scale the fonts
    //ImGui::GetStyle().ScaleAllSizes(4);

    std::cout << "STATS FOR CATS:" << std::endl;
    std::cout << "disx:" << display.w << std::endl;
    std::cout << "disy:" << display.h << std::endl;

    TTF_Font* font = TTF_OpenFont("assets/Arial.ttf", 48);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        // Handle error
    }
    frc = FrameRateCounter(renderer, font);
    changeWindow();

    gui = new PXGUI();
}

PXApplication::~PXApplication()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void PXApplication::changeWindow()
{
    SDL_SetWindowSize(window, resolutions[currentResolutionIndex].first, resolutions[currentResolutionIndex].second);
}

void PXApplication::toggleFullscreen() 
{
    Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
    Uint32 isFullscreen = SDL_GetWindowFlags(window) & fullscreenFlag;
    SDL_SetWindowFullscreen(window, isFullscreen ? 0 : fullscreenFlag);
    PXApplication::isFullscreen = !isFullscreen;
    if (!PXApplication::isFullscreen)
    {
        changeWindow();
    }
}
//STATIC
bool PXApplication::isFullscreen = true;
int PXApplication::currentResolutionIndex = 0;
float PXApplication::deltaTime = 0;
bool PXApplication::showFrameRate = true;

SDL_Renderer* PXApplication::renderer = nullptr;
SDL_Rect PXApplication::display = {};
