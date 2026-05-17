#pragma once
#include "Hooks/Common/audio.hpp"
#include "Hooks/Common/eventflags.hpp"
#include "Hooks/Common/input.hpp"
#include "Hooks/Common/inventory.hpp"
#include "Hooks/Common/level.hpp"
#include "Hooks/Common/playreport.hpp"
#include "Hooks/Randomizer/companions.hpp"
#include "Hooks/Randomizer/enemies.hpp"
#include "Hooks/Randomizer/fishing.hpp"
#include "Hooks/Randomizer/markers.hpp"
#include "Hooks/Mods/controls.hpp"

namespace ModLoader {
    void InstallCommonHooks() {
        AudioSystem::InstallHooks();
        EventFlags::InstallHooks();
        InputSystem::InstallHooks();
        Inventory::InstallHooks();
        LevelSystem::InstallHooks();
        PlayReport::InstallHooks();
    }

    void InstallRandomizerHooks() {
        // CompanionTweaks::InstallHooks(); // unfinished
        EnemyRandomizer::InstallHooks();
        FishingTweaks::InstallHooks();
        // MarkerTweaks::InstallHooks(); // unfinished
    }

    void InstallMods() {
        Controls::InstallHooks();
    }
}