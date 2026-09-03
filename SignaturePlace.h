#pragma once
#include <stdint.h>
#include <string_view>
#include "MoonloaderVersion.h"

namespace Cleo5Moonloader
{
    class SignaturePlace
    {
    public:
        int32_t offset;
        std::string_view signature;

        constexpr SignaturePlace(int32_t _offset, std::string_view _signature) 
            : offset(_offset), signature(_signature) {}
    };

    class SignatureVersionPlace : public SignaturePlace
    {
    public:
        Version moonloaderVersion;

        constexpr SignatureVersionPlace(int32_t _offset, std::string_view _signature, Version _version) : SignaturePlace(_offset, _signature), moonloaderVersion(_version) {}
    };
};