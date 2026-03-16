#include "lib.hpp"
#include "nn/hid.h"
#include "Hooks/Common/input.hpp"

// Store the current NpadButtonSet. This is called once every frame
nn::hid::NpadButtonSet prevButtons;
nn::hid::NpadButtonSet newButtons;
HOOK_DEFINE_TRAMPOLINE(GetNpadState__FullKeyState) {
    static void Callback(nn::hid::NpadFullKeyState* arg1, uint port) {
        Orig(arg1, port);
        if (arg1 != nullptr) {
            prevButtons = newButtons;
            newButtons = (*arg1).mButtons;
        }
    }
};

namespace InputSystem {
    bool IsButtonPressed(NpadButton button) {
        return newButtons.Test((int)button);
    }
    bool IsButtonJustPressed(NpadButton button) {
        return prevButtons.Test((int)button) == false && newButtons.Test((int)button) == true;
    }
    bool IsButtonJustReleased(NpadButton button) {
        return prevButtons.Test((int)button) == true && newButtons.Test((int)button) == false;
    }

    bool IsDebugComboHeld() {
        bool upHeld = IsButtonPressed(NpadButton::Up);
        bool bHeld = IsButtonPressed(NpadButton::B);
        bool xHeld = IsButtonPressed(NpadButton::X);
        return upHeld && bHeld && xHeld;
    }

    void InstallHooks() {
        GetNpadState__FullKeyState::InstallAtOffset(0x1431a50);
    }
}
