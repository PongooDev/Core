#include "pch.h"
#include "../Public/Utils.h"

#include "../Public/Patches.h"
#include "Gui/Public/Gui.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Engine.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DemoNetDriver.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/LocalPlayer.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameSession.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
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
#include "FortniteGame/Public/Building/BuildingRift.h"
#include "FortniteGame/Public/Building/BuildingFoundation.h"
#include "FortniteGame/Public/Building/BuildingGameplayActor.h"
#include "FortniteGame/Public/Building/BuildingItemCollectorActor.h"
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
#include "FortniteGame/Public/Athena/FortAthenaMapInfo.h"
#include "FortniteGame/Public/Athena/Vehicle/FortAthenaVehicle.h"
#include "FortniteGame/Public/World/FortPoiVolume.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_Heist.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_Barrier.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_GiveItemsAtPhase.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_ItemDropOnDeath.h"
#include "FortniteGame/Public/Capture/AthenaBarrierFlag.h"
#include "FortniteGame/Public/Athena/FortAthenaExitCraft.h"
#include "FortniteGame/Public/Inventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"
#include "FortniteGame/Public/Athena/FortAthenaExitCraftSpawner.h"
#include "FortniteGame/Public/Building/FortAthenaSupplyDrop.h"
#include "FortniteGame/Public/Player/FortPlayerControllerGameplay.h"


void Utils::InitConsole(FCoreConfig& Config)
{
    if (!Config.bIsClient || Config.bEnableClientConsole) {
        AllocConsole();
        FILE* fptr;
        freopen_s(&fptr, "CONOUT$", "w+", stdout);

        if (Gui::IsRunning()) {
            if (HWND ConsoleWindow = GetConsoleWindow()) {
                ShowWindow(ConsoleWindow, SW_HIDE);
            }
            Gui::CaptureConsoleOutput();
        }
    }
	if (Config.bIsClient) {
		SetConsoleTitleA("CoreClient (Finding Build) | Starting...");
		Gui::SetTitle("CoreClient (Finding Build) | Starting...");
		Log("Welcome to CoreClient, Made with love by Pongo_x86 and The Community!");
	}
	else {
		SetConsoleTitleA("Core (Finding Build) | Starting...");
		Gui::SetTitle("Core (Finding Build) | Starting...");
		Log("Welcome to Core, Made with love by Pongo_x86 and The Community!");
	}
}

uintptr_t Utils::ResolveRipRelative(uintptr_t instr, int dispOffset, int instrLen)
{
    int32_t disp = *reinterpret_cast<int32_t*>(instr + dispOffset);
    return instr + instrLen + disp;
}

void Utils::DumpGObjects(bool bLog) {
	if (!ServerOffsets::GUObjectArray) {
		Log("Cannot Dump GObjects, GObjects is nullptr!");
		return;
	}

	std::ofstream logFile("GObjects-Dump.txt", std::ios::trunc);
	if (!logFile.is_open()) {
		Log("Failed to open GObjects-Dump.txt!");
		return;
	}

	logFile << "==========GObjects-Dump==========\n";

	for (int i = 0; i < GUObjectArray.Num(); ++i)
	{
		FUObjectItem* Item = GUObjectArray.IndexToObject(i);
		if (!Item)
			continue;

		UObject* Object = (UObject*)Item->Object;

		if (!Object)
			continue;

		std::string fullName = Object->GetFullName();

		if (bLog) {
			Log(fullName);
		}

		logFile << fullName << std::endl;
	}

	logFile << "==========GObjects-Dump==========\n";

	logFile.close();

	Log("Dumped GObjects to GObjects-Dump.txt");
}

