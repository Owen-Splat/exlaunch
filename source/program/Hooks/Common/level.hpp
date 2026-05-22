#pragma once
#include "../lib.hpp"
#include "Config/config.hpp"
#include "Hooks/Common/eventflags.hpp"
#include "Game/Data/inventory.hpp"
#include <string>

inline std::string prevLevel = "";
inline std::string currentLevel = "";

HOOK_DEFINE_TRAMPOLINE(SetCurrentLevel) {
    static void Callback(long arg1, long arg2, long arg3, long arg4, long arg5) {
        EXL_ASSERT(global_config.initialized);
        Orig(arg1, arg2, arg3, arg4, arg5);
        prevLevel = currentLevel;
        std::string* levelNamePtr = reinterpret_cast<std::string*>(arg1);
        currentLevel = *levelNamePtr;
        if (global_config.debug_mode.enabled) {
            Logging.Log(currentLevel);
        }
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
