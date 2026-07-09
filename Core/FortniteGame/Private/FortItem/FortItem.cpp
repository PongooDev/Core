#include "pch.h"
#include "FortniteGame/Public/FortItem/FortItem.h"

void UFortItem::SetOwningControllerForTemporaryItem(AFortPlayerController* InController)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"SetOwningControllerForTemporaryItem"));

	return const_cast<UFortItem*>(this)->Call<void>(Func, InController);
}

FGuid UFortItem::GetItemGuid() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetItemGuid");

	return const_cast<UFortItem*>(this)->Call<FGuid>(Func);
}

AFortPlayerController* UFortItem::GetOwningController() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetOwningController");

	return const_cast<UFortItem*>(this)->Call<AFortPlayerController*>(Func);
}