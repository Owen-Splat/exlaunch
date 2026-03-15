#include "../lib.hpp"
#include "Pointers/inventory.hpp"
#include <string>
#include <unordered_map>

const std::unordered_map<int, std::string> itemsDict = {
    {0,     "SwordLv1"},
    {1,     "SwordLv2"},
    {2,     "Shield"},
    {3,     "MirrorShield"},
    {4,     "Bomb"},
    {5,     "Bow"},
    {6,     "HookShot"},
    {7,     "Boomerang"},
    {8,     "MagicRod"},
    {9,     "Shovel"},
    {10,    "SleepyMushroom"},
    {11,    "MagicPowder"},
    {12,    "RocsFeather"},
    {13,    "PowerBraceletLv1"},
    {14,    "PowerBraceletLv2"},
    {15,    "PegasusBoots"},
    {16,    "Ocarina"},
    {17,    "ClothesGreen"},
    {18,    "ClothesRed"},
    {19,    "ClothesBlue"},
    {20,    "Flippers"},
    {21,    "SecretMedicine"},
    {22,    "Seashell"},
    {23,    "GoldenLeaf"},
    {24,    "TailKey"},
    {25,    "SlimeKey"},
    {26,    "AnglerKey"},
    {27,    "FaceKey"},
    {28,    "BirdKey"},
    {29,    "YoshiDoll"},
    {30,    "Ribbon"},
    {31,    "DogFood"},
    {32,    "Bananas"},
    {33,    "Stick"},
    {34,    "Honeycomb"},
    {35,    "Pineapple"},
    {36,    "Hibiscus"},
    {37,    "Letter"},
    {38,    "Broom"},
    {39,    "FishingHook"},
    {40,    "PinkBra"},
    {42,    "MermaidsScale"},
    {43,    "MagnifyingLens"},
    {44,    "Compass"},
    {45,    "DungeonMap"},
    {47,    "StoneBeak"},
    {48,    "SmallKey"},
    {49,    "NightmareKey"},
    {50,    "FullMoonCello"},
    {51,    "ConchHorn"},
    {52,    "SeaLilysBell"},
    {53,    "SurfHarp"},
    {54,    "WindMarimba"},
    {55,    "CoralTriangle"},
    {56,    "EveningCalmOrgan"},
    {57,    "ThunderDrum"},
    {58,    "HeartPiece"},
    {59,    "HeartContainer"},
    {60,    "Arrow"},
    {61,    "Apple"},
    {62,    "GreenApple"},
    {100,   "Heart"},
    {101,   "Rupee1"},
    {102,   "Rupee5"},
    {104,   "Rupee20"},
    {105,   "Rupee50"},
    {106,   "Rupee100"},
    {108,   "Rupee300"},
    {110,   "Fairy"},
    {111,   "DefenceUp"},
    {112,   "PowerUp"},
    {113,   "Bottle"},
    {114,   "PanelDungeonPiece"},
    {115,   "PanelDungeonPieceSet"},
    {116,   "ShellRader"},
    {117,   "PanelDungeonPlusChip"},
    {120,   "Song_WindFish"},
    {121,   "Song_Mambo"},
    {122,   "Song_Soul"},
    {123,   "MagicPowder_MaxUp"},
    {124,   "Bomb_MaxUp"},
    {125,   "Arrow_MaxUp"},
    {126,   "BottleFairy"}
};

// The purpose of this is to have our own code run for specific items
// One example would be that getting SurfHarp needs to set 4 flags to disable the ghost
// Other items will just use the original code
HOOK_DEFINE_TRAMPOLINE(Inventory__AddItemID) {
    static void Callback(int ID, int count, uint index) {
        bool added = true;

        std::string itemName = itemsDict.at(ID);
        std::string indexStr = "";
        if (ID == 22 || ID == 23 || ID == 58 || ID == 59 || ID == 114 || ID == 115 || ID == 116) {
            indexStr = "[" + std::to_string(index) + "]";
        }
        Logging.Log("Player has obtained '" + itemName + indexStr + "'");

        switch (ID) {
            default:
                added = false;
                break;
        }

        if (!added) {
            Orig(ID, count, index);
        }
    }
};

// Normally, the game uses a signed 8 bit int for the ID arg
// This is an issue if we want to add custom items because we cannot surpass 127 total items
// For the standalone randomizer, there are enough unused IDs to add whatever is needed
// But for future multiworld support, we will need more
HOOK_DEFINE_TRAMPOLINE(Inventory__HasItemID) {
    static bool Callback(int ID, int count, int index) {
        if (ID <= 127) {
            return Orig((uint8_t)ID, count, index);
        }

        switch (ID) {
            default:
                return false;
        }
    }
};

namespace Inventory {
    void GiveDebugItems() {
        for (int i = 0; i < 58; i++) {
            if (i == 41 || i == 46) {
                continue;
            }
            Inventory__AddItemID::Callback(i, 1, -1);
        }
    }

    void AddItemID(int ID, int count, int index) {
        Inventory__AddItemID::Callback(ID, count, index);
    }

    bool HasItemID(int ID, int count, int index) {
        return Inventory__HasItemID::Callback(ID, count, index);
    }

    void InstallHooks() {
        Inventory__AddItemID::InstallAtOffset(0x8d08e0);
        Inventory__HasItemID::InstallAtOffset(0x8d1a30);
    }
}
