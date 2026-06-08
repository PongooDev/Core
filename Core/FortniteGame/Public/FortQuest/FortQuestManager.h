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
#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

#include "FortniteGame/Public/FortQuest/FortQuestObjectiveCompletion.h"

class UFortQuestItem;
struct FScriptContainerElement;
class AFortPlayerController;
struct FFortQuestObjectiveCompletion;

class UFortQuestManager : public UObject {
public:
	DefineUnrealClass(UFortQuestManager);

	DefineUProperty(TArray<UFortQuestItem*>, CurrentQuests);
	DefineUProperty(TArray<FFortQuestObjectiveCompletion>, PendingChanges);
	DefineUProperty(TArray<FString>, ActiveEventFlags);
public:
	static inline void (*SendCustomStatEventOG)(UFortQuestManager* This, FDataTableRowHandle& ObjectiveStat, int32 Count, bool bForceFlush);
	static void SendCustomStatEvent(UFortQuestManager* This, FDataTableRowHandle& ObjectiveStat, int32 Count, bool bForceFlush);

	static inline void (*SendStatEventOG)(
		UFortQuestManager* This,
		FScriptContainerElement* InObjectiveStat,
		uint8 InType,
		UObject* InTargetObject,
		FGameplayTagContainer* InTargetTags,
		FGameplayTagContainer* InSourceTags,
		FGameplayTagContainer* InContextTags,
		int32 InCount,
		bool bForceFlush
	);
	static void SendStatEvent(
		UFortQuestManager* This,
		FScriptContainerElement* InObjectiveStat,
		uint8 InType,
		UObject* InTargetObject,
		FGameplayTagContainer* InTargetTags,
		FGameplayTagContainer* InSourceTags,
		FGameplayTagContainer* InContextTags,
		int32 InCount,
		bool bForceFlush
	);

	FScriptContainerElement* ProcessPendingStatEvents();

	AFortPlayerController* GetPlayerControllerBP();

	void ProgressQuest(UFortQuestItem* QuestItem, FName ObjectiveBackendName, int32 InCount = 1);

	void ForceTriggerQuestsUpdated();

	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindUFortQuestManager_SendCustomStatEvent()), (LPVOID)SendCustomStatEvent, (LPVOID*)&SendCustomStatEventOG);
		
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindUFortQuestManager_SendStatEvent()), (LPVOID)SendStatEvent, (LPVOID*)&SendStatEventOG);

		Log("UFortQuestManager Hooked!");
	}
};