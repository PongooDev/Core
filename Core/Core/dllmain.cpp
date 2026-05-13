#include "pch.h"

#include "Public/Utils.h"
#include "Public/Configuration.h"
#include "Public/Finder.h"
#include "Public/Client.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/Engine.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "FortniteGame/Public/BuildingActor/BuildingContainer.h"

DWORD Main(LPVOID)
{
    ConfigurationManager::LoadConfig();
    FCoreConfig& Config = ConfigurationManager::GetConfig();

    if (Config.bIsClient) {
        Client::Init(Config);
    }
    else {
		Utils::InitConsole(Config);

        Finder::FindGUObjectArray();
        Finder::FindGIsClient();
        Finder::FindGIsServer();
        Finder::FindGEngine();
        Finder::FindGWorld();

        Sleep(1000);

        GEngine = reinterpret_cast<UEngine**>(ImageBase + ServerOffsets::GEngine);
        GWorld = reinterpret_cast<UWorld**>(ImageBase + ServerOffsets::GWorld);
        CoreGlobals::Init();

        Version::SetupVersion();
        Log(std::format("ImageBase: 0x{:X}", ImageBase).c_str());
        Log("FullVersion: " + Version::VersionString);
        Log(std::format("Engine Version: {}", Version::Engine_Version));

        if (Version::Fortnite_Version) {
            Log(std::format("Fortnite Build: {:.2f}", Version::Fortnite_Version));
            SetConsoleTitleA(std::format("Core ({:.2f}) | Starting...", Version::Fortnite_Version).c_str());
        }
        Log(std::format("Fortnite CL: {}", Version::Fortnite_CL));

        Finder::SetupOffsets();

        Utils::DumpGObjects(false);
        Utils::DumpClassProperties("Engine"); // DEBUG
        Utils::DumpClassProperties("World"); // DEBUG
        Utils::DumpClassProperties("WorldContext"); // DEBUG
        Utils::DumpClassProperties("GameplayAbilitySpec"); // DEBUG
        Utils::DumpClassProperties("ItemAndCount"); // DEBUG
		Utils::DumpEnumProperties("EFortQuickBars"); // DEBUG
		Utils::DumpClassProperties("FortItemEntryStateValue"); // DEBUG
		Utils::DumpClassProperties("FortHeroSpecialization"); // DEBUG

        Utils::Hook();

        *GIsClient = false;
        /*
            Setting GIsClient and GIsServer crashes unless you only set one of them, however
            setting GIsServer to true and not setting GIsClient to false crashes when the client connects
            so the best way to get around this is to only set GIsClient to false and not set GIsServer
            I will hopefully fix this and be able to set both in the future.
        */
        if (Version::Fortnite_Version >= 1.82) {
            *GIsServer = true;
        }

        Sleep(1000);

        Utils::SetLogVerbosity();

        Utils::RemoveLocalPlayer();
        Utils::LoadWorld();
    }

    return 0;
}

void Unload()
{
    Log("Core is unloading...");

    MH_Uninitialize();

    if (GetConsoleWindow()) {
        FreeConsole();
    }

    Log("Core has been unloaded!");
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, Main, 0, 0, 0);
        break;
    case DLL_PROCESS_DETACH:
        Unload();
        break;
    }
    return TRUE;
}
