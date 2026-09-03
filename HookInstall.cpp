#include "HookInstall.h"

namespace HookInstall
{
    void InstallCallHook(uintptr_t AddressInstruction, uintptr_t FunctionAddress)
    {
        if(reinterpret_cast<void*>(AddressInstruction))
        {
            DWORD oldprotect;
            uint32_t RelativeAddress = FunctionAddress - AddressInstruction - 5;
            VirtualProtect(reinterpret_cast<LPVOID>(AddressInstruction), 5, PAGE_READWRITE, &oldprotect);

            *reinterpret_cast<BYTE*>(AddressInstruction) = 0xE8;
            *reinterpret_cast<DWORD*>(AddressInstruction + 1) = RelativeAddress;
            VirtualProtect(reinterpret_cast<LPVOID>(AddressInstruction), 5, oldprotect, &oldprotect);
        }
    }

    void InstallJumpHook(uintptr_t AddressInstruction, uintptr_t FunctionAddress)
    {
        if(reinterpret_cast<void*>(AddressInstruction))
        {
            DWORD oldprotect;
            uint32_t RelativeAddress = FunctionAddress - AddressInstruction - 5;
            VirtualProtect(reinterpret_cast<LPVOID>(AddressInstruction), 5, PAGE_READWRITE, &oldprotect);

            *reinterpret_cast<BYTE*>(AddressInstruction) = 0xE9;
            *reinterpret_cast<DWORD*>(AddressInstruction + 1) = RelativeAddress;
            VirtualProtect(reinterpret_cast<LPVOID>(AddressInstruction), 5, oldprotect, &oldprotect);
        }
    }

    void SetMemory(uintptr_t address, int value, uint32_t size)
    {
        DWORD old;
        VirtualProtect(reinterpret_cast<LPVOID>(address), size, PAGE_READWRITE, &old);
        memset(reinterpret_cast<void*>(address), value, size);
        VirtualProtect(reinterpret_cast<LPVOID>(address), size, old, &old);
    }

    void SetMemory(uintptr_t address, uint32_t value, uint32_t size)
    {
        DWORD old;
        VirtualProtect(reinterpret_cast<LPVOID>(address), size, PAGE_READWRITE, &old);
        byte* bytes = reinterpret_cast<byte*>(&value);
        memcpy(reinterpret_cast<void*>(address), bytes, size);
        VirtualProtect(reinterpret_cast<LPVOID>(address), size, old, &old);
    }

    void SetMemory(uintptr_t address, DWORD value, uint32_t size)
    {
        DWORD old;
        VirtualProtect(reinterpret_cast<LPVOID>(address), size, PAGE_READWRITE, &old);
        byte* bytes = reinterpret_cast<byte*>(&value);
        memcpy(reinterpret_cast<void*>(address), bytes, size);
        VirtualProtect(reinterpret_cast<LPVOID>(address), size, old, &old);
    }
}
