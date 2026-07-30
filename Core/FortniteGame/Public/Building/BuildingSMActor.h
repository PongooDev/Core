#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/GameplayEffectTypes.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/HitResult.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"

#include "FortniteGame/Public/Building/BuildingActor.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class ABuildingActor;
class ABuildingSMActor;
class AFortPlayerController;
class AFortPlayerStateZone;
class UFortWorldItemDefinition;

class EFortStructuralGridEditMode {
public:
	DefineUnrealEnum(EFortStructuralGridEditMode);

	DefineEnumProperty(CanAdd);
	DefineEnumProperty(ExistingActor);
	DefineEnumProperty(NoStructuralSupport);
	DefineEnumProperty(InvalidActor);
	DefineEnumProperty(ReachedLimit);
	DefineEnumProperty(NoEditPermission);
	DefineEnumProperty(PatternNotPermittedByLayoutRequirement);
	DefineEnumProperty(ResourceTypeNotPermittedByLayoutRequirement);
	DefineEnumProperty(BuildingAtRequirementsDisabled);
	DefineEnumProperty(BuildingOtherThanRequirementsDisabled);
};

class EFortBuildPreviewMarkerOptionalAdjustment {
public:
	DefineUnrealEnum(EFortBuildPreviewMarkerOptionalAdjustment);

	DefineEnumProperty(None);
	DefineEnumProperty(FreeWallPieceOnTop);
	DefineEnumProperty(FreeWallPieceOnBottom);
};

class EBuildingReplacementType {
public:
	DefineUnrealEnum(EBuildingReplacementType);

	DefineEnumProperty(BRT_None);
	DefineEnumProperty(BRT_Edited);
	DefineEnumProperty(BRT_Conversion);
};

class ABuildingSMActor : public ABuildingActor {
public:
	DefineUnrealClass(ABuildingSMActor);

	DefineUProperty(uint8, ResourceType);

	DefineUProperty(AFortPlayerStateZone*, EditingPlayer);

	DefineBitfieldUProperty(bMirrored);

	DefineUProperty(TArray<ABuildingSMActor*>, AttachedBuildingActors);

	DefineBitfieldUProperty(bAllowResourceDrop);

	DefineUProperty(FCurveTableRowHandle, BuildingResourceAmountOverride);
public:
	ABuildingSMActor* ReplaceBuildingActor(
		uint8 ReplacementType,
		UClass* ReplacementClass,
		int ReplacementUpgradeLevel,
		int RotationIterations,
		bool InbMirrored,
		AFortPlayerController* EditingController);

	void RepairBuilding(AFortPlayerController* RepairingController, int32 ResourcesSpent);

	void SetEditingPlayer(AFortPlayerStateZone* NewEditingPlayer);

	bool HasBuildingResourceAmountOverride() {
		UProperty* Property = FindPropertyByName("BuildingResourceAmountOverride");
		return Property;
	}

	static void Hook() {
		Log("ABuildingSMActor Hooked!");
	}
};
