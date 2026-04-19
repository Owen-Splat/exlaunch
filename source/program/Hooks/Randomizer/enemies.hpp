#pragma once
#include "../lib.hpp"
#include <Config/config.hpp>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>

// we want to group actors by types
// we handle mixing types through code (flying enemies can be put on the ground for example)
// normally we also have rules for enemies, like max 1 of an enemy per room, or a "blocking" enemy cant be put in the way
// since the player can just reload the room for new enemies... I dont think we need to care about those rules
// actors that needs params to not crash - 0x1e EnemyHidy - incorrect param means disguise model cannot load
// TODO - remove blocking enemies (armos, urchins) and filter enemies that are too similar (shield/spear enemies)
uint16_t land_ids[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x8, 0x9, 0xa, 0x12, 0x13, 0x14, 0x15, 0x16,
                    0x17, 0x1b, 0x1c, 0x1d, 0x22, 0x24, 0x25, 0x27, 0x28, 0x29, 0x2e, 0x2f,
                    0x30, 0x31, 0x32, 0x33, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c,
                    0x3f, 0x40, 0x41, 0x42, 0x44, 0x4a, 0x4d, 0xb7};
uint16_t air_ids[] = {0x7, 0x10, 0x26, 0x34, 0x3e, 0x48};
uint16_t tree_ids[] = {0xc, 0x23};
uint16_t water_shallow_ids[] = {0xd, 0x43};
uint16_t water_ids[] = {0x1a};
uint16_t water_2d_ids[] = {0x45, 0x47};

std::vector<uint16_t> getValidEnemies(uint16_t id) {
    std::vector<uint16_t> vec = {};

    if (std::find(std::begin(land_ids), std::end(land_ids), id) != std::end(land_ids)) {
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
    }

    else if (std::find(std::begin(water_2d_ids), std::end(water_2d_ids), id) != std::end(water_2d_ids)) {
        vec.insert(vec.end(), std::begin(water_2d_ids), std::end(water_2d_ids));
    }

    return vec;
}

HOOK_DEFINE_INLINE(CreateActorObject) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.randomizer.enemies) {
            std::vector<uint16_t> vec = getValidEnemies(ctx->W[8]);
            if (vec.size() > 1) {
                ctx->W[8] = vec[exl::util::GetRandomU64() % vec.size()];
            }
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
        CreateActorObject::InstallAtOffset(0x8e1858);
        ObjTreasureBox__PopEnemy::InstallAtOffset(0xca92c4);
    }
}