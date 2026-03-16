#include "lib.hpp"
#include "Hooks/Common/inventory.hpp"
#include "Hooks/Common/audio.hpp"

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
            AudioSystem::PlaySoundEffect("SE_MAP_CUCCO_ANGRY_VOICE");
        }
        enabled = on;
    }
}
