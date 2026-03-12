#pragma once

#include "../lib.hpp"
#include "../lib/armv8/register.hpp"
#include "../lib/util/random.hpp"

const exl::util::ModuleInfo g_MainInfo = exl::util::GetMainModuleInfo();
const uintptr_t g_BaseAddress = g_MainInfo.m_Total.m_Start;
const uintptr_t g_MainEnd = g_BaseAddress + g_MainInfo.m_Total.m_Size;

// TODO: get offsets and pointers to inventory and event flags
// eventflags - 0xA1C5120?
const uintptr_t g_inventory = 0xa1c46e0;
// TODO: make a struct of inventory offsets for easy access
