#include "../lib.hpp"
#include "debug.hpp"
#include "Hooks/Common/eventflags.hpp"
#include "Pointers/inventorypointers.hpp"
#include <string>

std::string prevLevel = "";
std::string currentLevel = "";

// It would be too much of a pain to make the FishingBottle actor check for a flag instead of inventory bottle
// Instead we can just add and remove the bottle during the load
void FixFishingBottle() {
    if (currentLevel == "Field") {
        if (EventFlags::CheckFlag("Bottle1Get")) { // flag set when the player obtains bottle index 1
            InventorySystem::Items->MainItems.Bottle1 = true;
        }
        else {
            InventorySystem::Items->MainItems.Bottle1 = false;
        }
    }
    else if (currentLevel == "FishingPond") {
        if (EventFlags::CheckFlag("FishingBottleGet")) { // flag set when the player gets the item check for fishing bottle
            InventorySystem::Items->MainItems.Bottle1 = true;
        }
        else {
            InventorySystem::Items->MainItems.Bottle1 = true;
        }
    }
}

HOOK_DEFINE_TRAMPOLINE(SetCurrentLevel) {
    static void Callback(long arg1, long arg2, long arg3, long arg4, long arg5) {
        Orig(arg1, arg2, arg3, arg4, arg5);
        prevLevel = currentLevel;
        std::string* levelNamePtr = reinterpret_cast<std::string*>(arg1);
        currentLevel = *levelNamePtr;
        if (DebugMode::enabled) {
            Logging.Log(currentLevel);
        }
        FixFishingBottle();
    }
};

namespace LevelSystem {
    std::string GetCurrentLevel() {
        return currentLevel;
    }

    void InstallHooks() {
        SetCurrentLevel::InstallAtOffset(0xdfa450);
    }
}