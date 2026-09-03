#pragma once

#include <Windows.h>
#include <stdint.h>

namespace HookInstall
{
    void InstallCallHook(uintptr_t AddressInstruction, uintptr_t FunctionAddress);
    void InstallJumpHook(uintptr_t AddressInstruction, uintptr_t FunctionAddress);
    void SetMemory(uintptr_t address, int value, uint32_t size);
    void SetMemory(uintptr_t address, uint32_t value, uint32_t size);
    void SetMemory(uintptr_t address, DWORD value, uint32_t size);
};
