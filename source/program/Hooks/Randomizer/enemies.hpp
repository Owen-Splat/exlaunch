#pragma once
#include "../lib.hpp"
#include "Config/config.hpp"
#include "Game/Data/inventory.hpp"
#include "Hooks/Common/level.hpp"
#include <string>
#include <iterator>
#include <vector>
#include <deque>
#include <algorithm>
#include <random>

// we want to group actors by types
// actors not included are too problematic, whether they just block paths or straight up crash the game without additonal work
u16 land_ids[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x8, 0x9, 0xa, 0x12, 0x13, 0x14, 0x15, 0x16,
                    0x17, 0x1b, 0x1c, 0x1d, 0x22, 0x24, 0x25, 0x27, 0x28, 0x29, 0x2e, 0x2f,
                    0x30, 0x31, 0x32, 0x33, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c,
                    0x3f, 0x40, 0x41, 0x42, 0x44, 0x4a, 0x4d, 0xb7};
u16 air_ids[] = {0x7, 0x10, 0x26, 0x34, 0x3e, 0x48};
u16 tree_ids[] = {0xc, 0x23};
u16 water_shallow_ids[] = {0xd, 0x43};
u16 water_ids[] = {0x1a};
u16 water_2d_ids[] = {0x45, 0x47};

// in the future we should probably put these into a file to read the same time as the config.ini file
// we could also simply just ignore randomizing enemies entirely inside dungeons
u64 actor_exclusions[] = {
    7638153348067089846, // kanalet crow
    7638164343183367606, // kanalet mad bomber
    6990237534099981064, // stairs under armosdark leading to d6
    6990428849123214091, // stairs under armoslight inside armos maze
    // // the rest are kill room enemies
    // 11602973586154444641, 11601313323595920104, 11601306726526153448, 11601308925549409000,
    // 2234171345317354235, 2234164748247587579, 2234163648735959803, 2234177942387120891,
    // 2234652931410320809, 2234654030921948585, 2234539681712659881, 2234540781224287657,
    // 2234541880735915433, 2234542980247543209, 2234602353875443113, 2234603453387070889,
    // 2234604552898698665, 2234605652410326441, 8502845576057960233, 2234482507108015529,
    // 2234262604782460328, 2234263704294088104, 2234397844712676777, 2234398944224304553,
    // 2234400043735932329, 2234298888666181890, 2234299988177809666, 2234243913084793086,
    // 2234314281828970756, 2234315381340598532, 2234353864247570698, 2234340670108037383,
    // 2234331874015015175, 2233803008922054894, 2233795312340660462, 2234076787317371124,
    // 2234077886828998900, 2234078986340626676, 2234046000991793394, 2234054797084815602,
    // 2234038304410398962, 2234357162782096126, 2234364859363490558, 2234363759851862782,
    // 2234368157898373886, 2234323077921635067, 2234330774503029499, 2234317580363496187,
    // 2234324177433262843, 2234256007712340729, 2234258206735596281, 2234257107223968505,
    // 2234133961921657590, 2234141658503052022, 2234124066317007606, 2234132862410029814,
    // 2234122966805379830, 2235320334968578115, 2235321434480205891, 2235322533991833667,
    // 2235323633503461443, 2235324733015089219, 2235297245224394819, 2235298344736022595,
    // 2235299444247650371, 2235300543759278147, 2235192791619756099, 2235193891131383875,
    // 2235194990643011651, 2235135617015111747, 2235136716526739523, 2235137816038367299,
    // 2235171900898828355, 2235173000410456131, 2235174099922083907, 2235175199433711683,
    // 2235176298945339459, 2234088881944918772, 2234097678037940980, 2234181240921651959,
    // 2234162549223979767, 2234151554107702007, 2234157051665840887, 2234288993061174010,
    // 2234293391107685114, 2234270301363501818, 2234301087689079546, 2234292291596057338,
    // 2234540781223934722, 2234545179270445826, 2234552875851840258, 2234497900270451457,
    // 2234477009549523713, 2234482507107662593, 2234486905154173697, 2234500099293707009 // up to d3 done
};

bool canActorRandomize(u64 hash) {
    if (*Game::Data::Inventory::Level == 0xff) {
        return false;
    }
    if (std::find(std::begin(actor_exclusions), std::end(actor_exclusions), hash) != std::end(actor_exclusions)) {
        return false;
    }
    return true;
}

std::vector<u16> getValidEnemies(u16 id) {
    std::vector<u16> vec = {};

    if (std::find(std::begin(land_ids), std::end(land_ids), id) != std::end(land_ids)) {
        vec.insert(vec.end(), std::begin(land_ids), std::end(land_ids));
        // vec.insert(vec.end(), std::begin(air_ids), std::end(air_ids));
    }
    else if (id == 0x4b || id == 0x4c || id == 0x4e || id == 0x4f) { // EnemyStretchyGhost and EnemyKarakoro color variants
        vec.insert(vec.end(), std::begin(land_ids), std::end(land_ids));
        // vec.insert(vec.end(), std::begin(air_ids), std::end(air_ids));
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
        // vec.insert(vec.end(), std::begin(water_shallow_ids), std::end(water_shallow_ids));
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

// although pretty much anything can work, these are ones that look the best
// we can easily add more later if we want
uint16_t chest_ids[] = {0xc, 0x10, 0x12, 0x19, 0x1b, 0x1c, 0x1d, 0x25, 0x34, 0x3c};

HOOK_DEFINE_INLINE(ObjTreasureBox__PopEnemy) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->W[9] = chest_ids[exl::util::GetRandomU64() % 10];
    }
};

namespace EnemyRandomizer {
    void RandomizeEnemy(u64 actorDataOffset) {
        u16* actorID = reinterpret_cast<u16*>(actorDataOffset + 0xc);
        std::vector<u16> vec = getValidEnemies(*actorID);
        if (vec.size() > 1) {
            u64* hash = reinterpret_cast<u64*>(actorDataOffset);
            if (!isRequiredKill(*hash) && !canActorRandomize(*hash)) {
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
                f32* rotY = reinterpret_cast<f32*>(actorDataOffset + 0x24);
                *rotY = 0.0f;
                rotY = nullptr;
            }
            if (global_config.randomizer.enemy_sizes) {
                Vector3* scale = reinterpret_cast<Vector3*>(actorDataOffset + 0x2c);
                float scale_factor = exl::util::GetRandomF32(0.5f, 1.5f);
                scale->x = scale_factor;
                scale->y = scale_factor;
                scale->z = scale_factor;
                scale = nullptr;
            }
            hash = nullptr;
        }
        actorID = nullptr;
    }

    void InstallHooks() {
        EXL_ASSERT(global_config.initialized);
        if (global_config.randomizer.enemies) {
            ObjTreasureBox__PopEnemy::InstallAtOffset(0xca92c4);
        }
    }
}