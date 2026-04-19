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
    patch::CodePatcher p(0);

    // Nice Bombs - no limit to how many can be placed at once
    if (global_config.nice_items.bombs) {
        p.Seek(0xd52958);
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

void randoFixes() {
    // remove checking ShieldGet flag so that the user can immediately pause
    // this is important since Tarin uses the ShieldGet flag as well but we may want custom spawn locations
    patch::CodePatcher p(0xeacf2c);
    p.WriteInst(inst::Movz(reg::W0, 1));

    // ignore companions when opening color dungeon
    p.Seek(0xc868d4);
    p.WriteInst(inst::Nop());
    p.Seek(0xc868d8);
    p.WriteInst(inst::CmpImmediate(reg::W8, 0));

    // Make EnemySoldierIronBall ignore GoldenLeaf[4]
    p.Seek(0x6a62f8);
    p.WriteInst(inst::Nop());

    // Rewrite FlowControl::CompareInt event to check if the values are equal
    // To match FlowControl::CompareString, it returns 0 if they are equal, 1 if not
    // This allows us to check the index of items through the EventFlow system
    // The main purpose of this will be for Keysanity to know which dungeon text to display
    p.Seek(0x8049d8);
    p.WriteInst(inst::Movz(reg::W8, 1));

    // Make bombs, arrows, and power give 3 for a single drop
    p.Seek(0x88f674);
    p.WriteInst(inst::Movz(reg::W4, 3));
    p.Seek(0x895674);
    p.WriteInst(inst::Movz(reg::W4, 3));
    p.Seek(0x894740);
    p.WriteInst(inst::Movz(reg::X7, 3));

    // NPCs hold the proper item model before giving it to the player
    // This is done by changing the itemID of the model to a new Items.gsheet entry with the proper model
    p.Seek(0x9fa0f0); // bay-fisherman
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

void randoOptional() {
    patch::CodePatcher p(0);

    // free book - read the book of secrets without the magnifying lens
    if (global_config.randomizer.free_book) {
        p.Seek(0x7e3004);
        p.WriteInst(inst::Movz(reg::W0, 1));
    }

    // stealing
    p.Seek(0xa4a8f0);
    switch (global_config.randomizer.stealing) {
        case StealingMode::Always: // sword is not required to be able to steal
            p.WriteInst(inst::Branch(0xa4a910 - 0xa4a8f0));
            break;
        case StealingMode::Never: // player cannot steal no matter what
            p.WriteInst(inst::Nop());
            break;
        case StealingMode::Standard:
            break;
    }
}

void runCodePatches() {
    EXL_ASSERT(global_config.initialized);
    if (global_config.blur_removal.enabled) {
        blurRemoval();
    }
    if (global_config.nice_items.enabled) {
        niceItems();
    }
    if (global_config.ohko.enabled) {
        oneHitKO();
    }
    if (global_config.randomizer.enabled) {
        randoFixes();
        randoOptional();
    }
}
