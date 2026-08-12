#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/CoreMinimal.h"

#include "Public/Utils.h"
#include "Public/Configuration.h"
#include "Public/Finder.h"
#include "Public/Client.h"
#include "Gui/Public/Gui.h"

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

#include "FortniteGame/Public/FortGameSession.h"
#include "FortniteGame/Public/FortGameModeAthena.h"
#include "FortniteGame/Public/FortGameModeOutpost.h"
#include "FortniteGame/Public/Player/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortGameStateAthena.h"
#include "FortniteGame/Public/Mcp/McpProfileGroup.h"
#include "FortniteGame/Public/Building/BuildingActor.h"
#include "FortniteGame/Public/Building/BuildingTrap.h"
#include "FortniteGame/Public/Building/BuildingContainer.h"
#include "FortniteGame/Public/Building/BuildingGameplayActor.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/Items/FortPickup.h"
#include "FortniteGame/Public/Player/FortPlayerStateAthena.h"
#include "FortniteGame/Public/Pawns/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/Items/FortWorldItem.h"
#include "FortniteGame/Public/Analytics/FortAnalytics.h"
#include "FortniteGame/Public/FortGameInstance.h"
#include "FortniteGame/Public/FortGameSessionDedicated.h"
#include "FortniteGame/Public/Weapons/FortDecoTool.h"
#include "FortniteGame/Public/Quests/FortQuestManager.h"
#include "FortniteGame/Public/Building/BuildingItemCollectorActor.h"

#include "CrashReporter/Public/CrashReporter.h"

DWORD Main(LPVOID)
{
    FCrashReporter::Register();

    ConfigurationManager::LoadConfig();
    FCoreConfig& Config = ConfigurationManager::GetConfig();

    Finder::FindFMsg_Logf();

    GCanUseEngineLog = true;

    Finder::FindGUObjectArray();
    Finder::FindGIsClient();
    Finder::FindGIsServer();
    Finder::FindGEngine();
    Finder::FindGWorld();

    GEngine = reinterpret_cast<UEngine**>(ImageBase + ServerOffsets::GEngine);
    GWorld = reinterpret_cast<UWorld**>(ImageBase + ServerOffsets::GWorld);
    CoreGlobals::Init();

    Version::SetupVersion();

    Gui::Start();
    Utils::InitConsole(Config);

    Log(std::format("ImageBase: 0x{:X}", ImageBase).c_str());
    Log("FullVersion: " + Version::VersionString);
    Log(std::format("Engine Version: {}", Version::Engine_Version));
    if (Version::Fortnite_Version) {
        Log(std::format("Fortnite Build: {:.2f}", Version::Fortnite_Version));
        if (Config.bIsClient) {
            SetConsoleTitleA(std::format("CoreClient ({:.2f}) | Starting...", Version::Fortnite_Version).c_str());
            Gui::SetTitle(std::format("CoreClient ({:.2f}) | Starting...", Version::Fortnite_Version));
        }
        else {
            SetConsoleTitleA(std::format("Core ({:.2f}) | Starting...", Version::Fortnite_Version).c_str());
            Gui::SetTitle(std::format("Core ({:.2f}) | Starting...", Version::Fortnite_Version));
        }
    }
    Log(std::format("Fortnite CL: {}", Version::Fortnite_CL));

    Finder::SetupCoreOffsets();

    if (Config.bIsClient) {
        Client::Init(Config);
    }
    else {
        Utils::SetLogVerbosity();

        UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), "Fort.Pickup.ServerSideWeaponAutoPickupsEnabled 1", nullptr);

        if (!Config.bListenServer) {
            *GIsClient = false;
        }
        if (Version::Engine_Version > 4.20) {
            *GIsServer = true;
        }

        Finder::SetupOffsets();

        Utils::DumpGObjects(false);

        Utils::Hook();

        if (!Utils::SetupDedicatedServer(Config)) {
            Log("Failed to setup dedicated server!");
        }

        while (true) {
            UWorld* World = UWorld::GetWorld();
            if (World && World->GetName().ToString() != "Frontend" && World->AuthorityGameMode) {
                AGameMode* GameMode = World->AuthorityGameMode->Cast<AGameMode>();
                if (GameMode && GameMode->MatchState == MatchState::InProgress) {
                    break;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (!Config.bListenServer) {
            Utils::RemoveLocalPlayer();
        }

		if (!Utils::LoadWorld(Config)) {
			Log("Failed to load world!");
		}

        while (true) {
			UWorld* World = UWorld::GetWorld();
            if (World && World->GetName().ToString() != "FortniteEmptyDedicated" && World->AuthorityGameMode) {
				AGameMode* GameMode = World->AuthorityGameMode->Cast<AGameMode>();
				if (GameMode && GameMode->MatchState == MatchState::WaitingToStart) {
                    break;
				}
            }

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (Finder::FindCollectGarbageInternal()) {
            uintptr_t Patch2 = Finder::FindCollectGarbageInternal() + ImageBase;
            if (Patch2) {
                if (MH_CreateHook((LPVOID)Patch2, RetNull, nullptr) == MH_OK && MH_EnableHook((LPVOID)Patch2) == MH_OK) {
                    Log("Patched: " + std::to_string(Patch2 - ImageBase) + " with RetNull");
                }
                else {
                    Log("Failed to patch CollectGarbageInternal with RetNull!");
                }
            }
        }
    }

    return 0;
}

void Unload()
{
    Gui::Shutdown();

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
