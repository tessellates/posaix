#pragma once

#include "CRTP.hpp"
#include "SDLUserEvents.hpp"
#include "TriggerUpdate.hpp"

#include <vector>
#include <iostream>
#include <SDL.h>

// Map codes to types (declaration)
template<int Code>
struct CodeToTypeMap;

// Map types back to codes (declaration)
template<typename T>
struct TypeToCodeMap {
    static constexpr Uint32 code = -1; // Default, invalid code
};

// Macro to generate the mapping from code to type
#define GENERATE_CODE_MAP(_Type, _Code) \
    constexpr int _Type##_Code = _Code; \
    template<> struct CodeToTypeMap<_Code> { using Type = _Type; }; \
    template<> struct TypeToCodeMap<_Type> { static constexpr Uint32 code = _Code; };

template <typename _Data>
class UpdatableData 
{
public:
    using Data = _Data;

    Data data;

    void update()
    {
        for (auto& emit : dependencies)
        {
            emit(data);
        }
    }

    void updateEvent()
    {
        SDL_Event event;
        SDL_zero(event); // Initialize the event to zero
        event.type = PX_DATA_CHANGE;
        event.user.code = TypeToCodeMap<Data>().code;
        event.user.data1 = this;
        SDL_PushEvent(&event);
    }

    std::vector<std::function<void(const Data&)>> dependencies;    
};
