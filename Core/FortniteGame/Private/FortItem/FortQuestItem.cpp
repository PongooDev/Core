#include "pch.h"
#include "FortniteGame/Public/FortItem/FortQuestItem.h"

bool UFortQuestItem::HasCompletedQuest() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HasCompletedQuest");

	struct FortQuestItem_HasCompletedQuest
	{
	public:
		bool ReturnValue;
	};

	FortQuestItem_HasCompletedQuest Parms{};

	const_cast<UFortQuestItem*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

bool UFortQuestItem::HasCompletedObjectiveWithName(class FName BackendName) const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HasCompletedObjectiveWithName");

	struct FortQuestItem_HasCompletedObjectiveWithName
	{
	public:
		FName BackendName;
		bool ReturnValue;
	};

	FortQuestItem_HasCompletedObjectiveWithName Parms{};

	Parms.BackendName = BackendName;

	const_cast<UFortQuestItem*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

UFortQuestObjectiveInfo* UFortQuestItem::GetObjectiveInfo(const FDataTableRowHandle& ObjectiveStatHandle) const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetObjectiveInfo");

	struct FortQuestItem_GetObjectiveInfo
	{
	public:
		FDataTableRowHandle ObjectiveStatHandle;
		UFortQuestObjectiveInfo* ReturnValue;
	};

	FortQuestItem_GetObjectiveInfo Parms{};

	Parms.ObjectiveStatHandle = std::move(ObjectiveStatHandle);

	const_cast<UFortQuestItem*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}