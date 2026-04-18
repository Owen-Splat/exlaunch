#include "patches.hpp"

namespace patch = exl::patch;
namespace inst = exl::armv8::inst;
namespace reg = exl::armv8::reg;

void blurRemoval() {
    // Removes the blur around the edge of the screen
    // Credits to theboy181 for the patch
    patch::CodePatcher p(0x16cbd73);
    p.Write("NoTiltShift");
}

void niceItems() {
    // Nice Bombs - no limit to how many can be placed at once
    if (global_config.nice_items.bombs) {
        patch::CodePatcher p(0xd52958);
        p.WriteInst(inst::Movz(reg::W8, 1));
    }

    // Nice Hookshot - hookshot any surface
    if (global_config.nice_items.hookshot) {
        p.Seek(0xd7f188);
        p.WriteInst(inst::Movz(reg::W20, 3));
    }

    // Nice Magic Rod - no projectile count limit
    if (global_config.nice_items.rod) {
        p.Seek(0xd51698);
        p.WriteInst(inst::CmpImmediate(reg::X19, 0x10));
    }

    // Lv1 Sword Beams - not sure if we want to keep
    if (global_config.nice_items.sword) {
        p.Seek(0xde1ba8);
        p.Write(0x3942A109); // ldrb w9, [x8, #0xa8]
    }
}

void oneHitKO() {
    patch::CodePatcher p(0xd4c754); // normal damage
    p.WriteInst(inst::SubImmediate(reg::W22, reg::W8, 80));

    p.Seek(0xdb1f74); // fall/drown damage
    p.WriteInst(inst::SubImmediate(reg::W8, reg::W21, 80));

    p.Seek(0xd7c8c8); // trap damage
    p.WriteInst(inst::SubImmediate(reg::W20, reg::W8, 80));

    p.Seek(0xd96950); // blaino damage
    p.WriteInst(inst::SubImmediate(reg::W8, reg::W23, 80));
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
    if (global_config.nice_items.enabled) {
        niceItems();
    }
    if (global_config.randomizer_compatible.enabled) {
        randoCosmeticPatches();
        randoFixes();
    }
    // randoOptional();
    // testPatches();
}
