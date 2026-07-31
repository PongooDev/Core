#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptDelegates.h"
#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

#include "FortniteGame/Public/Quests/FortQuestObjectiveCompletion.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

class UFortQuestItem;
struct FScriptContainerElement;
class AFortPlayerController;
struct FFortQuestObjectiveCompletion;
class UFortQuestItemDefinition;

class EFortQuestObjectiveStatEvent {
public:
	DefineUnrealEnum(EFortQuestObjectiveStatEvent);

	DefineEnumProperty(Kill);
	DefineEnumProperty(TeamKill);
	DefineEnumProperty(KillContribution);
	DefineEnumProperty(Build);
	DefineEnumProperty(BuildingEdit);
	DefineEnumProperty(BuildingRepair);
	DefineEnumProperty(BuildingUpgrade);
	DefineEnumProperty(Complete);
	DefineEnumProperty(Craft);
	DefineEnumProperty(Collect);
	DefineEnumProperty(Win);
	DefineEnumProperty(Interact);
	DefineEnumProperty(Destroy);
	DefineEnumProperty(Ability);
	DefineEnumProperty(WaveComplete);
	DefineEnumProperty(Custom);
	DefineEnumProperty(Client);
	DefineEnumProperty(AthenaRank);
	DefineEnumProperty(AthenaOutlive);
	DefineEnumProperty(NumGameplayEvents);
	DefineEnumProperty(Acquire);
	DefineEnumProperty(Consume);
	DefineEnumProperty(OpenCardPack);
	DefineEnumProperty(PurchaseCardPack);
	DefineEnumProperty(Convert);
	DefineEnumProperty(Upgrade);
	DefineEnumProperty(QuestComplete);
	DefineEnumProperty(AssignWorker);
	DefineEnumProperty(CollectExpedition);
	DefineEnumProperty(CollectSuccessfulExpedition);
	DefineEnumProperty(LevelUpCollectionBook);
	DefineEnumProperty(HasItem);
	DefineEnumProperty(SlotInCollection);
	DefineEnumProperty(HasCompletedQuest);
	DefineEnumProperty(HasAssignedWorker);
	DefineEnumProperty(HasUpgraded);
	DefineEnumProperty(HasConverted);
	DefineEnumProperty(HasLeveledUpCollectionBook);
	DefineEnumProperty(Max_None);
};

class EStatMod {
public:
	DefineUnrealEnum(EStatMod);

	DefineEnumProperty(Delta);
	DefineEnumProperty(Set);
	DefineEnumProperty(Maximum);
};

class UFortQuestManager : public UObject {
public:
	DefineUnrealClass(UFortQuestManager);

	DefineUProperty(FMulticastScriptDelegate, OnQuestsUpdated);
	DefineUProperty(FMulticastScriptDelegate, OnQuestsCompleted);
	DefineUProperty(FMulticastScriptDelegate, OnQuestsGranted);
	DefineUProperty(FMulticastScriptDelegate, OnQuestRewardClaimed);
	DefineUProperty(FMulticastScriptDelegate, OnQuestSeen);
	DefineUProperty(FMulticastScriptDelegate, OnNoQuestRewardsToClaim);
	DefineUProperty(FMulticastScriptDelegate, OnPinnedQuestsChanged);
	DefineUProperty(FMulticastScriptDelegate, OnDailyQuestRerolled);
	DefineUProperty(FMulticastScriptDelegate, OnDisplayDynamicQuestUpdate);
	DefineUProperty(TArray<UFortQuestItem*>, CurrentQuests);
	DefineUProperty(TArray<FFortQuestObjectiveCompletion>, PendingChanges);
	DefineUProperty(TArray<FString>, ActiveEventFlags);
public:
	static inline void (*SendCustomStatEventOG)(UFortQuestManager* This, FDataTableRowHandle& ObjectiveStat, int32 Count, bool bForceFlush);
	static void SendCustomStatEvent(UFortQuestManager* This, FDataTableRowHandle& ObjectiveStat, int32 Count, bool bForceFlush);

	static inline void (*SendStatEventOG)(
		UFortQuestManager* This,
		FDataTableRowHandle* InObjectiveStat,
		EFortQuestObjectiveStatEvent InType,
		UObject* InTargetObject,
		FGameplayTagContainer& InTargetTags,
		FGameplayTagContainer& InSourceTags,
		FGameplayTagContainer& InContextTags,
		int32 InCount,
		bool bForceFlush
	);
	void SendStatEvent(
		FDataTableRowHandle* InObjectiveStat,
		EFortQuestObjectiveStatEvent InType,
		UObject* InTargetObject,
		FGameplayTagContainer& InTargetTags,
		FGameplayTagContainer& InSourceTags,
		FGameplayTagContainer& InContextTags,
		int32 InCount,
		bool bForceFlush
	);
	static void SendStatEventHook(
		UFortQuestManager* This,
		FDataTableRowHandle* InObjectiveStat,
		EFortQuestObjectiveStatEvent InType,
		UObject* InTargetObject,
		FGameplayTagContainer& InTargetTags,
		FGameplayTagContainer& InSourceTags,
		FGameplayTagContainer& InContextTags,
		int32 InCount,
		bool bForceFlush
	);

	FScriptContainerElement* ProcessPendingStatEvents();

	AFortPlayerController* GetPlayerControllerBP();

	void ProgressQuest(UFortQuestItem* QuestItem, FName ObjectiveBackendName, int32 InCount = 1);

	void DisplayQuestUpdate(AFortPlayerController* PlayerController, UFortQuestItemDefinition* QuestDefinition, FName ObjectiveBackendName, int32 NewCount, int32 Delta);

	void ForceTriggerQuestsUpdated();

	void GetSourceAndContextTags(FGameplayTagContainer* OutSourceTags, FGameplayTagContainer* OutContextTags) const;

	UFortQuestItem* GetQuestWithDefinition(UFortQuestItemDefinition* Definition);

	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindUFortQuestManager_SendCustomStatEvent()), (LPVOID)SendCustomStatEvent, (LPVOID*)&SendCustomStatEventOG);
		
		if (Finder::FindUFortQuestManager_SendStatEvent()) {
			MH_CreateHook((LPVOID)(ImageBase + Finder::FindUFortQuestManager_SendStatEvent()), (LPVOID)SendStatEventHook, (LPVOID*)&SendStatEventOG);
		}

		Log("UFortQuestManager Hooked!");
	}
};
