#pragma once
#include <string>

namespace EventFlags {
    bool CheckGettingFlag(int itemID);
    void SetGettingFlag(int itemID);
    bool CheckFlag(std::string flagName);
    void SetFlag(std::string flagName, bool flagState);
    void InstallHooks();
}
