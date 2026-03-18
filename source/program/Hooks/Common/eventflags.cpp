#include "../lib.hpp"
#include "debug.hpp"
#include "Pointers/flags.hpp"
#include <string>

HOOK_DEFINE_TRAMPOLINE(EventFlags__CheckFlag) {
    static bool Callback(long arg1, long strAddr) {
        return Orig(arg1, strAddr);
    }
};

HOOK_DEFINE_TRAMPOLINE(EventFlags__SetFlag) {
    static void Callback(long arg1, long strAddr, uint arg3) {
        bool flagState = !!arg3;
        if (EventFlags__CheckFlag::Callback(arg1, strAddr) != flagState) {
            Orig(arg1, strAddr, arg3);
            if (DebugMode::enabled) {
                std::string* strPtr = reinterpret_cast<std::string*>(strAddr);
                std::string stateStr = flagState ? "true" : "false";
                Logging.Log("Set flag '" + *strPtr + "' to " + stateStr);
            }
        }
    }
};

namespace EventFlags {
    bool CheckFlag(std::string flagName) {
        return EventFlags__CheckFlag::Callback((long)g_EventFlags, (long)&flagName);
    }

    void SetFlag(std::string flagName, bool flagState) {
        EventFlags__SetFlag::Callback((long)g_EventFlags, (long)&flagName, (long)flagState);
    }

    void InstallHooks() {
        EventFlags__CheckFlag::InstallAtOffset(0xdfa920);
        EventFlags__SetFlag::InstallAtOffset(0xdfa7b0);
    }
}
