#pragma once
#include "../lib.hpp"

const float SPEED_MULTIPLIER = 2.0f;

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

void install_speed_hack() {
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