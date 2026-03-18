#include "Pointers/inventorypointers.hpp"

namespace InventorySystem {
    InventoryItems* Items = reinterpret_cast<InventoryItems*>(0xA1C46E0);

    void SetBaseItem(BaseItemBits item, bool set) {
        if (set) {
            Items->BaseItems |= ((uint32_t)1 << (uint32_t)item);
        }
        else {
            Items->BaseItems &= ~((uint32_t)1 << (uint32_t)item);
        }
    }
}
