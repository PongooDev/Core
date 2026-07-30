#include "pch.h"
#include "FortniteGame/Public/Quests/FortQuestManager.h"

#include "FortniteGame/Public/Items/FortQuestItem.h"
#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"
#include "FortniteGame/Public/Quests/FortQuestObjectiveStatTableRow.h"
#include "FortniteGame/Public/Quests/FortQuestObjectiveInfo.h"
#include "FortniteGame/Public/Quests/FortQuestObjectiveCompletion.h"
#include "FortniteGame/Public/Player/FortRegisteredPlayerInfo.h"
#include "FortniteGame/Public/Player/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAthena.h"
#include "FortniteGame/Public/Mcp/McpProfileSys.h"
#include "FortniteGame/Public/Items/Definitions/FortQuestItemDefinition.h"
#include "FortniteGame/Public/FortEnums.h"

void UFortQuestManager::SendCustomStatEvent(UFortQuestManager* This, FDataTableRowHandle& ObjectiveStat, int32 Count, bool bForceFlush) {
	Log("SendCustomStatEvent Called!");

	for (UFortQuestItem* QuestItem : This->CurrentQuests) {
		if (QuestItem->HasCompletedQuest())
			continue;

		UFortQuestItemDefinition* QuestDefinition = QuestItem->ItemDefinition->Cast<UFortQuestItemDefinition>();
		if (!QuestDefinition)
			continue;

		UDataTable* ObjectiveStatTable = ObjectiveStat.DataTable;
		FName& ObjectiveStatRowName = ObjectiveStat.RowName;

		FGameplayTagContainer SourceTags;
		FGameplayTagContainer ContextTags;
		This->GetSourceAndContextTags(&SourceTags, &ContextTags);

		UFortQuestObjectiveInfo* ObjectiveInfo = QuestItem->GetObjectiveInfo(ObjectiveStat);
		if (!ObjectiveInfo) {
			continue;
		}

		for (int32 i = 0; i < QuestDefinition->Objectives.Num(); i++) {
			FFortMcpQuestObjectiveInfo& Objective = QuestDefinition->Objectives.GetWithSize(i, FFortMcpQuestObjectiveInfo::GetSize());
			if (QuestItem->HasCompletedObjectiveWithName(Objective.BackendName))
				continue;

			FDataTableRowHandle ObjectiveStatHandle = Objective.ObjectiveStatHandle;
			if (!Objective.ObjectiveStatHandle.RowName.IsNone()
				&& ObjectiveStatTable == ObjectiveStatHandle.DataTable
				&& ObjectiveStatRowName == ObjectiveStatHandle.RowName) {
				uint8* const* RowPtr = ObjectiveStatHandle.DataTable->RowMap.Find(ObjectiveStatHandle.RowName);
				if (RowPtr && *RowPtr) {
					FFortQuestObjectiveStatTableRow* Value = (FFortQuestObjectiveStatTableRow*)*RowPtr;

					if (SourceTags.HasAll(Value->SourceTagContainer)
						&& ContextTags.HasAll(Value->ContextTagContainer)) {
						This->ProgressQuest(QuestItem, Objective.BackendName, Count);
					}
				}
			}
		}
	}

	if (SendStatEventOG) {
		return SendCustomStatEventOG(This, ObjectiveStat, Count, bForceFlush);
	}
}

