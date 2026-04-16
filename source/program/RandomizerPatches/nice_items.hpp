#pragma once
#include "../lib.hpp"

// removes limit on number of bombs placed at once
HOOK_DEFINE_INLINE(NiceBombs) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.nice_items.enabled) {
            // game checks if player is allowed to place a bomb, we hardcode it to true
            ctx->W[8] = 1;
        }
    }
};

// hookshot any surface
HOOK_DEFINE_INLINE(NiceHookshot) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.nice_items.enabled) {
            // sets hook state to "pierce" surfaces instead of "hit"
            ctx->W[20] = 3;
        }
    }
};

// removes limit on number of projectiles at once
HOOK_DEFINE_INLINE(NiceMagicRod) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.nice_items.enabled) {
            // changes the projectile count limit from 3 to 16, 16 is just a safe number that the shoot speed cant reach
            ctx->x[19] = 16;
        }
    }
};

void install_nice_items_patch() {
    NiceBombs::InstallAtOffset(0xd5295c);
    NiceHookshot::InstallAtOffset(0xd7f18c);
    NiceMagicRod::InstallAtOffset(0xd5169c);
}
