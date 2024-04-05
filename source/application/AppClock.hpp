#pragma once

#include <SDL.h>

class AppClock {
public:
    AppClock();
    AppClock(float speed);
    void update(); // Call this once per frame
    float getDeltaTime() const; // Get the time elapsed since last frame in seconds
    Uint32 getTotalTime() const; // Get the total elapsed time since the start of the clock in milliseconds

public:
    Uint32 lastTick; // Time of the last update call
    Uint32 startTick; // The time when the clock was started
    float deltaTime; // Time between the current frame and the last frame in seconds
};
