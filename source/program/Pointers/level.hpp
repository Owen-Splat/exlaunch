#pragma once
#include <cstdint>

const uintptr_t lvAddr = 0xA1C4740;

uint64_t* g_Level = reinterpret_cast<uint64_t*>(lvAddr);
