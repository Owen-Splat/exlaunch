#include "lib.hpp"
#include "patches.hpp"
#include "Hooks/Common/inventory.hpp"
#include "Hooks/Common/eventflags.hpp"
#include "Hooks/Mods/controls.hpp"
#include "Hooks/Mods/itembehavior.hpp"

// Get rid of play reports
HOOK_DEFINE_REPLACE(PlayReport__Add) {
    static void Callback(long arg1, long arg2) {

    }
};

HOOK_DEFINE_REPLACE(PlayReport__Save) {
    static void Callback(long arg1) {

    }
};

// Do stuff once per file load
HOOK_DEFINE_TRAMPOLINE(PlayerLink__Init) {
    static void Callback(long arg1, long arg2) {
        Orig(arg1, arg2);
        Inventory::AddItemID(8, 1, -1);
        Inventory::AddItemID(6, 1, -1);
    }
};

extern "C" void exl_main(void* x0, void* x1) {
    /* Setup hooking environment. */
    exl::hook::Initialize();

    runCodePatches();

    // install common hooks
    EventFlags::InstallHooks();
    Inventory::InstallHooks();

    // install mod hooks
    Controls::InstallHooks();
    ItemBehavior::InstallHooks();

    // install effects on player actor init
    PlayReport__Save::InstallAtOffset(0x1432460);
    PlayerLink__Init::InstallAtOffset(0xd2bc50);
}

extern "C" NORETURN void exl_exception_entry() {
    /* Note: this is only applicable in the context of applets/sysmodules. */
    EXL_ABORT("Default exception handler called!");
}
