#pragma once
#include <cstdint>

namespace InventorySystem {
    enum class Companion : uint8_t {
        Alone,
        BowWow,
        Marin,
        Ghost,
        Rooster
    };

    enum class BaseItemBits {
        SwordLv1,
        Shield,
        Bomb,
        Bow,
        Hookshot,
        Boomerang,
        MagicRod,
        Shovel,
        MagicPowder,
        RocsFeather,
        PowerBraceletLv1,
        PegasusBoots,
        Ocarina
    };

    enum class ExtraItemBits {
        Flippers,
        Medicine,
        TailKey,
        SlimeKey,
        AnglerKey,
        FaceKey,
        BirdKey,
        FullMoonCello,
        ConchHorn,
        SeaLilysBell,
        SurfHarp,
        WindMarimba,
        CoralTriangle,
        EveningCalmOrgan,
        ThunderDrum,
        SleepyMushroom,
        ShellRader
    };

    enum class UpgradedItemBits {
        SwordLv2,
        MirrorShield,
        PowerBraceletLv2,
        MagicPowder_MaxUp,
        Bomb_MaxUp,
        Arrow_MaxUp,
        ClothesRed,
        ClothesBlue
    };

    struct InventoryItems {
        char unk1[0x80];
        int Rupees; // 0x80
        short Health; // 0x84
        char unk2[0x2];
        uint64_t Seashells; // 0x88
        uint64_t HeartPieces; // 0x90
        uint16_t HeartContainers; // 0x98
        uint8_t TradeItem; // 0x9a
        uint8_t Songs; // 0x9b
        uint8_t UpgradeItems; // 0x9c
        Companion Follower; // 0x9d
        uint8_t BombCount; // 0x9e
        uint8_t ArrowCount; // 0x9f
        uint8_t PowderCount; // 0xa0
        uint8_t EquippedSong; // 0xa1
        uint8_t EquipY; // 0xa2
        uint8_t EquipX; // 0xa3
        char unk3[0x4];
        uint32_t BaseItems; // 0xa8
        uint32_t ExtraItems; // 0xac
        uint64_t unk4; // 0xb0
        char unk5[0xa];
        uint8_t GoldenLeaves; // 0xc2
        char unk6[0x25];
        uint32_t Figurines; // 0xe8
        char unk7[0x34];
        bool HeroMode; // 0x120
        char unk8;
        uint8_t RareFishSign; // 0x122
    };

    extern InventoryItems* Items;

    void SetBaseItem(BaseItemBits item, bool set);
}
