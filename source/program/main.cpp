#include "main.h"
#include "patches.hpp"
#include <string>
#include <sstream>
#include <cmath>
#include <numbers>
#include <iomanip>

// example of using it
// int* rupeeCountPtr = getPointer(g_inventory, 0x80);
// *rupeeCountPtr += 1;
int* getPointer(uintptr_t addr, uintptr_t offset) {
    return reinterpret_cast<int*>(addr + offset);
}

HOOK_DEFINE_REPLACE(PlayerLink__SnapDirection) {
    static int Callback(double arg1, float x, float y) {
        float angle_rad = std::atan2f(y, x);
        int angle_deg = (int)std::floor(angle_rad * 180.0 / std::numbers::pi);

        angle_deg += 90;
        if (angle_deg > 360) {
            angle_deg -= 360;
        }

        return angle_deg * (0xffffffff / 360);
    }
};

extern "C" void exl_main(void* x0, void* x1) {
    /* Setup hooking environment. */
    exl::hook::Initialize();

    // runCodePatches();

    PlayerLink__SnapDirection::InstallAtOffset(0xded9f0);
}

extern "C" NORETURN void exl_exception_entry() {
    /* Note: this is only applicable in the context of applets/sysmodules. */
    EXL_ABORT("Default exception handler called!");
}
