#include "pch.h"
#include "FortniteGame/Public/FortQuest/FortQuestManager.h"

void UFortQuestManager::SendCustomStatEvent(UFortQuestManager* This, FDataTableRowHandle& ObjectiveStat, int32 Count, bool bForceFlush) {
	return SendCustomStatEventOG(This, ObjectiveStat, Count, bForceFlush);
}

void UFortQuestManager::SendStatEvent(
	UFortQuestManager* This,
	FScriptContainerElement* InObjectiveStat,
	uint8 InType,
	UObject* InTargetObject,
	FGameplayTagContainer* InTargetTags,
	FGameplayTagContainer* InSourceTags,
	FGameplayTagContainer* InContextTags,
	int32 InCount,
	bool bForceFlush
) {
	return SendStatEventOG(This, InObjectiveStat, InType, InTargetObject, InTargetTags, InSourceTags, InContextTags, InCount, bForceFlush);
}