#include "Pointers/inventorypointers.hpp"

namespace InventorySystem {
    InventoryItems* Items = reinterpret_cast<InventoryItems*>(0xA1C46E0);
    uint8_t* Level = reinterpret_cast<uint8_t*>(0xA1C53C8); // store here since we need the index for dungeon items
}