void Utils::DumpClassProperties(const char* ClassName)
{
	std::ofstream logFile(std::string(ClassName) + "_Properties.txt", std::ios::trunc);
	if (!logFile.is_open()) {
		Log("Failed to open file!");
		return;
	}

	UClass* Class = (UClass*)FUObjectArray::FindObjectFast(ClassName);
	if (!Class) {
		Log("Class not found: " + std::string(ClassName));
		return;
	}

	logFile << "==========Properties for " << ClassName << "==========\n";
	logFile << "Class Size: 0x" << std::format("{:X}", Class->PropertiesSize) << "\n\n";

	for (UField* Field = Class->GetChildrenOrChildProperties(); Field; Field = Field->Next)
	{
		if (!Field) continue;
		UProperty* Prop = (UProperty*)Field;

		std::string PropName = Prop->Name.ToString().ToString();
		std::string PropType = Prop->GetClass()->GetName().ToString();
		int32 Offset = Prop->Offset_Internal;
		int32 Size = Prop->ElementSize;

		logFile << std::format("0x{:04X} | {:40} | {:30} | Size: 0x{:X}\n",
			Offset, PropName, PropType, Size);
	}

	logFile << "==========End Properties==========\n";
	logFile.close();

	Log("Dumped properties for " + std::string(ClassName));
}

void Utils::DumpEnumProperties(const char* EnumName) {
	std::ofstream logFile(std::string(EnumName) + "_Properties.txt", std::ios::trunc);
	if (!logFile.is_open()) {
		Log("Failed to open file!");
		return;
	}

	UEnum* Enum = (UEnum*)FUObjectArray::FindObjectFast(EnumName);
	if (!Enum) {
		Log("Enum not found: " + std::string(EnumName));
		return;
	}

	logFile << "==========Properties for " << EnumName << "==========\n";

	auto Names = *(TArray<TPair<FName, int64>>*)(__int64(Enum) + 0x40);

	for (int i = 0; i < Names.Num(); i++)
	{
		auto& Pair = Names[i];
		auto& Name = Pair.Key();
		auto& Value = Pair.Value();

		if (Name.ComparisonIndex)
		{
			auto str = Name.ToString().ToString();
			auto colcolIdx = str.find_last_of("::");

			auto RealName = colcolIdx == -1 ? str : str.substr(colcolIdx + 1);

			logFile << std::format("{:40} | Value: {}\n", RealName, Value);
		}
	}

	logFile << "==========End Properties==========\n";
	logFile.close();

	Log("Dumped properties for " + std::string(EnumName));
}

uint8 Utils::GetEnumValueFromName(const char* EnumName, const char* EnumMemberName) {
	UEnum* Enum = (UEnum*)FUObjectArray::FindObjectFast(EnumName);
	if (!Enum) {
		Log("Enum not found: " + std::string(EnumName));
		return 0;
	}

	return Enum->GetValue(EnumMemberName);
}

void Utils::Hook() {
	MH_STATUS status = MH_Initialize();
	if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED) {
		Log(std::format("MH_Initialize failed with status: {}", MH_StatusToString(status)));
		return;
	}

	// Patches that i cant categorize
	Patches::ApplyPatches();

	// Engine
	UObject::Hook();
	UEngine::Hook();
	UWorld::Hook();
	AActor::Hook();
	UNetDriver::Hook();
	UNetConnection::Hook();
	UDemoNetDriver::Hook();
	AGameSession::Hook();
	UAbilitySystemComponent::Hook();
	APlayerController::Hook();
	AGameMode::Hook();
	AGameModeBase::Hook();
	UGameplayAbility::Hook();
	AController::Hook();

	// FortniteGame
	AFortAIDirector::Hook();
	AFortAIGoalManager::Hook();
	AFortGameSession::Hook();
	AFortGameModeAthena::Hook();
	AFortPlayerController::Hook();
	AFortGameState::Hook();
	AFortPlayerControllerZone::Hook();
	AFortGameModeZone::Hook();
	AFortGameStateZone::Hook();
	AFortGameMode::Hook();
	AFortGameStateAthena::Hook();
	AFortPlayerControllerAthena::Hook();
	UMcpProfileGroup::Hook();
	ABuildingActor::Hook();
	UFortKismetLibrary::Hook();
	ABuildingContainer::Hook();
	ABuildingRift::Hook();
	AFortPickup::Hook();
	AFortPlayerStateAthena::Hook();
	AFortPlayerPawn::Hook();
	ABuildingSMActor::Hook();
	UFortWorldItem::Hook();
	UFortAnalytics::Hook();
	UFortGameInstance::Hook();
	AFortGameSessionDedicated::Hook();
	AFortDecoTool::Hook();
	AFortGameModeOutpost::Hook();
	UFortQuestManager::Hook();
	ABuildingItemCollectorActor::Hook();
	AFortAthenaMapInfo::Hook();
	AFortAthenaSupplyDrop::Hook();
	AFortQuickBars::Hook();
	AFortAthenaVehicle::Hook();
	AFortPoiVolume::Hook();
	AFortAthenaMutator_Heist::Hook();
	AFortAthenaExitCraftSpawner::Hook();
	AFortAthenaMutator_Barrier::Hook();
	AFortAthenaMutator_GiveItemsAtPhase::Hook();
	AFortAthenaMutator_ItemDropOnDeath::Hook();
	AAthenaBarrierFlag::Hook();
	ABuildingFoundation::Hook();
	AFortPlayerControllerGameplay::Hook();

	status = MH_EnableHook(MH_ALL_HOOKS);
	if (status != MH_OK) {
		Log(std::format("MH_EnableHook failed with status: {}", MH_StatusToString(status)));
		return;
	}
}

