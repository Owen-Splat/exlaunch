#pragma once
#include "lib.hpp"
#include "nn/hid.h"

extern nn::hid::NpadButtonSet g_Buttons;

namespace InputSystem {
    bool IsRightStickPressed();
    bool IsDebugComboHeld();
    void InstallHooks();
}
