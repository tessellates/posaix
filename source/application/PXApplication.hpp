#pragma once

#include "AppClock.hpp"
#include "Singleton.hpp"
#include "FrameRateCounter.hpp"

#include <vector>
#include <SDL.h>
#include "imgui.h"

class Engine;
class PXGUI;

class PXApplication : public Singleton<PXApplication>
{
   friend class Singleton<PXApplication>; // Grant base access to constructor.
public:
    // boiler-plate singleton
    PXApplication(PXApplication &other) = delete;
    void operator=(const PXApplication &) = delete;

    int run();
    void init(bool test = true);
    void updateDisplay();
    void toggleFullscreen();
    void changeWindow();

    static bool isFullscreen;
    static int currentResolutionIndex;
    static float deltaTime;
    static bool showFrameRate;

    static SDL_Renderer* renderer;
    static SDL_Rect display;
    SDL_Event event;

    std::vector<std::pair<int,int>> resolutions;

private:
    PXApplication();
    ~PXApplication();

    void loop();
    
    Engine* engine = nullptr;
    PXGUI* gui = nullptr;

    SDL_Window* window = nullptr;
    ImGuiIO* io;

    bool isRunning = true;
    bool isInit = false;

    AppClock clock;
    FrameRateCounter frc;
};
