#pragma once

#include "UpdatableData.hpp"

// Generalized UpdateExe using the CodeToTypeMap
template<int Code>
struct UpdateExe {
    static void execute(void* ptr) {
        using TargetType = typename CodeToTypeMap<Code>::Type;
        static_cast<UpdatableData<TargetType>*>(ptr)->update();
    }
};

// Execute action based on code
#define TRIGGER_UPDATE(Code) case Code: UpdateExe<Code>::execute(ptr); break;

void triggerUpdate(void* ptr, int code);