void UFortQuestManager::SendStatEvent(
	FDataTableRowHandle* InObjectiveStat,
	EFortQuestObjectiveStatEvent InType,
	UObject* InTargetObject,
	FGameplayTagContainer& InTargetTags,
	FGameplayTagContainer& InSourceTags,
	FGameplayTagContainer& InContextTags,
	int32 InCount,
	bool bForceFlush
) {
	AFortPlayerController* PlayerController = GetPlayerControllerBP();
	if (!PlayerController) {
		Log("UFortQuestManager::SendStatEvent: PlayerController is null!");
		return;
	}

	for (UFortQuestItem* QuestItem : CurrentQuests) {
		if (QuestItem->HasCompletedQuest())
			continue;

		UFortQuestItemDefinition* QuestDefinition = QuestItem->ItemDefinition->Cast<UFortQuestItemDefinition>();
		if (!QuestDefinition)
			continue;

		for (int32 i = 0; i < QuestDefinition->Objectives.Num(); i++) {
			FFortMcpQuestObjectiveInfo& Objective = QuestDefinition->Objectives.GetWithSize(i, FFortMcpQuestObjectiveInfo::GetSize());
			if (QuestItem->HasCompletedObjectiveWithName(Objective.BackendName))
				continue;

			FDataTableRowHandle ObjectiveStatHandle = Objective.ObjectiveStatHandle;
			if (!Objective.ObjectiveStatHandle.RowName.IsNone() && ObjectiveStatHandle.DataTable) {
				uint8* const* RowPtr = ObjectiveStatHandle.DataTable->RowMap.Find(ObjectiveStatHandle.RowName);
				if (RowPtr && *RowPtr) {
					FFortQuestObjectiveStatTableRow* Value = (FFortQuestObjectiveStatTableRow*)*RowPtr;

					if (Value->Type == InType
						&& InTargetTags.HasAll(Value->TargetTagContainer)
						&& InSourceTags.HasAll(Value->SourceTagContainer)) {
						/* InContextTags->HasAll(Value->ContextTagContainer) intentionally not checked */
						ProgressQuest(QuestItem, Objective.BackendName, InCount);
					}
				}
			}
		}
	}
}

void UFortQuestManager::SendStatEventHook(
	UFortQuestManager* This,
	FDataTableRowHandle* InObjectiveStat,
	EFortQuestObjectiveStatEvent InType,
	UObject* InTargetObject,
	FGameplayTagContainer& InTargetTags,
	FGameplayTagContainer& InSourceTags,
	FGameplayTagContainer& InContextTags,
	int32 InCount,
	bool bForceFlush
) {
	This->SendStatEvent(InObjectiveStat, InType, InTargetObject, InTargetTags, InSourceTags, InContextTags, InCount, bForceFlush);
	if (SendStatEventOG) {
		SendStatEventOG(This, InObjectiveStat, InType, InTargetObject, InTargetTags, InSourceTags, InContextTags, InCount, bForceFlush);
	}
}

FScriptContainerElement* UFortQuestManager::ProcessPendingStatEvents() {
	FScriptContainerElement* (*ProcessPendingStatEventsInternal)(UFortQuestManager*) = decltype(ProcessPendingStatEventsInternal)(ImageBase + Finder::FindUFortQuestManager_ProcessPendingStatEvents());
	return ProcessPendingStatEventsInternal(this);
}

AFortPlayerController* UFortQuestManager::GetPlayerControllerBP() {
	if (Version::Fortnite_Version <= 8.30 || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11) {
		UWorld* World = UWorld::GetWorld();
		if (!World) {
			Log("UFortQuestManager::GetPlayerControllerBP: World is null!");
			return nullptr;
		}
		
		UFortRegisteredPlayerInfo* PlayerInfo = OuterPrivate->Cast<UFortRegisteredPlayerInfo>();
		if (!PlayerInfo) {
			Log("UFortQuestManager::GetPlayerControllerBP: PlayerInfo is null!");
			return nullptr;
		}

		TArray<AActor*> PlayerControllers;
		UGameplayStatics::GetAllActorsOfClass(World, AFortPlayerController::StaticClass(), &PlayerControllers);

		for (AActor* Actor : PlayerControllers) {
			AFortPlayerController* PC = Actor->Cast<AFortPlayerController>();
			if (!PC)
				continue;
			
			if (PC->GetRegisteredPlayerInfo() != PlayerInfo)
				continue;

			return PC;
		}
	}
	else {
		// sdk
	}

	return nullptr;
}

