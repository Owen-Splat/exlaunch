#include "patches.hpp"

namespace patch = exl::patch;
namespace inst = exl::armv8::inst;
namespace reg = exl::armv8::reg;

void blurRemoval() {

}

void freeBook() {
    patch::CodePatcher p(0x7e3004);
    p.WriteInst(inst::Movz(reg::W0, 1));
}

void niceBombs() {
    patch::CodePatcher p(0xd52958);
    p.WriteInst(inst::Movz(reg::W8, 1));
}

void extraBombDrops() {
    patch::CodePatcher p (0x88f674);
    p.WriteInst(inst::Movz(reg::W4, 5));
}

void runCodePatches() {
    extraBombDrops();
}
