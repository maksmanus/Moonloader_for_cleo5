#include "VersionFinder.h"
#include "PorterPlugin.h"

#define nullptr_value 0

Cleo5Moonloader::Version Cleo5Moonloader::FindMoonloaderVersion()
{
    HMODULE moonloaderModule = GetModuleHandleA(MoonLoaderCleo5Instance::MoonloaderLibName.data());

    uint8_t* checkingAddress = reinterpret_cast<uint8_t*>(moonloaderModule + 0x1000);

    switch (*checkingAddress)
    {
    case 0x83:
        return Version::Version_027_pr3; 
        break;
    case 0x68:
        return Version::Version_026_5;
        break;        
    default:
        return Version::None;
        break;
    }
}