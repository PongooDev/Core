#include "pch.h"
#include "FortniteGame/Public/FortItem/FortQuestItem.h"

bool UFortQuestItem::HasCompletedQuest() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HasCompletedQuest");

	return const_cast<UFortQuestItem*>(this)->Call<bool>(Func);
}

bool UFortQuestItem::HasCompletedObjectiveWithName(class FName BackendName) const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HasCompletedObjectiveWithName");

	return const_cast<UFortQuestItem*>(this)->Call<bool>(Func, BackendName);
}

UFortQuestObjectiveInfo* UFortQuestItem::GetObjectiveInfo(const FDataTableRowHandle& ObjectiveStatHandle) const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetObjectiveInfo");

	return const_cast<UFortQuestItem*>(this)->Call<UFortQuestObjectiveInfo*>(Func, ObjectiveStatHandle);
}