#include "../lib.hpp"
#include "debug.hpp"
#include <string>

std::string* currentLevel = nullptr;
HOOK_DEFINE_TRAMPOLINE(SetCurrentLevel) {
    static void Callback(long arg1, long arg2, long arg3, long arg4, long arg5) {
        Orig(arg1, arg2, arg3, arg4, arg5);
        currentLevel = reinterpret_cast<std::string*>(arg1);
        if (DebugMode::enabled) {
            Logging.Log(*currentLevel);
        }
    }
};

namespace LevelSystem {
    std::string GetCurrentLevel() {
        if (currentLevel == nullptr) {
            return "NULL";
        }
        return *currentLevel;
    }

    void InstallHooks() {
        SetCurrentLevel::InstallAtOffset(0xdfa450);
    }
}