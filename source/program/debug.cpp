#include "lib.hpp"
#include "Hooks/Common/inventory.hpp"

namespace DebugMode {
    bool enabled = false;

    void Toggle(bool on) {
        if (enabled != on) {
            if (on) {
                Logging.Log("DEBUG MODE ENABLED!");
            }
            else {
                Logging.Log("DEBUG MODE DISABLED!");
            }
        }
        if (on) {
            Inventory::GiveDebugItems();
        }
        enabled = on;
    }
}
