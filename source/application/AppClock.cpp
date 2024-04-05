#include "AppClock.hpp"
#include <iostream>

AppClock::AppClock()
{
    lastTick = SDL_GetTicks();
    startTick = lastTick;
    deltaTime = 0.0f;
}

void AppClock::update() {
    Uint32 currentTick = SDL_GetTicks();
    deltaTime = (currentTick - lastTick);
    lastTick = currentTick;
}

float AppClock::getDeltaTime() const {
    return deltaTime;
}

Uint32 AppClock::getTotalTime() const {
    return lastTick - startTick;
}

