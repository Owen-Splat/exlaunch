#include "../lib.hpp"
#include <cmath>
#include <numbers>
#include <Config/config.hpp>
#include "Hooks/Common/input.hpp"

const float SPEED_MULTIPLIER = 1.15f;

HOOK_DEFINE_INLINE(BaseWalkSpeedMultiplier) {
    static void Callback(exl::hook::nx64::InlineFloatCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.speed_hack.enabled) {
            ctx->S[0] = 1.0f * SPEED_MULTIPLIER;
        }
    }
};

HOOK_DEFINE_INLINE(PowerWalkSpeedMultiplier) {
    static void Callback(exl::hook::nx64::InlineFloatCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.speed_hack.enabled) {
            ctx->S[0] = 1.15f + SPEED_MULTIPLIER;
        }
    }
};

HOOK_DEFINE_INLINE(BaseSwimSpeedMultiplier) {
    static void Callback(exl::hook::nx64::InlineFloatCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.speed_hack.enabled) {
            ctx->S[2] = 1.0f * SPEED_MULTIPLIER;
        }
    }
};

HOOK_DEFINE_INLINE(PowerSwimSpeedMultiplier) {
    static void Callback(exl::hook::nx64::InlineFloatCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.speed_hack.enabled) {
            ctx->S[2] = 1.15f * SPEED_MULTIPLIER;
        }
    }
};

// We replace the function for mapping 8 directions with our own code that maps to 360 degrees of movement
HOOK_DEFINE_REPLACE(PlayerLink__SnapDirection) {
    static int Callback(double arg1, float x, float y) {
        EXL_ASSERT(global_config.initialized);

        float angleRad = std::atan2f(y, x);
        int angleDeg = (int)std::floor(angleRad * 180.0 / std::numbers::pi);

        angleDeg += 90;
        if (angleDeg >= 360) {
            angleDeg -= 360;
        }

        if (global_config.control_scheme.movement == MovementMode::Standard) {
            angleDeg = (int)std::round(angleDeg / 45.0f) * 45;
        }

        return angleDeg * (int)std::round(0xffffffff / 360);
    }
};

namespace Controls {
    void InstallHooks() {
        // walk
        BaseWalkSpeedMultiplier::InstallAtOffset(0xdcdea4);
        PowerWalkSpeedMultiplier::InstallAtOffset(0xdcff24);

        // shield walk
        BaseWalkSpeedMultiplier::InstallAtOffset(0xdb80a8);
        PowerWalkSpeedMultiplier::InstallAtOffset(0xdb8eac);

        // swim
        BaseSwimSpeedMultiplier::InstallAtOffset(0xdde288);
        PowerSwimSpeedMultiplier::InstallAtOffset(0xddf128);

        // 360 movement
        PlayerLink__SnapDirection::InstallAtOffset(0xded9f0);
    }
}
