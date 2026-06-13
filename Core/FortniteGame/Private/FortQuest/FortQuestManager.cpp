#include "pch.h"
#include "FortniteGame/Public/FortQuest/FortQuestManager.h"

#include "FortniteGame/Public/FortItem/FortQuestItem.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"
#include "FortniteGame/Public/FortQuest/FortQuestObjectiveStatTableRow.h"
#include "FortniteGame/Public/FortQuest/FortQuestObjectiveInfo.h"
#include "FortniteGame/Public/FortQuest/FortQuestObjectiveCompletion.h"
#include "FortniteGame/Public/Info/FortRegisteredPlayerInfo.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAthena.h"
#include "FortniteGame/Public/Mcp/McpProfileSys.h"

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
				for (int32 j = 0; j < ObjectiveStatHandle.DataTable->RowMap.Num(); j++) {
					auto& Row = ObjectiveStatHandle.DataTable->RowMap[j];

					FName& Key = Row.Key();
					if (Key != ObjectiveStatHandle.RowName) {
						continue;
					}

					FFortQuestObjectiveStatTableRow* Value = (FFortQuestObjectiveStatTableRow*)Row.Value();

					if (!SourceTags.HasAll(Value->SourceTagContainer)) {
						continue;
					}

					if (!ContextTags.HasAll(Value->ContextTagContainer)) {
						continue;
					}

					This->ProgressQuest(QuestItem, Objective.BackendName, Count);
				}
			}
		}
	}

	return SendCustomStatEventOG(This, ObjectiveStat, Count, bForceFlush);
}

void UFortQuestManager::SendStatEvent(
	UFortQuestManager* This,
	FDataTableRowHandle* InObjectiveStat,
	uint8 InType,
	UObject* InTargetObject,
	FGameplayTagContainer* InTargetTags,
	FGameplayTagContainer* InSourceTags,
	FGameplayTagContainer* InContextTags,
	int32 InCount,
	bool bForceFlush
) {
	/*Log("FortQuestManager: " + This->GetName().ToString());
	Log("InObjectiveStat: " + std::to_string((uintptr_t)InObjectiveStat));
	Log("InType: " + std::to_string(InType));
	Log("InTargetObject: " + (InTargetObject ? InTargetObject->GetName().ToString() : "nullptr"));
	for (int32 i = 0; i < InTargetTags->Num(); i++) {
		Log("InTargetTags[" + std::to_string(i) + "]: " + InTargetTags->GetByIndex(i).ToString().ToString());
	}
	for (int32 i = 0; i < InSourceTags->Num(); i++) {
		Log("InSourceTags[" + std::to_string(i) + "]: " + InSourceTags->GetByIndex(i).ToString().ToString());
	}
	for (int32 i = 0; i < InContextTags->Num(); i++) {
		Log("InContextTags[" + std::to_string(i) + "]: " + InContextTags->GetByIndex(i).ToString().ToString());
	}
	Log("InCount: " + std::to_string(InCount));
	Log("bForceFlush: " + std::string(bForceFlush ? "true" : "false"));*/

	AFortPlayerController* PlayerController = This->GetPlayerControllerBP();
	if (!PlayerController) {
		Log("UFortQuestManager::SendStatEvent: PlayerController is null!");
		return SendStatEventOG(This, InObjectiveStat, InType, InTargetObject, InTargetTags, InSourceTags, InContextTags, InCount, bForceFlush);
	}

	for (UFortQuestItem* QuestItem : This->CurrentQuests) {
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
				for (int32 j = 0; j < ObjectiveStatHandle.DataTable->RowMap.Num(); j++) {
					auto& Row = ObjectiveStatHandle.DataTable->RowMap[j];

					FName& Key = Row.Key();

					if (Key != ObjectiveStatHandle.RowName) {
						continue;
					}

					FFortQuestObjectiveStatTableRow* Value = (FFortQuestObjectiveStatTableRow*)Row.Value();

					if (Value->Type != InType) {
						continue;
					}

					if (!InTargetTags->HasAll(Value->TargetTagContainer)) {
						continue;
					}

					if (!InSourceTags->HasAll(Value->SourceTagContainer)) {
						continue;
					}

					/*if (!InContextTags->HasAll(Value->ContextTagContainer)) {
						continue;
					}*/

					This->ProgressQuest(QuestItem, Objective.BackendName, InCount);
				}
			}
		}
	}

	return SendStatEventOG(This, InObjectiveStat, InType, InTargetObject, InTargetTags, InSourceTags, InContextTags, InCount, bForceFlush);
}

FScriptContainerElement* UFortQuestManager::ProcessPendingStatEvents() {
	FScriptContainerElement* (*ProcessPendingStatEventsInternal)(UFortQuestManager*) = decltype(ProcessPendingStatEventsInternal)(ImageBase + Finder::FindUFortQuestManager_ProcessPendingStatEvents());
	return ProcessPendingStatEventsInternal(this);
}

AFortPlayerController* UFortQuestManager::GetPlayerControllerBP() {
	if (Version::Fortnite_Version <= 1.91) {
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

	if (bCompletedObjective) {
		ObjectiveInfo->AchievedCount = ObjectiveInfo->RequiredCount;
	}
	else {
		ObjectiveInfo->AchievedCount = NewCount;
	}

	Log("UFortQuestManager::ProgressQuest: Quest: " + QuestItem->GetName().ToString() + " Objective: " + ObjectiveInfo->GetName().ToString() + " InCount: " + std::to_string(InCount) + " AchievedCount: " + std::to_string(ObjectiveInfo->AchievedCount));

	FFortQuestObjectiveCompletion NewCompletion{};
	NewCompletion.StatName = QuestItem->TemplateId; // idk if this is right tbh, i dont think it is
	NewCompletion.Count = InCount;

	PendingChanges.Add(NewCompletion);

	// idrk exactly how these work so if somebody could figure this out that would be fire
	if (PlayerController && PlayerController->AthenaProfile) {
		TArray<FFortQuestObjectiveCompletion> Advanced;

		Advanced.Add(NewCompletion);

		FDedicatedServerUrlContext Context;
		PlayerController->AthenaProfile->UpdateQuests(Advanced, &Context);
	}

	ForceTriggerQuestsUpdated();
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

	ProcessEvent(Func, nullptr);
}

void UFortQuestManager::GetSourceAndContextTags(FGameplayTagContainer* OutSourceTags, FGameplayTagContainer* OutContextTags) const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetSourceAndContextTags");

	struct FortQuestManager_GetSourceAndContextTags
	{
	public:
		FGameplayTagContainer OutSourceTags;
		FGameplayTagContainer OutContextTags;
	};

	FortQuestManager_GetSourceAndContextTags Parms{};

	const_cast<UFortQuestManager*>(this)->ProcessEvent(Func, &Parms);

	if (OutSourceTags != nullptr)
		*OutSourceTags = std::move(Parms.OutSourceTags);

	if (OutContextTags != nullptr)
		*OutContextTags = std::move(Parms.OutContextTags);
}