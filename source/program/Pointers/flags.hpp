#pragma once
#include <cstdint>

const uintptr_t flagAddr = 0xA1C5120;
uint64_t* g_EventFlags = reinterpret_cast<uint64_t*>(flagAddr);
