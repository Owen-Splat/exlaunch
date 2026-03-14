#pragma once
#include <cstdint>

const uintptr_t invAddr = 0xA1C46E0;

struct {
    uintptr_t BaseItems = 0xA8;
    uintptr_t Upgrades = 0x9C;
    uintptr_t PermItems = 0xAC;
    uintptr_t Rupees = 0x80;
    uintptr_t Health = 0x84;
    uintptr_t Seashells = 0x88;
    uintptr_t HeartPieces = 0x90;
    uintptr_t HeartContainers = 0x98;
    uintptr_t TradeItem = 0x9A;
    uintptr_t Companion = 0x9D;
    uintptr_t BombCount = 0x9E;
    uintptr_t ArrowCount = 0x9F;
    uintptr_t PowderCount = 0xA0;
    uintptr_t EquippedSong = 0xA1;
    uintptr_t EquipY = 0xA2;
    uintptr_t EquipX = 0xA3;
    uintptr_t GoldenLeaves = 0xC2;
} invOffsets;

struct {
    uint8_t Alone = 0;
    uint8_t BowWow = 1;
    uint8_t Marin = 2;
    uint8_t Ghost = 3;
    uint8_t Rooster = 4;
} g_Companions;

uint32_t* g_BaseItems = reinterpret_cast<uint32_t*>(invAddr + invOffsets.BaseItems);
uint8_t* g_Upgrades = reinterpret_cast<uint8_t*>(invAddr + invOffsets.Upgrades);
uint32_t* g_PermItems = reinterpret_cast<uint32_t*>(invAddr + invOffsets.PermItems);
int32_t* g_Rupees = reinterpret_cast<int32_t*>(invAddr + invOffsets.Rupees);
int16_t* g_Health = reinterpret_cast<int16_t*>(invAddr + invOffsets.Health);
uint64_t* g_Seashells = reinterpret_cast<uint64_t*>(invAddr + invOffsets.Seashells);
uint8_t* g_Companion = reinterpret_cast<uint8_t*>(invAddr + invOffsets.Companion);
uint8_t* g_BombCount = reinterpret_cast<uint8_t*>(invAddr + invOffsets.BombCount);
