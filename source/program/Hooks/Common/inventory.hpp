#pragma once

namespace Inventory {
    void AddItemID(int ID, int count, int index);
    bool HasItemID(int ID, int count, int index);
    void InstallHooks();
}
