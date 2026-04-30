#pragma once
#include "pch.h"
#include "Core/Public/Utils.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"

class AFortPlayerPawn;
class ABuildingSMActor;
struct FCreateBuildingActorData;
enum class EFortResourceType : uint8;
class UFortMontageItemDefinitionBase;
class AFortQuickBars;
class AFortInventory;
class FFortItemEntry;
class UFortWorldItem;
class UFortItemDefinition;

class AFortPlayerController : public APlayerController {
public:
	DefineUnrealClass(AFortPlayerController);

	DefineUProperty(AFortPlayerPawn*, MyFortPawn);

	DefineUProperty(AFortInventory*, WorldInventory);

	DefineBitfieldUProperty(bBuildFree);

	DefineUProperty(AFortQuickBars*, ClientQuickBars);

	DefineUProperty(AFortQuickBars*, QuickBars);

	DefineUProperty(bool, bHasClientFinishedLoading);

	DefineUProperty(bool, bHasServerFinishedLoading);
public:
	void ClientForceProfileQuery();

	static inline void (*OnReadyToStartMatchOG)(AFortPlayerController* This);
	static void OnReadyToStartMatch(AFortPlayerController* This);

	void SpawnQuickBars();

	void SetupQuickBars();

	static inline void (*ServerCheatOG)(AFortPlayerController* This, FString* Msg);
	static void ServerCheat(AFortPlayerController* This, FString* Msg);

	static inline void (*ServerExecuteInventoryItemOG)(AFortPlayerController* This, FGuid& ItemGuid);
	static void ServerExecuteInventoryItem(AFortPlayerController* This, FGuid& ItemGuid);

	FFortItemEntry* FindItemEntry(FGuid Guid);
	FFortItemEntry* FindItemEntry(UFortItemDefinition* ItemDefinition);

	UFortWorldItem* FindItemInstance(FGuid Guid);
	UFortWorldItem* FindItemInstance(UFortItemDefinition* ItemDefinition);

	static void Hook() {
		/*HookVTableIdx(
			AFortPlayerController::GetDefaultObj(),
			Finder::FindAFortPlayerController_OnReadyToStartMatchVFT(),
			OnReadyToStartMatch,
			(LPVOID*)&OnReadyToStartMatchOG
		);*/
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortPlayerController_OnReadyToStartMatch()), OnReadyToStartMatch, (LPVOID*)&OnReadyToStartMatchOG);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerCheat"),
			ServerCheat,
			(LPVOID*)&ServerCheatOG
		);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerExecuteInventoryItem"),
			ServerExecuteInventoryItem,
			(LPVOID*)&ServerExecuteInventoryItemOG
		);

		Log("Hooked AFortPlayerController");
	}
};