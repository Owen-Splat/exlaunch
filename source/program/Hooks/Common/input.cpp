#include "lib.hpp"
#include "nn/hid.h"
#include "Hooks/Common/input.hpp"

// Store the current NpadButtonSet. This is called once every frame
nn::hid::NpadButtonSet g_Buttons;
HOOK_DEFINE_TRAMPOLINE(GetNpadState__FullKeyState) {
    static void Callback(nn::hid::NpadFullKeyState* arg1, uint port) {
        Orig(arg1, port);
        if (arg1 != nullptr) {
            g_Buttons = (*arg1).mButtons;
        }
    }
};

namespace InputSystem {
    bool IsRightStickPressed() {
        return g_Buttons.Test((int)nn::hid::NpadButton::StickR);
    }

    bool IsDebugComboHeld() {
        bool upHeld = g_Buttons.Test((int)nn::hid::NpadButton::Up);
        bool bHeld = g_Buttons.Test((int)nn::hid::NpadButton::B);
        bool xHeld = g_Buttons.Test((int)nn::hid::NpadButton::X);
        if (upHeld && bHeld && xHeld) {
            return true;
        }
        else {
            return false;
        }
    }

    void InstallHooks() {
        GetNpadState__FullKeyState::InstallAtOffset(0x1431a50);
    }
}