FString Utils::GetDefaultMapName(FCoreConfig& Config) {
	if (!Config.MapToLoad.empty()) {
		return Config.MapToLoad;
	}

	FString MapName = L"Athena_Terrain";
	if (Version::Fortnite_Version >= 11.00 && Version::Fortnite_Version < 19.00) {
		MapName = L"Apollo_Terrain";
	}
	else if (Version::Fortnite_Version >= 19.00 && Version::Fortnite_Version < 23.00) {
		MapName = L"Artemis_Terrain";
	}
	else if (Version::Fortnite_Version >= 23.00 && Version::Fortnite_Version < 27.00) {
		MapName = L"Asteria_Terrain";
	}

	return MapName;
}

FString Utils::BuildTravelURL(FString MapName, std::vector<std::pair<std::string, std::string>> Options) {
	std::string MapNameStr = MapName.ToString();
	UWorld* World = (UWorld*)FUObjectArray::FindObjectFast(MapNameStr);
	std::string URL = "";
	if (!World) {
		//Log("Utils::BuildTravelURL: World not found: " + MapNameStr);
		URL = MapNameStr;
	}
	else {
		URL = World->GetPathName().ToString();
	}
	if (!Options.empty()) {
		for (size_t i = 0; i < Options.size(); ++i) {
			URL += "?";
			if (Options[i].second.empty()) {
				URL += Options[i].first;
			}
			else {
				URL += Options[i].first + "=" + Options[i].second;
			}
		}
	}

	return FString(std::wstring(URL.begin(), URL.end()).c_str());
}

void Utils::SetLogVerbosity() {
	FCoreConfig& Config = ConfigurationManager::GetConfig();
	if (Config.bIsProd) {
		return;
	}

    //UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogGarbage VeryVerbose", nullptr);
    //UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogAbilitySystem VeryVerbose", nullptr);
    //UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogAbilitySystemPrediction VeryVerbose", nullptr);

    //UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortAIDebug VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortBotMission VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortAnalytics VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortPlayerPawnAthena VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortSignificance VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortPerformance VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortMemory VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortClientBot VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortServerHitches VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortOutpost VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortPlacement VeryVerbose", nullptr);
    //UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortQuest VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortConnectivity VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortLootQuota VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortLoot VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortTeams VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortAnalyticsEvent VeryVerbose", nullptr);
    //UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortDamage VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortHealing VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortKnockback VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortStore VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortTooltip VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortCollectionBook VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortPlayerRegistration VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortServerBanning VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFort VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortInventory VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortInventoryUI VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortBuilding Verbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortWorld VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortAI VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortAIGoalSelection VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortThreatVisuals VeryVerbose", nullptr);
    //UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortWind VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortAIDirector VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortEQS VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortGameDifficulty VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortGameUI VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortLoadingScreen VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortDayNight VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortCustomization VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortFeedback VeryVerbose", nullptr);
    //UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortAbility VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortMission VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortMissionRewards VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortUserCloudHelper VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortHomebase VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortCheat VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortniteLlamaStoreAutomationTests VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortniteTransmogAutomationTests VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortniteVaultAutomationTests VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortniteCollctionBookAutomationTests VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortniteAutomationHelper VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortniteSkillTreeAutomationTests VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortniteClientAutomationHelper VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortniteInventoryAutomationTests VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortniteCrowdControlAutomationTests VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortniteCraftingAutomationTests VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortniteDestructibleAutomationTests VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortniteReplay VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortGameFeedback VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortUI VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortPlayerFeedback VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortNavigation VeryVerbose", nullptr);
	//UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogFortVoiceChat VeryVerbose", nullptr);
}

