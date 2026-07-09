#include "pch.h"

#include "Public/Utils.h"
#include "Public/Configuration.h"
#include "Public/Finder.h"
#include "Public/Client.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/Engine.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Core/Public/Templates/SharedPointer.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DemoNetDriver.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/LocalPlayer.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameSession.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"

#include "FortniteGame/Public/FortGameSession/FortGameSession.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeOutpost.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"
#include "FortniteGame/Public/Mcp/McpProfileGroup.h"
#include "FortniteGame/Public/BuildingActor/BuildingActor.h"
#include "FortniteGame/Public/BuildingActor/BuildingTrap.h"
#include "FortniteGame/Public/BuildingActor/BuildingContainer.h"
#include "FortniteGame/Public/BuildingActor/BuildingGameplayActor.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/FortPickup/FortPickup.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortItem/FortWorldItem.h"
#include "FortniteGame/Public/FortAnalytics/FortAnalytics.h"
#include "FortniteGame/Public/FortGameInstance/FortGameInstance.h"
#include "FortniteGame/Public/FortGameSession/FortGameSessionDedicated.h"
#include "FortniteGame/Public/FortWeapon/FortDecoTool.h"
#include "FortniteGame/Public/FortQuest/FortQuestManager.h"
#include "FortniteGame/Public/BuildingActor/BuildingItemCollectorActor.h"

#include "CrashReporter/Public/CrashReporter.h"

DWORD Main(LPVOID)
{
    FCrashReporter::Register();

    ConfigurationManager::LoadConfig();
    FCoreConfig& Config = ConfigurationManager::GetConfig();

    Finder::FindFMsg_Logf();

    Utils::InitConsole(Config);

    Finder::FindGUObjectArray();
    Finder::FindGIsClient();
    Finder::FindGIsServer();
    Finder::FindGEngine();
    Finder::FindGWorld();

    GCanUseEngineLog = true;

    GEngine = reinterpret_cast<UEngine**>(ImageBase + ServerOffsets::GEngine);
    GWorld = reinterpret_cast<UWorld**>(ImageBase + ServerOffsets::GWorld);
    CoreGlobals::Init();

    Finder::SetupCoreOffsets();

    if (Config.bIsClient) {
        Client::Init(Config);
    }
    else {
        Utils::SetLogVerbosity();

        UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), "Fort.Pickup.ServerSideWeaponAutoPickupsEnabled 1", nullptr);

        Version::SetupVersion();
        Log(std::format("ImageBase: 0x{:X}", ImageBase).c_str());
        Log("FullVersion: " + Version::VersionString);
        Log(std::format("Engine Version: {}", Version::Engine_Version));

        if (Version::Fortnite_Version) {
            Log(std::format("Fortnite Build: {:.2f}", Version::Fortnite_Version));
            SetConsoleTitleA(std::format("Core ({:.2f}) | Starting...", Version::Fortnite_Version).c_str());
        }
        Log(std::format("Fortnite CL: {}", Version::Fortnite_CL));

        if (!Config.bListenServer) {
            *GIsClient = false;
        }
        if (Version::Fortnite_Version >= 4) {
            *GIsServer = true;
        }

        Sleep(2000);

        Finder::SetupOffsets();

        Utils::DumpGObjects(false);
        Utils::DumpClassProperties("NetDriver"); // DEBUG

        Utils::Hook();

        Sleep(1000);

        if (!Config.bListenServer) {
            if (!Utils::SetupDedicatedServer(Config)) {
                Log("Failed to setup dedicated server!");
            }

            while (true) {
                UWorld* World = UWorld::GetWorld();
                if (World && World->AuthorityGameMode && World->GetName().ToString() != "Frontend") {
                    AGameMode* GameMode = World->AuthorityGameMode->Cast<AGameMode>();
                    if (GameMode && GameMode->MatchState == MatchState::InProgress) {
                        break;
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

		if (!Config.bListenServer) {
            Utils::RemoveLocalPlayer();
		}

		if (!Utils::LoadWorld(Config)) {
			Log("Failed to load world!");
		}

        while (true) {
			UWorld* World = UWorld::GetWorld();
            if (World && World->AuthorityGameMode && World->GetName().ToString() != "FortniteEmptyDedicated") {
				AGameMode* GameMode = World->AuthorityGameMode->Cast<AGameMode>();
				if (GameMode && GameMode->MatchState == MatchState::InProgress) {
                    break;
				}
            }

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (Finder::FindCollectGarbageInternal()) {
            uintptr_t Patch2 = Finder::FindCollectGarbageInternal() + ImageBase;
            if (Patch2) {
                MH_CreateHook((LPVOID)Patch2, RetNull, nullptr);
                Log("Patched: " + std::to_string(Patch2 - ImageBase) + " with RetNull");
            }
        }
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
