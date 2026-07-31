#pragma once
#include "../lib.hpp"
#include <cmath>
#include <numbers>
#include <Config/config.hpp>
#include <Game/framework.hpp>
#include <Game/Actors/player.hpp>
#include <string>

HOOK_DEFINE_INLINE(BaseWalkSpeedMultiplier) {
    static void Callback(exl::hook::nx64::InlineFloatCtx* ctx) {
        ctx->S[0] = 1.0f * global_config.movement.speed;
    }
};

HOOK_DEFINE_INLINE(PowerWalkSpeedMultiplier) {
    static void Callback(exl::hook::nx64::InlineFloatCtx* ctx) {
        ctx->S[0] = 1.15f * global_config.movement.speed;
    }
};

HOOK_DEFINE_INLINE(BaseSwimSpeedMultiplier) {
    static void Callback(exl::hook::nx64::InlineFloatCtx* ctx) {
        ctx->S[2] = 1.0f * global_config.movement.speed;
    }
};

HOOK_DEFINE_INLINE(PowerSwimSpeedMultiplier) {
    static void Callback(exl::hook::nx64::InlineFloatCtx* ctx) {
        ctx->S[2] = 1.15f * global_config.movement.speed;
    }
};

// We replace the function for mapping 8 directions with our own code that maps to 360 degrees of movement
HOOK_DEFINE_REPLACE(PlayerLink__SnapDirection) {
    static int Callback(double arg1, float x, float y) {
        float angleRad = std::atan2f(y, x);
        int angleDeg = (int)std::floor(angleRad * 180.0 / std::numbers::pi);

        angleDeg += 90;
        if (angleDeg >= 360) {
            angleDeg -= 360;
        }

        return angleDeg * (int)std::round(0xffffffff / 360);
    }
};

namespace Controls {
    void InstallHooks() {
        EXL_ASSERT(global_config.initialized);

        if (global_config.movement.speed != 1.0f) {
            // walk
            BaseWalkSpeedMultiplier::InstallAtOffset(0xdcdea4);
            PowerWalkSpeedMultiplier::InstallAtOffset(0xdcff24);

            // shield walk
            BaseWalkSpeedMultiplier::InstallAtOffset(0xdb80a8);
            PowerWalkSpeedMultiplier::InstallAtOffset(0xdb8eac);

            // swim
            BaseSwimSpeedMultiplier::InstallAtOffset(0xdde288);
            PowerSwimSpeedMultiplier::InstallAtOffset(0xddf128);
        }

        if (global_config.movement.full_direction) {
            // 360 movement
            PlayerLink__SnapDirection::InstallAtOffset(0xded9f0);
        }
    }
}
