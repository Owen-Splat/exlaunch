#include "../lib.hpp"
#include <cmath>
#include <numbers>

// We replace the function for mapping 8 directions with our own code that maps to 360 degrees of movement
HOOK_DEFINE_REPLACE(PlayerLink__SnapDirection) {
    static int Callback(double arg1, float x, float y) {
        float angle_rad = std::atan2f(y, x);
        int angle_deg = (int)std::floor(angle_rad * 180.0 / std::numbers::pi);

        angle_deg += 90;
        if (angle_deg >= 360) {
            angle_deg -= 360;
        }

        return angle_deg * (0xffffffff / 360);
    }
};

namespace Controls {
    void InstallHooks() {
        PlayerLink__SnapDirection::InstallAtOffset(0xded9f0);
    }
}
