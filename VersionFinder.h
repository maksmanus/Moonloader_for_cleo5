#pragma once

#include <string_view>
#include <cstdint>
#include <string>
#include <Windows.h>

#include "PatterinFInder.h"
#include "MoonloaderVersion.h"

namespace Cleo5Moonloader
{
    class VersionDecription
    {
    public:
        std::string_view versionString;
        std::string_view findingSignature;
        int32_t signatureOffset;
        Version enumDescription;

        constexpr VersionDecription(std::string_view _versionString, std::string_view _signature, int32_t _signatureOffset, Version _enumDescription) : versionString(_versionString), findingSignature(_signature), signatureOffset(_signatureOffset), enumDescription(_enumDescription){} 
    };

    Version FindMoonloaderVersion();
}