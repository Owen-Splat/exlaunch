#pragma once

extern std::string currentLevel;

namespace LevelSystem {
    std::string GetCurrentLevel();
    void InstallHooks();
}
