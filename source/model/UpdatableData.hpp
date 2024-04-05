#pragma once

#include "CRTP.hpp"
#include "SDLUserEvents.hpp"

#include <vector>
#include <iostream>
#include <SDL.h>

class Updatable
{
public:
    virtual ~Updatable() = 0;
    virtual void update() = 0;
};

inline Updatable::~Updatable() {}  // Provide an empty definition for the pure virtual destructor

template <typename _Data>
class UpdatableData : public Updatable
{
public:
    using Data = _Data;

    Data data;

    void update() override
    {
        for (auto& emit : dependencies)
        {
            emit();
        }
    }

    void updateEvent()
    {
        SDL_Event event;
        SDL_zero(event); // Initialize the event to zero
        event.type = PX_DATA_CHANGE;
        event.user.data1 = this;
        SDL_PushEvent(&event);
    }

    std::vector<std::function<void()>> dependencies;    
};

inline void triggerUpdate(void* ptr, int)
{
    static_cast<Updatable*>(ptr)->update();
}