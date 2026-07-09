#pragma once
#include "../lib.hpp"
#include "Hooks/Common/actorswitch.hpp"
#include "Hooks/Common/eventflags.hpp"
#include <string>

// AreaShellDropper call drop table
HOOK_DEFINE_INLINE(AreaShellDropper__FetchItemDrop) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        long arg1 = ctx->X[19];
        int index = *(int*)(arg1 + 0x2c8);
        if (index == 29) {
            // taltal-heights-hole (outside d4)
            EventFlags::SetFlag("HeightsHoleGet", true);
        }
        else if (index == 31) {
            // taltal-bomb-hole (by the bridge in the mountains)
            EventFlags::SetFlag("BridgeHoleGet", true);
        }
    }
};

// Force value to 0 (false) so that it thinks the player does not have the shell
// Otherwise the AreaShellDropper actor will get deleted
HOOK_DEFINE_INLINE(AreaShellDropper__CheckShell) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[9] = 0;
    }
};

// Function running every frame that checks if the player has the shell and deletes itself if so
// Not sure why this exists when the game checks separately when a bomb explodes
// We can just replace it with an empty func
HOOK_DEFINE_REPLACE(AreaShellDropper__Delete) {
    static void Callback(long arg1) {

    }
};

// Tree call drop table
HOOK_DEFINE_TRAMPOLINE(ObjTree_CallDropTableOnBonk) {
    static void Callback(long arg1, long arg2, long arg3, long arg4, long arg5) {
        u64 hash = *(u64*)(arg1 + 0x98);
        switch (hash) {
            case 7061688197741586161:
                EventFlags::SetFlag("BeachBonkTreeGet", true);
                break;
            case 8430773688343466954:
                EventFlags::SetFlag("TailCaveBonkTreeGet", true);
                break;
            case 5188616263732613924:
                EventFlags::SetFlag("UkukuBonkTreeGet", true);
                break;
            default:
                // unrandomized trees will function as normal
                Orig(arg1, arg2, arg3, arg4, arg5);
                break;
        }
    }
};

// grass - TODO - test flag = TreasureBoxOpenField04E
HOOK_DEFINE_TRAMPOLINE(GrassLiftedBreak) {
    static void Callback(unsigned long arg1, char arg2) {
        u64 hash = *(u64*)(arg1 + 0x98);
        switch (hash) {
            case 7638262199744254410:
                EventFlags::SetFlag("MabeBushGet", true);
                break;
            case 7638228114883793354:
                EventFlags::SetFlag("PondIslandBushGet", true);
                break;
            case 7638220418302398921:
                EventFlags::SetFlag("CoastIslandBushGet", true);
                break;
            case 7638329269953548747:
                EventFlags::SetFlag("BayBushGet", true);
                break;
            case 7638382046511681996:
                EventFlags::SetFlag("MansionBushGet", true);
                break;
            default:
                Orig(arg1, arg2);
                break;
        }
    }
};

// This is called when a rock is lifted and again when it breaks
// we set the flag on lift, so nothing will happen when it tries to set again on break
HOOK_DEFINE_TRAMPOLINE(RockLiftedBreak) {
    static unsigned long Callback(unsigned long arg1, char arg2) {
        u64 hash = *(u64*)(arg1 + 0x98);
        switch (hash) {
            case 7638141253465199046:
                EventFlags::SetFlag("MoblinCaveRockGet", true);
                break;
            case 7638196229046587848:
                EventFlags::SetFlag("DesertSouthRockGet", true);
                break;
            case 7638170940279148999:
                EventFlags::SetFlag("RockMazeRockGet", true);
                break;
            case 7638364454325637580:
                EventFlags::SetFlag("PlainsRockGet", true);
                break;
            case 7638117064209387967:
                EventFlags::SetFlag("TaltalWestRockGet", true);
                break;
            case 7638155547116360135:
                EventFlags::SetFlag("TaltalEastRockGet", true);
                break;
            default:
                return Orig(arg1, arg2);
        }
        return arg1;
    }
};

namespace SeashellRandomizer {
    void InstallHooks() {
        AreaShellDropper__FetchItemDrop::InstallAtOffset(0x856fc);
        AreaShellDropper__CheckShell::InstallAtOffset(0x84a44);
        AreaShellDropper__Delete::InstallAtOffset(0x85510);
        ObjTree_CallDropTableOnBonk::InstallAtOffset(0xcb1f30);
        GrassLiftedBreak::InstallAtOffset(0xba63e0);
        RockLiftedBreak::InstallAtOffset(0xc49b10);
    }
}
