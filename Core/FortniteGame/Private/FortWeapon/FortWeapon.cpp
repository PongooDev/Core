#include "pch.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"

#include "FortniteGame/Public/FortItem/FortItemEntry.h"

void AFortWeap_BuildingTool::OnRep_DefaultMetadata(UBuildingEditModeMetadata* OldMetadata)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_DefaultMetadata");

	return Call(Func, OldMetadata);
}

void AFortWeap_EditingTool::OnRep_EditActor()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_EditActor");

	Call(Func);
}