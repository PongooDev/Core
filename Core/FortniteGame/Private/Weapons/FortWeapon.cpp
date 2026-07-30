#include "pch.h"
#include "FortniteGame/Public/Weapons/FortWeapon.h"

#include "FortniteGame/Public/Items/FortItemEntry.h"
#include "FortniteGame/Public/Weapons/FortWeap_BuildingTool.h"
#include "FortniteGame/Public/Weapons/FortWeap_EditingTool.h"

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
