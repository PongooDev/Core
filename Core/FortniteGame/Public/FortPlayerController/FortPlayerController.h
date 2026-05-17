#pragma once
#include "pch.h"
#include "Core/Public/Utils.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/LocalPlayer.h"

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
class UFortRegisteredPlayerInfo;
class UFortMcpProfileWorld;
struct FBuildingClassData;

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

	DefineUProperty(UFortRegisteredPlayerInfo*, MyPlayerInfo);
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

	void ClientReportDamagedResourceBuilding(ABuildingSMActor* BuildingSMActor, uint8 PotentialResourceType, int32 PotentialResourceCount, bool bDestroyed, bool bJustHitWeakspot);

	static void ServerAttemptInventoryDrop(AFortPlayerController* This, FGuid& ItemGuid, int Count, bool bTrash);

	void ClientForceUpdateQuickbar(uint8 QuickbarToRefresh);

	void OnRep_QuickBar();

	static bool IsUsingOldQuickBars();

	static inline void (*ServerClientPawnLoadedOG)(AFortPlayerController* This, bool bIsPawnLoaded);
	static void ServerClientPawnLoaded(AFortPlayerController* This, bool bIsPawnLoaded);

	static inline void (*ServerSpotActorOG)(AFortPlayerController* This, AActor* NewlySpottedActor);
	static void ServerSpotActor(AFortPlayerController* This, AActor* NewlySpottedActor);

	static inline bool (*RemoveInventoryItemOG)(AFortPlayerController* This, FGuid* ItemGuid, int32 Count, bool bForceRemoval);
	static bool RemoveInventoryItem(AFortPlayerController* This, FGuid* ItemGuid, int32 Count, bool bForceRemoval);

	static inline void (*ServerCreateBuildingActorOldOG)(AFortPlayerController* This, FBuildingClassData& BuildingClassData, FVector& BuildLoc, FRotator& BuildRot, bool bMirrored);
	static void ServerCreateBuildingActorOld(AFortPlayerController* This, FBuildingClassData& BuildingClassData, FVector& BuildLoc, FRotator& BuildRot, bool bMirrored);

	bool CanAffordToPlaceBuildableClass(FBuildingClassData* ClassToBuildData);

	int32 PayBuildableClassPlacementCost(FBuildingClassData* ClassToBuildData);

	static inline void (*ServerBeginEditingBuildingActorOG)(AFortPlayerController* This, ABuildingSMActor* BuildingActorToEdit);
	static void ServerBeginEditingBuildingActor(AFortPlayerController* This, ABuildingSMActor* BuildingActorToEdit);

	static inline void (*ServerEditBuildingActorOG)(AFortPlayerController* This, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, int32 RotationIterations, bool bMirrored);
	static void ServerEditBuildingActor(AFortPlayerController* This, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, int32 RotationIterations, bool bMirrored);

	static inline void (*ServerEndEditingBuildingActorOG)(AFortPlayerController* This, ABuildingSMActor* BuildingActorToStopEditing);
	static void ServerEndEditingBuildingActor(AFortPlayerController* This, ABuildingSMActor* BuildingActorToStopEditing);

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

		UFunction* ServerAttemptInventoryDropFunc = AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerAttemptInventoryDrop");
		if (ServerAttemptInventoryDropFunc) {
			HookEveryVTable(
				AFortPlayerController::StaticClass(),
				ServerAttemptInventoryDropFunc,
				ServerAttemptInventoryDrop,
				nullptr
			);
		}
		else {
			HookEveryVTable(
				AFortPlayerController::StaticClass(),
				AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerSpawnInventoryDrop"),
				ServerAttemptInventoryDrop,
				nullptr
			);
		}

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerClientPawnLoaded"),
			ServerClientPawnLoaded,
			(LPVOID*)&ServerClientPawnLoadedOG
		);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerSpotActor"),
			ServerSpotActor,
			(LPVOID*)&ServerSpotActorOG
		);

		HookEveryVTableIdx(
			AFortPlayerController::StaticClass(),
			Finder::FindAFortPlayerController_RemoveInventoryItemVFT(),
			RemoveInventoryItem,
			(LPVOID*)&RemoveInventoryItemOG
		);

		if (Version::Fortnite_Version <= 1.72) {
			HookEveryVTable(
				AFortPlayerController::StaticClass(),
				AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerCreateBuildingActor"),
				ServerCreateBuildingActorOld,
				(LPVOID*)&ServerCreateBuildingActorOldOG
			);
		}

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerBeginEditingBuildingActor"),
			ServerBeginEditingBuildingActor,
			(LPVOID*)&ServerBeginEditingBuildingActorOG
		);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerEditBuildingActor"),
			ServerEditBuildingActor,
			(LPVOID*)&ServerEditBuildingActorOG
		);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerEndEditingBuildingActor"),
			ServerEndEditingBuildingActor,
			(LPVOID*)&ServerEndEditingBuildingActorOG
		);

		Log("Hooked AFortPlayerController");
	}
};