#include "pch.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"
#include "FortniteGame/Public/FortAbility/FortGameplayAbility.h"

class UFortResourceItemDefinition* UFortKismetLibrary::K2_GetResourceItemDefinition(const uint8 ResourceType)
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/FortniteGame.FortKismetLibrary.K2_GetResourceItemDefinition");

	if (!Func) {
		return nullptr;
	}

	struct FortKismetLibrary_K2_GetResourceItemDefinition final
	{
	public:
		uint8 ResourceType;
		UFortResourceItemDefinition* ReturnValue;
	};

	FortKismetLibrary_K2_GetResourceItemDefinition Parms{};

	Parms.ResourceType = ResourceType;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

EFortStructuralGridQueryResults UFortKismetLibrary::CanPlaceBuildableClassInStructuralGrid(
	UObject* WorldContextObject,
	UClass* ClassToBuild,
	FVector WorldLocation,
	FRotator WorldRotation,
	bool bMirrored,
	TArray<ABuildingActor*>* ExistingBuildings,
	EFortBuildPreviewMarkerOptionalAdjustment* MarkerOptionalAdjustment)
{
	EFortStructuralGridQueryResults(*CanPlaceBuildableClassInStructuralGridInternal)(UObject*, UClass*, FVector, FRotator, bool, TArray<ABuildingActor*>*, EFortBuildPreviewMarkerOptionalAdjustment*) = decltype(CanPlaceBuildableClassInStructuralGridInternal)(ImageBase + Finder::FindUFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid());
	return CanPlaceBuildableClassInStructuralGridInternal(WorldContextObject, ClassToBuild, WorldLocation, WorldRotation, bMirrored, ExistingBuildings, MarkerOptionalAdjustment);
}

EFortStructuralGridQueryResults UFortKismetLibrary::CanPlaceBuildableClassInStructuralGrid(
	UObject* WorldContextObject,
	TSubclassOf<ABuildingSMActor> ClassToBuild,
	FVector WorldLocation,
	FRotator WorldRotation,
	bool bMirrored,
	TArray<ABuildingActor*>* ExistingBuildings,
	EFortBuildPreviewMarkerOptionalAdjustment* MarkerOptionalAdjustment)
{
	EFortStructuralGridQueryResults(*CanPlaceBuildableClassInStructuralGridInternal)(UObject*, TSubclassOf<ABuildingSMActor>, FVector, FRotator, bool, TArray<ABuildingActor*>*, EFortBuildPreviewMarkerOptionalAdjustment*) = decltype(CanPlaceBuildableClassInStructuralGridInternal)(ImageBase + Finder::FindUFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid());
	return CanPlaceBuildableClassInStructuralGridInternal(WorldContextObject, ClassToBuild, WorldLocation, WorldRotation, bMirrored, ExistingBuildings, MarkerOptionalAdjustment);
}

FFortAbilitySetHandle UFortKismetLibrary::EquipFortAbilitySet(TScriptInterface<IAbilitySystemInterface> AbilitySystemInterfaceActor, UFortAbilitySet* AbilitySet, UObject* OverrideSourceObject)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/FortniteGame.FortKismetLibrary.EquipFortAbilitySet");

	if (!Func) {
		Log("UFortKismetLibrary::EquipFortAbilitySet: Failed to find function!");
		return FFortAbilitySetHandle();
	}

	struct FortKismetLibrary_EquipFortAbilitySet
	{
	public:
		TScriptInterface<IAbilitySystemInterface> AbilitySystemInterfaceActor;
		UFortAbilitySet* AbilitySet;
		UObject* OverrideSourceObject;
		FFortAbilitySetHandle ReturnValue;
	};

	FortKismetLibrary_EquipFortAbilitySet Parms{};

	Parms.AbilitySystemInterfaceActor = AbilitySystemInterfaceActor;
	Parms.AbilitySet = AbilitySet;
	Parms.OverrideSourceObject = OverrideSourceObject;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}