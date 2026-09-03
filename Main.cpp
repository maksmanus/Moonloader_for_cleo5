#include "PorterPlugin.h"
#include "PatterinFInder.h"
#include "HookInstall.h"
#include "remote_memory.hpp"
#include "CTheScriptPatcher.h"
#include "VersionFinder.h"
#include <Windows.h>
#include <thread>
#include <mutex>

std::mutex PluginMutex;
remote::memory MoonLoaderCleo5Instance::ProcessMemory;

void Cleo5Moonloader::Hooks::Moonloader_RegisterFunctionsHook(void* pointer)
{
    HMODULE moonloaderModule = GetModuleHandleA(MoonLoaderCleo5Instance::MoonloaderLibName.data());
    auto FoundVersion = FindMoonloaderVersion();
    std::string forkedSignature = Cleo5Moonloader::Data::FindSignatureForVersion(Cleo5Moonloader::Data::RegisterAllMoonloaderFunctionsSignature, FoundVersion);

    if(forkedSignature.empty())
    {
        return;
    }

    char (*original)(void*) = reinterpret_cast<decltype(original)>(SSMP::PatternScan(moonloaderModule, forkedSignature.c_str()));
    if(original)
    {
        original(pointer);
        std::thread localthread = std::thread([pointer, moonloaderModule, FoundVersion]()
        {
            std::string forkedsignature_MoonLoader__Registering__RegisterCleoFunctions = Cleo5Moonloader::Data::FindSignatureForVersion(Cleo5Moonloader::Data::MoonLoaderRegisteringRegisterCleoFunctionsSignature, FoundVersion);
            std::string forkedsignature_MoonLoader__IsSampfuncLoaded = Cleo5Moonloader::Data::FindSignatureForVersion(Cleo5Moonloader::Data::MoonLoaderIsSampfuncLoadedSignature, FoundVersion);
            std::string forkedsignature_MoonLoader__Registering__RegisterSampFunctions = Cleo5Moonloader::Data::FindSignatureForVersion(Cleo5Moonloader::Data::MoonLoaderRegisteringRegisterSampFunctionsSignature, FoundVersion);
            std::string forkedsignature_MoonLoader__Registering__RegisterSampFunctions_Raknet = Cleo5Moonloader::Data::FindSignatureForVersion(Cleo5Moonloader::Data::MoonLoaderRegisteringRegisterSampFunctionsRaknetSignature, FoundVersion); 

            int (WINAPI* CleoGetVersion)();
            if(!GetModuleHandleA(MoonLoaderCleo5Instance::CleoLibName.data()))
            {
                return;
            }
            else
            {
                CleoGetVersion = GetProcAddress(GetModuleHandleA(MoonLoaderCleo5Instance::CleoLibName.data()), "_CLEO_GetVersion@0");
            }
            while(!IsCleoLoaded_forCleo5())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            if(forkedsignature_MoonLoader__IsSampfuncLoaded.empty() || forkedsignature_MoonLoader__Registering__RegisterCleoFunctions.empty() || forkedsignature_MoonLoader__Registering__RegisterSampFunctions.empty() || forkedsignature_MoonLoader__Registering__RegisterSampFunctions_Raknet.empty())
            {
                return;
            }

            void (* MoonLoader__Registering__RegisterCleoFunctions)(void*) = reinterpret_cast<decltype(MoonLoader__Registering__RegisterCleoFunctions)>(SSMP::PatternScan(moonloaderModule, forkedsignature_MoonLoader__Registering__RegisterCleoFunctions.c_str()));
            bool (*MoonLoader__IsSampfuncLoaded)() = reinterpret_cast<decltype(MoonLoader__IsSampfuncLoaded)>(SSMP::PatternScan(moonloaderModule, forkedsignature_MoonLoader__IsSampfuncLoaded.c_str()));
            void (*MoonLoader__Registering__RegisterSampFunctions)(void*) = reinterpret_cast<decltype(MoonLoader__Registering__RegisterSampFunctions)>(SSMP::PatternScan(moonloaderModule, forkedsignature_MoonLoader__Registering__RegisterSampFunctions.c_str()));
            void (*MoonLoader__Registering__RegisterSampFunctions_Raknet)(void*) = reinterpret_cast<decltype(MoonLoader__Registering__RegisterSampFunctions_Raknet)>(SSMP::PatternScan(moonloaderModule, forkedsignature_MoonLoader__Registering__RegisterSampFunctions_Raknet.c_str()));

            if(!MoonLoader__Registering__RegisterCleoFunctions || !MoonLoader__IsSampfuncLoaded || !MoonLoader__Registering__RegisterSampFunctions || !MoonLoader__Registering__RegisterSampFunctions_Raknet)
            {
                return;
            }
            if(IsCleoLoaded_forCleo5() && CleoGetVersion() >= 0x5000000)
            {
                MoonLoader__Registering__RegisterCleoFunctions(pointer);

                if(MoonLoader__IsSampfuncLoaded())
                {
                    MoonLoader__Registering__RegisterSampFunctions(pointer);
                    MoonLoader__Registering__RegisterSampFunctions_Raknet(pointer);
                }
            }

        });

        localthread.detach();
    }
}

