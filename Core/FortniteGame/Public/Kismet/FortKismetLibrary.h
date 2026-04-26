#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"

#include "FortniteGame/Public/FortEnums.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySet.h"

class ABuildingActor;
class ABuildingSMActor;
class IFortInventoryOwnerInterface;
class UFortWorldItem;
class AFortAIGoalManager;
class AFortPlayerPawn;
class AFortAIDirector;
class AFortPickup;
class AFortPawn;
class UFortResourceItemDefinition;
class IAbilitySystemInterface;

class UFortKismetLibrary : public UBlueprintFunctionLibrary {
public:
	DefineUnrealClass(UFortKismetLibrary);

	static UFortResourceItemDefinition* K2_GetResourceItemDefinition(const uint8 ResourceType);

	static EFortStructuralGridQueryResults CanPlaceBuildableClassInStructuralGrid(
		UObject* WorldContextObject,
		UClass* ClassToBuild,
		FVector WorldLocation,
		FRotator WorldRotation,
		bool bMirrored,
		TArray<ABuildingActor*>* ExistingBuildings,
		EFortBuildPreviewMarkerOptionalAdjustment* MarkerOptionalAdjustment);

	static EFortStructuralGridQueryResults CanPlaceBuildableClassInStructuralGrid(
		UObject* WorldContextObject,
		TSubclassOf<ABuildingSMActor> ClassToBuild,
		FVector WorldLocation,
		FRotator WorldRotation,
		bool bMirrored,
		TArray<ABuildingActor*>* ExistingBuildings,
		EFortBuildPreviewMarkerOptionalAdjustment* MarkerOptionalAdjustment);
	
	static FFortAbilitySetHandle EquipFortAbilitySet(TScriptInterface<IAbilitySystemInterface> AbilitySystemInterfaceActor, UFortAbilitySet* AbilitySet, UObject* OverrideSourceObject);
};