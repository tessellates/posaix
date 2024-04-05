#include "TriggerUpdate.hpp"
#include "TranslationData.hpp"

#include <iostream>

/**
 * @brief Triggers the update function of UpdatableData<_Data> 
 *
 * Given any struct or class that is designed to be used with UpdatableData:
 * There should exist GENERATE_CODE_MAP(_Data, x) with x being a unique number every time.
 * TRIGGER_UPDATE(_Datax) should be added to the switch statement.
 * @param ptr pointer that will be statically cast to a template instance
 * @param code associated code with the template instance
 */
void triggerUpdate(void* ptr, int code) 
{
    switch (code) {
        TRIGGER_UPDATE(TranslationData1)
        default: std::cout << "Unknown code\n"; break;
    }
}
