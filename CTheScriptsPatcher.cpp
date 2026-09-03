#include <stdint.h>
#include "VersionFinder.h"
#include "PorterPlugin.h"
#include "remote_memory.hpp"

namespace Cleo5Moonloader
{
    constexpr uintptr_t Original_CTheScripts__ScriptSpace_Address = 0xA49960;
    constexpr uint32_t OriginalLimit_ScriptSpace = 200000;

    namespace Data
    {
        constexpr SignaturePlace Signatures_026_5[]
        {
            {1, "A1 ? ? ? ? 0F B7 4C 24"},
            {1, "A1 ? ? ? ? C3 ? ? ? ? ? ? ? ? ? ? E9"},
            {1, "A1 ? ? ? ? 8B 4C 24 ? ? ? ? C3"},
        };

        constexpr SignaturePlace Signatures_027_pr3[]
        {
            {1, "A1 ? ? ? ? 0F B7 4C 24"},
            {1, "A1 ? ? ? ? C3 ? ? ? ? ? ? ? ? ? ? E9"},
            {1, "A1 ? ? ? ? 8B 4C 24 ? ? ? ? C3"}
        };
        
        constexpr SignaturePlace DetectPlayerPedSignature_026_5 = {1, "A1 ? ? ? ? ? ? 74 ? ? ? 83 EC"};
        constexpr SignaturePlace DetectPlayerPedSignature_027_pr3 = {1, "A1 ? ? ? ? ? ? 74 ? ? ? 8D 4E"};

        constexpr auto Moonloader_026_detect_playerped_offset = 12;
    };
    
    bool CheckOriginalAddress()
    {
        uintptr_t* chechingPlace = reinterpret_cast<uintptr_t*>(0x44CA42 + 2);
        if(*chechingPlace == Original_CTheScripts__ScriptSpace_Address)
            return true;
        
            return false;
    }

    uintptr_t GetNewAddressData()
    {
        if(CheckOriginalAddress())
        {
            return Original_CTheScripts__ScriptSpace_Address;
        }

        return *reinterpret_cast<uintptr_t*>(0x44CA42 + 2);
    }

    void PatchMoonloaderMemory()
    {
        if(!CheckOriginalAddress())
        {
            bool IsPatchedLuaFunctions = false;
            bool IsPatchedDetectPActor = false;
            HMODULE moonloaderModule = GetModuleHandleA(MoonLoaderCleo5Instance::MoonloaderLibName.data());
            auto result = Cleo5Moonloader::FindMoonloaderVersion();

            switch (result)
            {
                case Version::Version_026_5:
                {
                    // patching LUA functions
                    for(const auto& object : Data::Signatures_026_5)
                    {
                        uintptr_t address = reinterpret_cast<uintptr_t>(SSMP::PatternScan(moonloaderModule, object.signature.data()));
                        if(address != 0)
                        {
                            address += object.offset;
                            MoonLoaderCleo5Instance::ProcessMemory.write(address, GetNewAddressData());
                        }
                    }
                    uintptr_t detectplace = reinterpret_cast<uintptr_t>(SSMP::PatternScan(moonloaderModule, Data::DetectPlayerPedSignature_026_5.signature.data()));
                    if(detectplace != 0)
                    {
                        detectplace += Data::DetectPlayerPedSignature_026_5.offset;
                        uint32_t detectAddress = GetNewAddressData() + Data::Moonloader_026_detect_playerped_offset;
                        MoonLoaderCleo5Instance::ProcessMemory.write<uintptr_t>(detectplace, detectAddress);
                    }
                    break;
                }
                case Version::Version_027_pr1:
                {
                    break;
                }
                case Version::Version_027_pr3:
                {
                    // disabling moonloader function CTheScripts__InitScriptSpace
                    uintptr_t disabling_address = reinterpret_cast<uintptr_t>(SSMP::PatternScan(moonloaderModule, "83 EC ? B9 ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? A3 ? ? ? ? C3 ? ? ? ? 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 ? ? ? ? ? ? ? ? ? 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 ? ? ? ? ? ? ? ? ? 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 ? ? ? ? ? ? ? ? ? 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 ? ? ? ? ? ? ? ? ? 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 ? ? ? ? ? ? ? ? ? 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 ? ? ? ? ? ? ? ? ? 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 ? ? ? ? ? ? ? ? ? 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 ? ? ? ? ? ? ? ? ? 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 ? ? ? ? ? ? ? ? ? 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 ? ? ? ? ? ? ? ? ? 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 ? ? ? ? ? ? ? ? ? 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 ? ? ? ? ? ? ? ? ? 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 ? ? ? ? ? ? ? ? ? 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 83 C4"));
                    if(disabling_address != 0)
                    {
                        MoonLoaderCleo5Instance::ProcessMemory.write<uint8_t>(disabling_address, 0xC3);
                    }

                    for(const auto& object : Data::Signatures_027_pr3)
                    {
                        uintptr_t address = reinterpret_cast<uintptr_t>(SSMP::PatternScan(moonloaderModule, object.signature.data()));
                        if(address != 0)
                        {
                            address += object.offset;
                            MoonLoaderCleo5Instance::ProcessMemory.write(address, GetNewAddressData());
                        }
                    }

                    uintptr_t detectplace = reinterpret_cast<uintptr_t>(SSMP::PatternScan(moonloaderModule, Data::DetectPlayerPedSignature_027_pr3.signature.data()));
                    if(detectplace != 0)
                    {
                        detectplace += Data::DetectPlayerPedSignature_027_pr3.offset;
                        uint32_t detectAddress = GetNewAddressData() + Data::Moonloader_026_detect_playerped_offset;
                        MoonLoaderCleo5Instance::ProcessMemory.write<uintptr_t>(detectplace, detectAddress);
                    }
                }
            }
        }
    }
}