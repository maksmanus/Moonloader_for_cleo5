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
    static bool IsCleo5Hooked = false, IsCleoChecked = false;
    HMODULE moonloaderModule = GetModuleHandleA(MoonLoaderCleo5Instance::MoonloaderLibName.data());
    auto FoundVersion = FindMoonloaderVersion();
    std::string forkedSignature = Cleo5Moonloader::Data::FindSignatureForVersion(Cleo5Moonloader::Data::RegisterAllMoonloaderFunctionsSignature, FoundVersion);

    if(forkedSignature.empty())
    {
        return;
    }

    auto calling_lambda = [pointer, moonloaderModule, FoundVersion]()
    {
        std::string forkedsignature_MoonLoader__Registering__RegisterCleoFunctions = Cleo5Moonloader::Data::FindSignatureForVersion(Cleo5Moonloader::Data::MoonLoaderRegisteringRegisterCleoFunctionsSignature, FoundVersion);
        std::string forkedsignature_MoonLoader__IsSampfuncLoaded = Cleo5Moonloader::Data::FindSignatureForVersion(Cleo5Moonloader::Data::MoonLoaderIsSampfuncLoadedSignature, FoundVersion);
        std::string forkedsignature_MoonLoader__Registering__RegisterSampFunctions = Cleo5Moonloader::Data::FindSignatureForVersion(Cleo5Moonloader::Data::MoonLoaderRegisteringRegisterSampFunctionsSignature, FoundVersion);
        std::string forkedsignature_MoonLoader__Registering__RegisterSampFunctions_Raknet = Cleo5Moonloader::Data::FindSignatureForVersion(Cleo5Moonloader::Data::MoonLoaderRegisteringRegisterSampFunctionsRaknetSignature, FoundVersion); 
        std::string forkedSignature_Moonloader_IsCleoLoaded = Cleo5Moonloader::Data::FindSignatureForVersion(Cleo5Moonloader::Data::IsCleoLoadedSignature, FoundVersion);

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
            if(!IsCleo5Hooked && !IsCleo5Hooked)
            {
                void* TargetFunction = reinterpret_cast<void*>(SSMP::PatternScan(moonloaderModule, forkedSignature_Moonloader_IsCleoLoaded.c_str()));
                if(TargetFunction)
                {
                    HookInstall::InstallJumpHook(reinterpret_cast<uintptr_t>(TargetFunction), reinterpret_cast<uintptr_t>(IsCleoLoaded_forCleo5));
                    IsCleo5Hooked = true;
                }
                IsCleoChecked = true;
            }
            MoonLoader__Registering__RegisterCleoFunctions(pointer);
    
            if(MoonLoader__IsSampfuncLoaded())
            {
                MoonLoader__Registering__RegisterSampFunctions(pointer);
                MoonLoader__Registering__RegisterSampFunctions_Raknet(pointer);
            }
        }
    };

    char (*original)(void*) = reinterpret_cast<decltype(original)>(SSMP::PatternScan(moonloaderModule, forkedSignature.c_str()));
    if(original)
    {
        original(pointer);
        if(!IsCleoLoaded_forCleo5())
        {
            std::thread localthread = std::thread(calling_lambda);
            localthread.detach();
        }
        else
        {
            calling_lambda();
        }
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
                {;
                    HookInstall::InstallCallHook(reinterpret_cast<uintptr_t>(SSMP::PatternScan(GetModuleHandleA(MoonLoaderCleo5Instance::MoonloaderLibName.data()), forkedSignature.c_str())), reinterpret_cast<uintptr_t>(Cleo5Moonloader::Hooks::Moonloader_RegisterFunctionsHook));
                }
            }        
        }

        Cleo5Moonloader::PatchMoonloaderMemory();
    }
    else
    {
        //MessageBoxA(NULL, "MoonLoader Not found, you can continue game", 0, 0);
    }
}

MoonLoaderCleo5Instance instance;
