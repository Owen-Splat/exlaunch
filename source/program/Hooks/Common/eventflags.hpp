#pragma once
#include <string>

namespace EventFlags {
    bool CheckFlag(std::string flagName);
    void SetFlag(std::string flagName, bool flagState);
    void InstallHooks();
}
