#pragma once
#include "../lib.hpp"
#include "Hooks/Common/eventflags.hpp"
#include "Config/config.hpp"
#include <string>

inline const std::string heart_flags[] = {
    "MabeWellHeartGet",
    "SmallFishFirstGet",
    "DiamondIslandHeartGet"
}

HOOK_DEFINE_INLINE(CheckDisplayHeartPiece) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.randomizer.enabled) {
            // Check global flag at heart_flags[index] instead of HeartPiece[index]
            // Reverse condition so that the markers show what the player is missing
            long value = 0;
            int index = ctx->X[9];
            if (!EventFlags::CheckFlag(heart_flags[index]))
            {
                ctx->X[8] = 1ULL << index;
            }
            else {
                ctx->X[8] = 0;
            }
        }
    }
};

namespace MarkerTweaks {
    void installHooks() {
        CheckDisplayHeartPiece::InstallAtOffset(0xe91c28);
    }
}
