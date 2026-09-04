#include "VersionFinder.h"
#include "PorterPlugin.h"
#include <iostream>

#define nullptr_value 0

Cleo5Moonloader::Version Cleo5Moonloader::FindMoonloaderVersion()
{
    HMODULE moonloaderModule = GetModuleHandleA(MoonLoaderCleo5Instance::MoonloaderLibName.data());

    uint8_t* checkingAddress = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(moonloaderModule) + 0x5000 + 1);

    switch (*checkingAddress)
    {
    case 0xC8:
    case 0xEE:
        return Version::Version_026_5; 
        break;
    case 0xEC:
        return Version::Version_027_pr3;
    default:
        return Version::None;
        break;
    }
}