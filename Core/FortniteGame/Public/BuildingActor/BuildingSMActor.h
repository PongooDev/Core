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

#include "BuildingActor.h"
#include "FortniteGame/Public/FortEnums.h"

class ABuildingActor;
class ABuildingSMActor;
class AFortPlayerController;
class AFortPlayerStateZone;
class UFortWorldItemDefinition;

class ABuildingSMActor : public ABuildingActor {
public:
	DefineUnrealClass(ABuildingSMActor);

	DefineUProperty(uint8, ResourceType);

	DefineUProperty(AFortPlayerStateZone*, EditingPlayer);

	DefineBitfieldUProperty(bMirrored);
public:
	ABuildingSMActor* ReplaceBuildingActor(
		EBuildingReplacementType ReplacementType,
		UClass* ReplacementClass,
		int ReplacementUpgradeLevel,
		int RotationIterations,
		bool InbMirrored,
		AFortPlayerController* EditingController);

	void RepairBuilding(AFortPlayerController* RepairingController, int32 ResourcesSpent);

	void SetEditingPlayer(AFortPlayerStateZone* NewEditingPlayer);

	static void Hook() {
		Log("ABuildingSMActor Hooked!");
	}
};