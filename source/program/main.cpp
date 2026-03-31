#include "lib.hpp"
#include "debug.hpp"
#include "patches.hpp"
#include "err.hpp"
#include "Config/config.hpp"
#include "Config/file_handler.hpp"
#include "Hooks/Common/audio.hpp"
#include "Hooks/Common/eventflags.hpp"
#include "Hooks/Common/input.hpp"
#include "Hooks/Common/inventory.hpp"
#include "Hooks/Common/level.hpp"
#include "Hooks/Mods/controls.hpp"
#include "Hooks/Mods/itembehavior.hpp"
#include "Pointers/flags.hpp"
#include <string>

const exl::util::ModuleInfo g_MainInfo = exl::util::GetMainModuleInfo();
const uintptr_t g_BaseAddress = g_MainInfo.m_Total.m_Start;
const uintptr_t g_MainEnd = g_BaseAddress + g_MainInfo.m_Total.m_Size;

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
        // Logging.Log(std::to_string((long)g_BaseAddress));
        // Logging.Log(std::to_string((long)g_MainEnd));

        Orig(arg1, arg2);
        // DebugMode::Toggle(InputSystem::IsDebugComboHeld());
    }
};

PatchConfig global_config;

HOOK_DEFINE_TRAMPOLINE(nnMain){
    static void Callback(){
        R_ABORT_UNLESS(FileHandler::MountSD());
        std::string config_str;
        if (R_FAILED(FileHandler::ReadFile("sd:/config/lasr-exl/config.ini", config_str))) {
            nn::err::ApplicationErrorArg err(
                nn::err::MakeErrorCode(nn::err::ErrorCodeCategoryType::unk1, 0x420), "Failed to read config!",
                "Please ensure 'sd:/config/lasr-exl/config.ini' exists",
                nn::settings::LanguageCode::Make(nn::settings::Language::Language_English)
            );
            nn::err::ShowApplicationError(err);
            EXL_ABORT("Failed to read config.");
        }
        global_config.parse(config_str);
        Orig();
    }
};

extern "C" void exl_main(void* x0, void* x1) {
    /* Setup hooking environment. */
    exl::hook::Initialize();
    nnMain::InstallAtOffset(0x1bbf0);

    // runCodePatches();

    // install common hooks
    AudioSystem::InstallHooks();
    EventFlags::InstallHooks();
    InputSystem::InstallHooks();
    Inventory::InstallHooks();
    LevelSystem::InstallHooks();

    // install mod hooks
    Controls::InstallHooks();
    ItemBehavior::InstallHooks();

    // Get rid of play report logging
    PlayReport__Add::InstallAtOffset(0x1432450);
    PlayReport__Save::InstallAtOffset(0x1432460);

    // install effects on player actor init
    PlayerLink__Init::InstallAtOffset(0xd2bc50);
}

extern "C" NORETURN void exl_exception_entry() {
    /* Note: this is only applicable in the context of applets/sysmodules. */
    EXL_ABORT("Default exception handler called!");
}
