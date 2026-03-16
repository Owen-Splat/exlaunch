#include "../lib.hpp"
#include <string>

HOOK_DEFINE_TRAMPOLINE(AudioSystem__PlaySoundEffect) {
    static long Callback(float arg1, float arg2, long strAddr, long arg4) {
        return Orig(arg1, arg2, strAddr, arg4);
    }
};

namespace AudioSystem {
    void PlaySoundEffect(std::string soundEffect) {
        AudioSystem__PlaySoundEffect::Callback(1.0f, 1.0f, (long)&soundEffect, 0);
    }

    void InstallHooks() {
        AudioSystem__PlaySoundEffect::InstallAtOffset(0xa57e0);
    }
}
