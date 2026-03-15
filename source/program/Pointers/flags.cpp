#include "Pointers/flags.hpp"
#include <cstdint>
#include <string>

const uintptr_t flagAddr = 0xA1C5120;
uint64_t* g_EventFlags = reinterpret_cast<uint64_t*>(flagAddr);

// s_Save248* g_Save248 = reinterpret_cast<s_Save248*>(flagAddr + 0x248);
