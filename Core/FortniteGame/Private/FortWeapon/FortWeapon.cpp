#include "pch.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"

#include "FortniteGame/Public/FortItem/FortItemEntry.h"

void AFortWeap_BuildingTool::OnRep_DefaultMetadata(UBuildingEditModeMetadata* OldMetadata)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_DefaultMetadata");

	struct FortWeap_BuildingTool_OnRep_DefaultMetadata final
	{
	public:
		UBuildingEditModeMetadata* OldMetadata;
	};

	FortWeap_BuildingTool_OnRep_DefaultMetadata Parms{};

	Parms.OldMetadata = OldMetadata;

	ProcessEvent(Func, &Parms);
}

void AFortWeap_EditingTool::OnRep_EditActor()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_EditActor");

	ProcessEvent(Func, nullptr);
}