void UFortQuestManager::ProgressQuest(UFortQuestItem* QuestItem, FName ObjectiveBackendName, int32 InCount) {
	if (QuestItem->HasCompletedQuest())
		return;

	AFortPlayerController* PlayerController = GetPlayerControllerBP();
	if (!PlayerController) {
		Log("UFortQuestManager::ProgressQuest: PlayerController is null!");
		return;
	}

	UFortQuestItemDefinition* QuestDefinition = QuestItem->ItemDefinition->Cast<UFortQuestItemDefinition>();
	if (!QuestDefinition) {
		Log("UFortQuestManager::ProgressQuest: QuestDefinition is null!");
		return;
	}

	UFortQuestObjectiveInfo* ObjectiveInfo = nullptr;
	for (UFortQuestObjectiveInfo* Info : QuestItem->Objectives) {
		if (Info->BackendName == ObjectiveBackendName) {
			ObjectiveInfo = Info;
			break;
		}
	}

	if (!ObjectiveInfo) {
		Log("UFortQuestManager::ProgressQuest: ObjectiveInfo not found for backend name: " + ObjectiveBackendName.ToString().ToString());
		return;
	}

	int32 AchievedCount = ObjectiveInfo->AchievedCount;
	int32 NewCount = AchievedCount + InCount;
	bool bCompletedObjective = NewCount >= ObjectiveInfo->RequiredCount;
	bool bCompletedAllObjectives = bCompletedObjective;
	if (bCompletedAllObjectives) {
		for (UFortQuestObjectiveInfo* Info : QuestItem->Objectives) {
			if (Info->BackendName == ObjectiveBackendName)
				continue;

			if (Info->AchievedCount < Info->RequiredCount) {
				bCompletedAllObjectives = false;
				break;
			}
		}
	}

	ObjectiveInfo->AchievedCount = bCompletedObjective ? ObjectiveInfo->RequiredCount : NewCount;

	Log("UFortQuestManager::ProgressQuest: Quest: " + QuestItem->GetName().ToString() + " Objective: " + ObjectiveInfo->GetName().ToString() + " InCount: " + std::to_string(InCount) + " AchievedCount: " + std::to_string(ObjectiveInfo->AchievedCount));

	FFortQuestObjectiveCompletion NewCompletion{};
	NewCompletion.StatName = QuestItem->TemplateId; // idk if this is right tbh, i dont think it is
	NewCompletion.Count = InCount;

	PendingChanges.Add(NewCompletion, FFortQuestObjectiveCompletion::GetSize());

	// idrk exactly how these work so if somebody could figure this out that would be fire
	if (PlayerController && PlayerController->AthenaProfile) {
		TArray<FFortQuestObjectiveCompletion> Advanced;

		Advanced.Add(NewCompletion, FFortQuestObjectiveCompletion::GetSize());

		FDedicatedServerUrlContext Context;
		PlayerController->AthenaProfile->UpdateQuests(Advanced, &Context);
	}
}

void UFortQuestManager::ForceTriggerQuestsUpdated()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ForceTriggerQuestsUpdated");

	if (!Func) {
		Log("UFortQuestManager::ForceTriggerQuestsUpdated: Function not found!");
		return;
	}

	Call(Func);
}

void UFortQuestManager::GetSourceAndContextTags(FGameplayTagContainer* OutSourceTags, FGameplayTagContainer* OutContextTags) const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetSourceAndContextTags");

	return const_cast<UFortQuestManager*>(this)->Call<void>(Func, OutSourceTags, OutContextTags);
}

UFortQuestItem* UFortQuestManager::GetQuestWithDefinition(UFortQuestItemDefinition* Definition) {
	for (UFortQuestItem* Quest : CurrentQuests) {
		if (Quest && Quest->ItemDefinition == Definition)
			return Quest;
	}

	return nullptr;
}
