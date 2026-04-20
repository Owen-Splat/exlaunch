#pragma once
#include "../lib.hpp"
#include "Hooks/Common/level.hpp"
#include <Config/config.hpp>
#include <string>
#include <iterator>
#include <vector>
#include <deque>
#include <algorithm>
#include <random>

// we want to group actors by types
// we handle mixing types through code (flying enemies can be put on the ground for example)
// actors that needs params to not crash - 0x1e EnemyHidy - incorrect param means disguise model cannot load
u16 land_ids[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x8, 0x9, 0xa, 0x12, 0x13, 0x14, 0x15, 0x16,
                    0x17, 0x1b, 0x1c, 0x1d, 0x22, 0x24, 0x25, 0x27, 0x28, 0x29, 0x2e, 0x2f,
                    0x30, 0x31, 0x32, 0x33, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c,
                    0x3f, 0x40, 0x41, 0x42, 0x44, 0x4a, 0x4d, 0xb7};
u16 air_ids[] = {0x7, 0x10, 0x26, 0x34, 0x3e, 0x48};
u16 tree_ids[] = {0xc, 0x23};
u16 water_shallow_ids[] = {0xd, 0x43};
u16 water_ids[] = {0x1a};
u16 water_2d_ids[] = {0x45, 0x47};

std::vector<u16> getValidEnemies(u16 id) {
    std::vector<u16> vec = {};

    if (std::find(std::begin(land_ids), std::end(land_ids), id) != std::end(land_ids)) {
        vec.insert(vec.end(), std::begin(land_ids), std::end(land_ids));
        vec.insert(vec.end(), std::begin(air_ids), std::end(air_ids));
    }
    else if (id == 0x4b || id == 0x4c || id == 0x4e || id == 0x4f) { // EnemyStretchyGhost and EnemyKarakoro color variants
        vec.insert(vec.end(), std::begin(land_ids), std::end(land_ids));
        vec.insert(vec.end(), std::begin(air_ids), std::end(air_ids));
    }
    else if (std::find(std::begin(air_ids), std::end(air_ids), id) != std::end(air_ids)) {
        vec.insert(vec.end(), std::begin(air_ids), std::end(air_ids));
    }

    else if (std::find(std::begin(tree_ids), std::end(tree_ids), id) != std::end(tree_ids)) {
        vec.insert(vec.end(), std::begin(tree_ids), std::end(tree_ids));
    }

    else if (std::find(std::begin(water_shallow_ids), std::end(water_shallow_ids), id) != std::end(water_shallow_ids)) {
        vec.insert(vec.end(), std::begin(water_shallow_ids), std::end(water_shallow_ids));
    }

    else if (std::find(std::begin(water_ids), std::end(water_ids), id) != std::end(water_ids)) {
        vec.insert(vec.end(), std::begin(water_ids), std::end(water_ids));
        vec.insert(vec.end(), std::begin(water_shallow_ids), std::end(water_shallow_ids));
    }

    else if (std::find(std::begin(water_2d_ids), std::end(water_2d_ids), id) != std::end(water_2d_ids)) {
        vec.insert(vec.end(), std::begin(water_2d_ids), std::end(water_2d_ids));
    }

    return vec;
}

u16 blocking_ids[] = {0x3, 0x15, 0x16, 0x30, 0x41};
u16 annoying_ids[] = {0x26, 0x3e, 0x48, 0x8, 0x9, 0x13, 0x14, 0x2e, 0x2f, 0x4d};
std::deque<int> lastTen;

bool isEnemyValid(u16 vanilla_id, u16 new_id) {
    // if the enemy is vanilla then it is obviously valid :P
    if (new_id == vanilla_id) {
        return true;
    }

    // specific case where we want to block vires from spawning in the overworld
    if (new_id == 0x26 && currentLevel == "Field") {
        return false;
    }

    // make sure a non-blocking enemy is not changed into a blocking enemy
    if (std::find(std::begin(blocking_ids), std::end(blocking_ids), new_id) != std::end(blocking_ids)) {
        return false;
    }

    // prevent a powerful or annoying enemies from spawning too frequently
    if (std::find(std::begin(annoying_ids), std::end(annoying_ids), new_id) != std::end(annoying_ids)) {
        if (std::find(lastTen.begin(), lastTen.end(), (int)new_id) != lastTen.end()) {
            return false;
        }
    }

    return true;
}

// The variants are just different colors but the same enemy effectively
// We only include the first variant in the enemy pool so that they arent prominent
u16 randomizeEnemyVariants(u16 id) {
    if (id == 0x4a || id == 0x4d) {
        id += exl::util::GetRandomU64() % 3;
    }
    return id;
}

bool isRequiredKill(u64 hash) {
    return false; // return false for now, need to compile a list of hashes
}

struct Vector3 {
    float x;
    float y;
    float z;
};

HOOK_DEFINE_INLINE(InterceptActorLoad) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.randomizer.enemies) {
            u16* actorID = reinterpret_cast<u16*>(ctx->X[26] + 0xc);
            std::vector<u16> vec = getValidEnemies(*actorID);
            if (vec.size() > 1) {
                u64* hash = reinterpret_cast<u64*>(ctx->X[26]);
                if (!isRequiredKill(*hash)) {
                    u16 new_enemy;
                    do {
                        new_enemy = vec[exl::util::GetRandomU64() % vec.size()];
                    }
                    while (!isEnemyValid(*actorID, new_enemy));
                    if (lastTen.size() >= 10) {
                        lastTen.pop_front();
                    }
                    lastTen.push_back((int)new_enemy);
                    new_enemy = randomizeEnemyVariants(new_enemy);
                    *actorID = new_enemy;
                    // some enemies are rotated, we do not want them to randomize into an enemy that is rotation locked
                    // while they still work, I just dont like how it looks
                    f32* rotY = reinterpret_cast<f32*>(ctx->X[26] + 0x24);
                    *rotY = 0.0f;
                    rotY = nullptr;
                }
                hash = nullptr;
                if (global_config.randomizer.enemy_sizes) {
                    Vector3* scale = reinterpret_cast<Vector3*>(ctx->X[26] + 0x2c);
                    float scale_factor = exl::util::GetRandomF32(0.5f, 1.5f);
                    scale->x = scale_factor;
                    scale->y = scale_factor;
                    scale->z = scale_factor;
                    scale = nullptr;
                }
            }
            actorID = nullptr;
        }
    }
};

// although pretty much anything can work, these are ones that look the best
// we can easily add more later if we want
uint16_t chest_ids[] = {0xc, 0x10, 0x12, 0x19, 0x1b, 0x1c, 0x1d, 0x25, 0x34, 0x3c};

HOOK_DEFINE_INLINE(ObjTreasureBox__PopEnemy) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.randomizer.enemies) {
            ctx->W[9] = chest_ids[exl::util::GetRandomU64() % 10];
        }
    }
};

namespace EnemyRandomizer {
    void installHooks() {
        InterceptActorLoad::InstallAtOffset(0x8e177c);
        ObjTreasureBox__PopEnemy::InstallAtOffset(0xca92c4);
    }
}