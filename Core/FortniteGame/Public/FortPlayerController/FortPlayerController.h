#pragma once
#include "pch.h"
#include "Core/Public/Utils.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/LocalPlayer.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/OnlineReplStructs.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"
#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"

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
struct FFortItemEntryStateValue;
class UFortMcpProfileAccount;
class UFortMcpProfileAthena;
class UFortQuestManager;

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
	DefineUProperty(UFortMcpProfileAccount*, MainMcpProfile);
	DefineUProperty(UFortMcpProfileAthena*, AthenaProfile);
public:
	void ClientForceProfileQuery();

	static inline void (*OnReadyToStartMatchOG)(AFortPlayerController* This);
	static void OnReadyToStartMatch(AFortPlayerController* This);

	void SpawnQuickBars();

	void SetupQuickBars();

	static inline void (*ServerCheatOG)(AFortPlayerController* This, FString& Msg);
	static void ServerCheat(AFortPlayerController* This, FString& Msg);

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

	static inline bool (*RemoveInventoryItemOG)(AFortPlayerController* This, FGuid& ItemGuid, int32 Count, bool bForceRemoval);
	static bool RemoveInventoryItem(AFortPlayerController* This, FGuid& ItemGuid, int32 Count, bool bForceRemoval);

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

	static void ServerRemoveInventoryStateValue(AFortPlayerController* This, FGuid& ItemGuid, uint8 StateValueType);

	static void ServerSetInventoryStateValue(AFortPlayerController* This, FGuid& ItemGuid, FFortItemEntryStateValue& StateValue);

	UFortRegisteredPlayerInfo* GetRegisteredPlayerInfo() const;

	UFortQuestManager* GetQuestManager(uint8 SubGame) const;

	void ServerRepairBuildingActor(ABuildingSMActor* BuildingActorToRepair);
	static inline void (*execServerRepairBuildingActorOG)(AFortPlayerController* Context, FFrame& Stack);
	static void execServerRepairBuildingActor(AFortPlayerController* Context, FFrame& Stack);

	int32 PayBuildingRepairCost(ABuildingSMActor* BuildingToRepair);

	static inline void (*ServerPlayEmoteItemOG)(AFortPlayerController* This, UFortMontageItemDefinitionBase* EmoteAsset);
	static void ServerPlayEmoteItem(AFortPlayerController* This, UFortMontageItemDefinitionBase* EmoteAsset, float EmoteRandomNumber);

	static inline void (*GetPlayerViewPointOG)(AFortPlayerController* This, FVector& out_Location, FRotator& out_Rotation);
	static void GetPlayerViewPoint(AFortPlayerController* This, FVector& out_Location, FRotator& out_Rotation);

	FUniqueNetIdRepl GetGameAccountId() const;

	static void ServerAttemptInteract(AFortPlayerController* This, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, uint8 InteractType, UObject* OptionalObjectData);
	static inline void (*execServerAttemptInteractOG)(AFortPlayerController* Context, FFrame& Stack);
	static void execServerAttemptInteract(AFortPlayerController* Context, FFrame& Stack);

	FVector GetDropFinalLocation();

	void ClientExecuteInventoryItem(FGuid& ItemGuid, float Delay, bool bForceExecute, bool bActivateSlotAfterSettingFocused);

	static void Hook() {
		/*HookVTableIdx(
			AFortPlayerController::GetDefaultObj(),
			Finder::FindAFortPlayerController_OnReadyToStartMatchVFT(),
			OnReadyToStartMatch,
			(LPVOID*)&OnReadyToStartMatchOG
		);*/
		MH_CreateHook(
			(LPVOID)(GetOffsetFromVTable(
				AFortPlayerController::GetDefaultObj(),
				Finder::FindAFortPlayerController_OnReadyToStartMatchVFT()
			)),
			OnReadyToStartMatch,
			(LPVOID*)&OnReadyToStartMatchOG
		);

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
			HookEveryVTableIdx(
				AFortPlayerController::StaticClass(),
				ServerAttemptInventoryDropFunc->GetVTableIndex(),
				ServerAttemptInventoryDrop,
				nullptr
			);
		}
		else {
			HookEveryVTableIdx(
				AFortPlayerController::StaticClass(),
				AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerSpawnInventoryDrop")->GetVTableIndex(),
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

		/*HookEveryVTableIdx(
			AFortPlayerController::StaticClass(),
			Finder::FindAFortPlayerController_RemoveInventoryItemVFT(),
			RemoveInventoryItem,
			(LPVOID*)&RemoveInventoryItemOG
		);*/
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortPlayerController_RemoveInventoryItem()), RemoveInventoryItem, (LPVOID*)&RemoveInventoryItemOG);

		if (Version::Fortnite_Version <= 3.6 || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11) {
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

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerRemoveInventoryStateValue"),
			ServerRemoveInventoryStateValue
		);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerSetInventoryStateValue"),
			ServerSetInventoryStateValue
		);

		/*HookEveryVTableIdx(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerRepairBuildingActor")->GetVTableIndex(),
			ServerRepairBuildingActor,
			(LPVOID*)&ServerRepairBuildingActorOG
		);*/
		ExecHook(AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerRepairBuildingActor"), execServerRepairBuildingActor, execServerRepairBuildingActorOG);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerPlayEmoteItem"),
			ServerPlayEmoteItem,
			(LPVOID*)&ServerPlayEmoteItemOG
		);

		MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortPlayerController_GetPlayerViewPoint()), GetPlayerViewPoint, (LPVOID*)&GetPlayerViewPointOG);

		UFunction* ServerAttemptInteractFunc = AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerAttemptInteract");
		if (ServerAttemptInteractFunc) {
			/*HookEveryVTable(
				AFortPlayerController::StaticClass(),
				ServerAttemptInteractFunc,
				ServerAttemptInteract,
				(LPVOID*)&ServerAttemptInteractOG
			);*/
			ExecHook(ServerAttemptInteractFunc, execServerAttemptInteract, execServerAttemptInteractOG);
		}

		Log("Hooked AFortPlayerController");
	}
};