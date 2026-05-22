#pragma once
#include "../lib.hpp"
#include <string>

// Instead of Seashell{0}, we force AreaShellDropper to use our own Randomizer{0} drop entry
HOOK_DEFINE_INLINE(AreaShellDropper__FetchItemDrop) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        std::string entry = "Randomizer{0}";
        ctx->X[8] = (long)&entry;
    }
};

// Force value to 0 so that it thinks the player does not have the shell
// Otherwise the AreaShellDropper actor will get deleted
HOOK_DEFINE_INLINE(AreaShellDropper__CheckShell) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[9] = 0;
    }
};

// Function that checks if the player has the shell and deletes itself if so
// Not sure why this exists when the game checks separately when a bomb explodes
// We can just replace it with an empty func
HOOK_DEFINE_REPLACE(AreaShellDropper__Delete) {
    static void Callback(long arg1) {

    }
};

namespace SeashellRandomizer {
    void InstallHooks() {
        AreaShellDropper__FetchItemDrop::InstallAtOffset(0x855c0);
        AreaShellDropper__CheckShell::InstallAtOffset(0x84a44);
        AreaShellDropper__Delete::InstallAtOffset(0x85510);
    }
}
