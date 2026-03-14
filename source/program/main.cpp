#include "lib.hpp"
#include "patches.hpp"
#include "Pointers/flags.h"
#include "Pointers/inventory.h"
#include "Pointers/level.h"
#include <string>
#include <sstream>
#include <cmath>
#include <numbers>
#include <iomanip>
#include <limits.h>
#include <random>
#include <vector>

// // the vanilla instruction where this is installed runs as normal after func
// // so we actually want to install at +4 of the place we want to insert custom code
// HOOK_DEFINE_INLINE(PlayerLink__PlaceBomb) {
//     static void Callback(exl::hook::InlineCtx *ctx) {
//         std::vector<uint16_t> IDs = {0x1, 0x2, 0x3, 0x4, 0x5, 0xf5, 0xf6, 0xfa, 0x100, 0x104, 0x115};

//         std::mt19937 rng(exl::util::GetRandomU64());
//         std::uniform_int_distribution<std::size_t> index_dist(0, IDs.size() - 1);
//         std::size_t random_index = index_dist(rng);
//         uint16_t random_element = IDs[random_index];

//         ctx->W[9] = random_element;
//     }
// };

// Map this function so we can use it
HOOK_DEFINE_TRAMPOLINE(EventFlags__SetFlag) {
    static void Callback(long arg1, long arg2, uint arg3) {
        Orig(arg1, arg2, arg3);
    }
};

// Map this function so we can use it. We have pointers for items but this is simpler to use
HOOK_DEFINE_TRAMPOLINE(Inventory__AddItemID) {
    static void Callback(uint ID, int count, ulong index) {
        Orig(ID, count, index);
    }
};

// // Flag that gets set after talking to Tarin that allows the player to pause
// std::string flagName = "ShieldGet";
// // Called once during file load
// HOOK_DEFINE_TRAMPOLINE(PlayerLink__Init) {
//     static void Callback(long arg1, long arg2) {
//         Orig(arg1, arg2);
//         *g_BaseItems = 0xffffffff;
//         EventFlags__SetFlag::Callback((long)g_EventFlags, (long)&flagName, 1);
//     }
// };

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

extern "C" void exl_main(void* x0, void* x1) {
    /* Setup hooking environment. */
    exl::hook::Initialize();

    // runCodePatches();

    Inventory__AddItemID::InstallAtOffset(0x8d08e0);
    // PlayerLink__Init::InstallAtOffset(0xd2bc50);
    PlayerLink__SnapDirection::InstallAtOffset(0xded9f0);
    EventFlags__SetFlag::InstallAtOffset(0xdfa7b0);
}

extern "C" NORETURN void exl_exception_entry() {
    /* Note: this is only applicable in the context of applets/sysmodules. */
    EXL_ABORT("Default exception handler called!");
}
