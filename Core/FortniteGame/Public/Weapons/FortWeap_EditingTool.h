#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "FortniteGame/Public/Weapons/FortWeap_BuildingToolBase.h"

class UBuildingEditModeMetadata;
class UFortWeaponItemDefinition;
class ABuildingSMActor;


class AFortWeap_EditingTool : public AFortWeap_BuildingToolBase {
public:
	DefineUnrealClass(AFortWeap_EditingTool);

	DefineUProperty(ABuildingSMActor*, EditActor);
	DefineUProperty(bool, bEditConfirmed);
public:
	void OnRep_EditActor();
};
