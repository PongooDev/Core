#pragma once
#include "pch.h"

#include "FortAccountItem.h"

class UFortQuestObjectiveInfo;

class UFortQuestItem : public UFortAccountItem {
public:
	DefineUnrealClass(UFortQuestItem);

	DefineUProperty(TArray<UFortQuestObjectiveInfo*>, Objectives);
public:
	bool HasCompletedQuest() const;
	bool HasCompletedObjectiveWithName(FName BackendName) const;
};