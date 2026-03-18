#include "Pointers/inventorypointers.hpp"

namespace InventorySystem {
    InventoryItems* Items = reinterpret_cast<InventoryItems*>(0xA1C46E0);
    uint8_t* Level = reinterpret_cast<uint8_t*>(0xA1C53C8); // store here since we need the index for dungeon items

    void SetBaseItem(BaseItemBits item, bool set) {
        if (set) {
            Items->BaseItems |= ((uint32_t)1 << (uint32_t)item);
        }
        else {
            Items->BaseItems &= ~((uint32_t)1 << (uint32_t)item);
        }
    }
}
