#include "lib.hpp"
#include "nn/hid.h"
#include "debug.hpp"
#include "Config/config.hpp"
#include "Hooks/Common/input.hpp"
#include <string>

// Store previous and current buttons to be able to tell when a button was just pressed rather than held
nn::hid::NpadButtonSet prevButtons;
nn::hid::NpadButtonSet newButtons;

nn::hid::AnalogStickState mapDpadToStick() {
    bool up = newButtons.Test((int)InputSystem::NpadButton::Up);
    bool right = newButtons.Test((int)InputSystem::NpadButton::Right);
    bool down = newButtons.Test((int)InputSystem::NpadButton::Down);
    bool left = newButtons.Test((int)InputSystem::NpadButton::Left);

    nn::hid::AnalogStickState modStick{};

    if (up) modStick.mY = 0x7FFFFFFF;
    if (right) modStick.mX = 0x7FFFFFFF;
    if (down) modStick.mY = -0x7FFFFFFF;
    if (left) modStick.mX = -0x7FFFFFFF;

    if (up && right) {
        modStick.mX = 0x3FFFFFFF;
        modStick.mY = 0x3FFFFFFF;
    }
    if (up && left) {
        modStick.mX = -0x3FFFFFFF;
        modStick.mY = 0x3FFFFFFF;
    }
    if (down && right) {
        modStick.mX = 0x3FFFFFFF;
        modStick.mY = -0x3FFFFFFF;
    }
    if (down && left) {
        modStick.mX = -0x3FFFFFFF;
        modStick.mY = -0x3FFFFFFF;
    }

    return modStick;
}

HOOK_DEFINE_TRAMPOLINE(GetNpadState__FullKeyState) {
    static void Callback(nn::hid::NpadFullKeyState* arg1, uint port) {
        Orig(arg1, port);
        EXL_ASSERT(global_config.initialized);
        if (arg1 != nullptr) {
            InputSystem::MainLoop(*arg1);
            if (global_config.control_scheme.movement == MovementMode::DPad) {
                (*arg1).mAnalogStickL = mapDpadToStick();
            }
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

    void MainLoop(nn::hid::NpadFullKeyState pad) {
        prevButtons = newButtons;
        newButtons = pad.mButtons;
    }

    void InstallHooks() {
        GetNpadState__FullKeyState::InstallAtOffset(0x1431a50);
    }
}
