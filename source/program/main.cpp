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
#include "Hooks/Randomizer/enemies.hpp"
#include <string>

// Get rid of play reports
HOOK_DEFINE_REPLACE(PlayReport__Add) {
    static void Callback(long arg1, long arg2) {
        
    }
};

HOOK_DEFINE_REPLACE(PlayReport__Save) {
    static void Callback(long arg1) {
        
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
        runCodePatches();
        Orig();
    }
};

extern "C" void exl_main(void* x0, void* x1) {
    /* Setup hooking environment. */
    exl::hook::Initialize();
    nnMain::InstallAtOffset(0x1bbf0);

    // install common hooks
    AudioSystem::InstallHooks();
    EventFlags::InstallHooks();
    InputSystem::InstallHooks();
    Inventory::InstallHooks();
    LevelSystem::InstallHooks();

    // install mod hooks
    Controls::InstallHooks();

    // install randomizer specific hooks
    EnemyRandomizer::installHooks();

    // Get rid of play report logging
    PlayReport__Add::InstallAtOffset(0x1432450);
    PlayReport__Save::InstallAtOffset(0x1432460);
}

extern "C" NORETURN void exl_exception_entry() {
    /* Note: this is only applicable in the context of applets/sysmodules. */
    EXL_ABORT("Default exception handler called!");
}
