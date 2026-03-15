#include "../lib.hpp"
#include <cmath>
#include <numbers>
#include "Hooks/Common/input.hpp"

// We replace the function for mapping 8 directions with our own code that maps to 360 degrees of movement
bool snap = true;
int inputBuffer = 0;
HOOK_DEFINE_REPLACE(PlayerLink__SnapDirection) {
    static int Callback(double arg1, float x, float y) {
        float angleRad = std::atan2f(y, x);
        int angleDeg = (int)std::floor(angleRad * 180.0 / std::numbers::pi);

        angleDeg += 90;
        if (angleDeg >= 360) {
            angleDeg -= 360;
        }

        if (InputSystem::IsRightStickPressed()) {
            if (inputBuffer == 0) {
                snap = !snap;
            }
            inputBuffer++;
        }
        else {
            inputBuffer = 0;
        }

        if (snap) {
            angleDeg = (int)std::round(angleDeg / 45.0f) * 45;
        }

        return angleDeg * (int)std::round(0xffffffff / 360);
    }
};

namespace Controls {
    void InstallHooks() {
        PlayerLink__SnapDirection::InstallAtOffset(0xded9f0);
    }
}