MoonLoaderCleo5Instance::MoonLoaderCleo5Instance()
{
    if(GetModuleHandleA(MoonLoaderCleo5Instance::MoonloaderLibName.data()))
    {
        MoonLoaderCleo5Instance::ProcessMemory = remote::memory(GetCurrentProcessId());

        HMODULE Cleomodule = GetModuleHandleA(MoonLoaderCleo5Instance::CleoLibName.data());

        if(Cleomodule)
        {
            int (WINAPI* CleoGetVersion)() = GetProcAddress(Cleomodule, "_CLEO_GetVersion@0");
            if(CleoGetVersion() >= 0x5000000)
            {
                std::string forkedSignature;
                switch (Cleo5Moonloader::FindMoonloaderVersion())
                {
                    case Cleo5Moonloader::Version::Version_026_5:
                    {
                        forkedSignature = Cleo5Moonloader::Data::FindSignatureForVersion(Cleo5Moonloader::Data::CallingRegisterAllMoonloaderFunctionsSignature, Cleo5Moonloader::Version::Version_026_5);
                        break;
                    }
                    case Cleo5Moonloader::Version::Version_027_pr3:
                    {
                        forkedSignature = Cleo5Moonloader::Data::FindSignatureForVersion(Cleo5Moonloader::Data::CallingRegisterAllMoonloaderFunctionsSignature, Cleo5Moonloader::Version::Version_027_pr3);
                        break;
                    }
                    
                    default:
                    break;
                }
                
                if(!forkedSignature.empty())
                {
                    HookInstall::InstallCallHook(reinterpret_cast<uintptr_t>(SSMP::PatternScan(GetModuleHandleA(MoonLoaderCleo5Instance::MoonloaderLibName.data()), forkedSignature.c_str())), reinterpret_cast<uintptr_t>(Cleo5Moonloader::Hooks::Moonloader_RegisterFunctionsHook));
                }
            }        
        }

        Cleo5Moonloader::PatchMoonloaderMemory();
    }
    else
    {
        MessageBoxA(NULL, "MoonLoader Not found, you can continue gamme", 0, 0);
    }
}

void MoonLoaderCleo5Instance::asyncMoonloaderLog(std::string text)
{
    HMODULE moonloaderModule = GetModuleHandleA(MoonloaderLibName.data());
    static void* (*GetMoonloaderLogObject)() = nullptr;
    static void (__thiscall* MoonLoader__Log)(void* _this, const char* text) = nullptr;
                
    if(moonloaderModule)
    {
        if(GetMoonloaderLogObject == nullptr || MoonLoader__Log == nullptr)
        {
            GetMoonloaderLogObject = reinterpret_cast<decltype(GetMoonloaderLogObject)>(SSMP::PatternScan(moonloaderModule, "55 8B EC 64 A1 ? ? ? ? 6A ? 68 ? ? ? ? 50 64 89 25 ? ? ? ? A1 ? ? ? ? A8 ? 75 ? 83 C8 ? A3 ? ? ? ? 6A"));
            MoonLoader__Log = reinterpret_cast<decltype(MoonLoader__Log)>(SSMP::PatternScan(moonloaderModule, "55 8B EC 6A ? 68 ? ? ? ? 64 A1 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC ? 8B 55 ? 57"));
        }

        if(GetMoonloaderLogObject && MoonLoader__Log)
        {
            std::thread localthread = std::thread([=]()
            {
                while(!GetMoonloaderLogObject())
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
                MoonLoader__Log(GetMoonloaderLogObject(), text.c_str());
            });
            localthread.detach();
        }
    }
}

MoonLoaderCleo5Instance instance;
