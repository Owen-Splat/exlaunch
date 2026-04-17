#include "patches.hpp"

namespace patch = exl::patch;
namespace inst = exl::armv8::inst;
namespace reg = exl::armv8::reg;

// Credits to theboy181 for the patch
void blurRemoval() {
    patch::CodePatcher p(0x16cbd73);
    p.Write("NoTiltShift");
}

void randoCosmeticPatches() {
    // NPCs hold the proper item model before giving it to the player
    // This is done by changing the itemID of the model to a new Items.gsheet entry with the proper model
    patch::CodePatcher p(0x9fa0f0); // bay-fisherman
    p.WriteInst(inst::Movz(reg::W4, 200));
    p.Seek(0xa40374); // syrup
    p.WriteInst(inst::Movz(reg::W3, 201));
    p.Seek(0xa534a4); // walrus
    p.WriteInst(inst::Movz(reg::W8, 202));

    // Songs, tunics, and capacity upgrades show the correct item model
    // This is done by making them go to the default itemID case
    // Default case means it will use its own npcKey in Items.gsheet
    p.Seek(0xd798c4);
    p.WriteInst(inst::Branch(0xd799f8 - 0xd798c4));
    p.Seek(0xd79814);
    p.WriteInst(inst::Branch(0xd799f8 - 0xd79814));
    p.Seek(0xd79804);
    p.WriteInst(inst::Branch(0xd799f8 - 0xd79804));
}

void randoFixes() {
    // remove checking ShieldGet flag so that the user can immediately pause
    // this is important since Tarin uses the ShieldGet flag as well but we may want custom spawn locations
    patch::CodePatcher p(0xeacf2c);
    p.WriteInst(inst::Movz(reg::W0, 1));

    // free book - read the book of secrets without the magnifying lens
    // not sure if this setting will be kept or baked in as a default feature
    p.Seek(0x7e3004);
    p.WriteInst(inst::Movz(reg::W0, 1));
}

void runCodePatches() {
    EXL_ASSERT(global_config.initialized);
    if (global_config.blur_removal.enabled) {
        blurRemoval();
    }
    if (global_config.randomizer_compatible.enabled) {
        randoCosmeticPatches()
        randoFixes();
    }
    // randoOptional();
    // testPatches();
}