void Utils::RemoveLocalPlayer() {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("Utils::RemoveLocalPlayer: World is nullptr!");
		return;
	}

	UGameInstance* GameInstance = World->OwningGameInstance;
	if (!GameInstance || IsBadReadPtr(GameInstance)) {
		Log("Utils::RemoveLocalPlayer: OwningGameInstance is null or not readable yet!");
		return;
	}

	if (GameInstance->LocalPlayers.Num() > 0) {
		ULocalPlayer* LocalPlayer = GameInstance->LocalPlayers[0];
		APlayerController* PlayerController = (LocalPlayer && LocalPlayer->_HasPlayerController()) ? LocalPlayer->PlayerController : nullptr;

		if (PlayerController) {
			LocalPlayer->PlayerController = nullptr;
			PlayerController->K2_DestroyActor();
		}

		GameInstance->LocalPlayers.RemoveAt(0);
		Log("Utils::RemoveLocalPlayer: Local Player Removed!");
	}
}

bool Utils::SetupDedicatedServer(FCoreConfig& Config) {
	if (Config.bListenServer) {
		return false; // we dont wanna do this if we are a listen server (not a dedicated server)
	}

	UWorld* World = nullptr;
	while (true) {
		UWorld* CurrentWorld = UWorld::GetWorld();
		if (CurrentWorld && CurrentWorld->AuthorityGameMode && CurrentWorld->AuthorityGameMode->GetWorld()) {
			World = CurrentWorld;
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	FString TravelURL = "/Game/Maps/FortniteEmptyDedicated";

	UFortGameInstance* FortGameInstance = World->OwningGameInstance ? World->OwningGameInstance->Cast<UFortGameInstance>() : nullptr;

	const bool bUseGameInstance = FortGameInstance && FortGameInstance->GetWorld();

	Log(std::string("Utils::SetupDedicatedServer: travelling via ") + (bUseGameInstance ? "game instance" : "world"));

	bool bTravelOk = bUseGameInstance
		? FortGameInstance->ServerTravel(TravelURL)
		: World->ServerTravel(TravelURL);

	if (!bTravelOk) {
		Log("Utils::SetupDedicatedServer: ServerTravel failed!");
		return false;
	}

	return true;
}

bool Utils::LoadWorld(FCoreConfig& Config) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("Utils::LoadWorld: World is nullptr!");
		return false;
	}

	FString MapName = Utils::GetDefaultMapName(Config);
	Log("Loading World: " + MapName.ToString());

	std::vector<std::pair<std::string, std::string>> Options = {
		{"listen", ""},
		{"RequiredPlayers", "1"}
	};

	if (Config.Playlist.find_first_not_of("0123456789") == std::string::npos) {
		Options.push_back({ "playlistId", Config.Playlist });
	}
	else {
		Options.push_back({ "Playlist", Config.Playlist });
	}

	FString TravelURL = Utils::BuildTravelURL(MapName, Options);

	Log("Travel URL: " + TravelURL.ToString());

	UFortGameInstance* FortGameInstance = World->OwningGameInstance->Cast<UFortGameInstance>();

	bool bTravelOk = FortGameInstance
		? FortGameInstance->ServerTravel(TravelURL)
		: World->ServerTravel(TravelURL);

	if (!bTravelOk) {
		Log("Utils::LoadWorld: ServerTravel failed!");
		return false;
	}

	return true;
}

