#pragma once
#include "../lib.hpp"

/*
    The PlayReports are disabled so that telemetry does not get sent
    They have never banned anyone for obvious modding using this telemetry
    But it is still nice to disable
    TODO - create a system for mod developers to have their own PlayReports
*/

HOOK_DEFINE_REPLACE(PlayReport__Add) {
    static void Callback(long arg1, long arg2) {
        
    }
};

HOOK_DEFINE_REPLACE(PlayReport__Save) {
    static void Callback(long arg1) {
        
    }
};

namespace PlayReport {
    void InstallHooks() {
        PlayReport__Add::InstallAtOffset(0x1432450);
        PlayReport__Save::InstallAtOffset(0x1432460);
    }
}