uintptr_t Utils::GetCallDestination(uintptr_t callAddr)
{
	int32_t rel = *reinterpret_cast<int32_t*>(callAddr + 1);

	uintptr_t nextInstr = callAddr + 5;

	return nextInstr + rel;
}

double Utils::NowSeconds()
{
	using Clock = std::chrono::high_resolution_clock;
	return std::chrono::duration<double>(Clock::now().time_since_epoch()).count();
}

UObject* Utils::GetObjectFromString(const std::string& InString, EClassCastFlags Flags) {
	std::string FinalString = InString;
	UObject* OutObject = nullptr;
	if (FinalString.contains("/")) {
		if (FinalString.starts_with("FortniteGame/"))
		{
			FinalString = "/Game/" + FinalString.substr(strlen("FortniteGame/"));
		}

		size_t contentPos = FinalString.find("/Content/");
		if (contentPos != std::string::npos)
		{
			if (FinalString.contains("/Game/Content/"))
			{
				FinalString.replace(FinalString.find("/Game/Content/"), strlen("/Game/Content/"), "/Game/");
			}
			else
			{
				size_t contentPos = FinalString.find("/Content/");
				FinalString = FinalString.substr(0, contentPos)
					+ "/Game/"
					+ FinalString.substr(contentPos + strlen("/Content/"));
			}
		}

		if (!FinalString.contains("."))
		{
			size_t lastSlash = FinalString.find_last_of('/');
			if (lastSlash != std::string::npos)
			{
				std::string className = FinalString.substr(lastSlash + 1);
				FinalString += "." + className;
			}
			else
			{
				return nullptr;
			}
		}

		OutObject = StaticLoadObject(FinalString);
	}
	else {
		OutObject = FUObjectArray::FindObject(FinalString, false, Flags);
	}

	return OutObject;
}

UObject* Utils::GetObjectFromString(const std::string& InString, UClass* Class) {
	std::string FinalString = InString;
	UObject* OutObject = nullptr;
	if (FinalString.contains("/")) {
		if (FinalString.starts_with("FortniteGame/"))
		{
			FinalString = "/Game/" + FinalString.substr(strlen("FortniteGame/"));
		}

		size_t contentPos = FinalString.find("/Content/");
		if (contentPos != std::string::npos)
		{
			if (FinalString.contains("/Game/Content/"))
			{
				FinalString.replace(FinalString.find("/Game/Content/"), strlen("/Game/Content/"), "/Game/");
			}
			else
			{
				size_t contentPos = FinalString.find("/Content/");
				FinalString = FinalString.substr(0, contentPos)
					+ "/Game/"
					+ FinalString.substr(contentPos + strlen("/Content/"));
			}
		}

		if (!FinalString.contains("."))
		{
			size_t lastSlash = FinalString.find_last_of('/');
			if (lastSlash != std::string::npos)
			{
				std::string className = FinalString.substr(lastSlash + 1);
				FinalString += "." + className;
			}
			else
			{
				return nullptr;
			}
		}

		OutObject = StaticLoadObject(FinalString);
	}
	else {
		OutObject = FUObjectArray::FindObjectWithClass(FinalString, false, Class);
	}

	return OutObject;
}

wchar_t* Utils::StringToWChar(const std::string& input) {
	if (input.empty()) {
		wchar_t* empty = (wchar_t*)malloc(sizeof(wchar_t));
		if (!empty) return nullptr;
		empty[0] = L'\0';
		return empty;
	}

	int sizeNeeded = MultiByteToWideChar(
		CP_UTF8,
		MB_ERR_INVALID_CHARS,
		input.c_str(),
		-1,
		nullptr,
		0
	);

	if (sizeNeeded == 0) {
		return nullptr;
	}

	wchar_t* buffer = (wchar_t*)malloc(sizeof(wchar_t) * sizeNeeded);
	if (!buffer) return nullptr;

	int result = MultiByteToWideChar(
		CP_UTF8,
		MB_ERR_INVALID_CHARS,
		input.c_str(),
		-1,
		buffer,
		sizeNeeded
	);

	if (result == 0) {
		free(buffer);
		return nullptr;
	}

	return buffer;
}

std::string Utils::StringToLower(const std::string& str) {
	std::string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return lowerStr;
}
