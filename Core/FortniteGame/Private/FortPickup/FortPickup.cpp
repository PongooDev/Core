#include "pch.h"
#include "FortniteGame/Public/FortPickup/FortPickup.h"

#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortItem/FortItemEntry.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"

void AFortPickup::OnRep_PrimaryPickupItemEntry()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_PrimaryPickupItemEntry"));

	if (!Func) {
		Log("AFortPickup::OnRep_PrimaryPickupItemEntry: Failed to find function!");
		return;
	}

	ProcessEvent(Func, nullptr);
}

void AFortPickup::OnRep_TossedFromContainer()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_TossedFromContainer"));

	ProcessEvent(Func, nullptr);
}

void AFortPickup::TossPickup(const struct FVector& FinalLocation, class AFortPawn* ItemOwner, int32 OverrideMaxStackCount, bool bToss, bool bShouldCombinePickupsWhenTossCompletes, const uint8 InPickupSourceTypeFlags, const uint8 InPickupSpawnSource)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"TossPickup"));

	struct FortPickup_TossPickup final
	{
	public:
		FVector FinalLocation;
		AFortPawn* ItemOwner;
		int32 OverrideMaxStackCount;
		bool bToss;
		bool bShouldCombinePickupsWhenTossCompletes;
		uint8 InPickupSourceTypeFlags;
		uint8 InPickupSpawnSource;
	};

	FortPickup_TossPickup Parms{};

	Parms.FinalLocation = FinalLocation;
	Parms.ItemOwner = ItemOwner;
	Parms.OverrideMaxStackCount = OverrideMaxStackCount;
	Parms.bToss = bToss;
	Parms.bShouldCombinePickupsWhenTossCompletes = bShouldCombinePickupsWhenTossCompletes;
	Parms.InPickupSourceTypeFlags = InPickupSourceTypeFlags;
	Parms.InPickupSpawnSource = InPickupSpawnSource;

	ProcessEvent(Func, &Parms);
}

void AFortPickup::OnRep_bPickedUp()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_bPickedUp"));

	ProcessEvent(Func, nullptr);
}

void AFortPickup::OnRep_PickupLocationData()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_PickupLocationData"));

	ProcessEvent(Func, nullptr);
}

void AFortPickup::ForceFinishedTargetSpline()
{
	void (*ForceFinishedTargetSplineInternal)(AFortPickup * This) = decltype(ForceFinishedTargetSplineInternal)(ImageBase + Finder::FindAFortPickup_ForceFinishedTargetSpline());
	ForceFinishedTargetSplineInternal(this);
}

void AFortPickup::SetPickupTarget(AFortPawn* PickupTarget, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound)
{
	void (*SetPickupTargetInternal)(AFortPickup * This, AFortPawn * PickupTarget, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound) = decltype(SetPickupTargetInternal)(ImageBase + Finder::FindAFortPickup_SetPickupTarget());
	SetPickupTargetInternal(this, PickupTarget, InFlyTime, InStartDirection, bPlayPickupSound);
}

void AFortPickup::UpdateSpecialActorStat(ESpecialActorStatType InStat, double InValue, float InLogicCondition)
{
	void (*UpdateSpecialActorStatInternal)(AFortPickup * This, ESpecialActorStatType InStat, double InValue, float InLogicCondition) = decltype(UpdateSpecialActorStatInternal)(ImageBase + Finder::FindAFortPickup_UpdateSpecialActorStat());
	UpdateSpecialActorStatInternal(this, InStat, InValue, InLogicCondition);
}

void AFortPickup::SetupForMovementCompToss()
{
	void (*SetupForMovementCompTossInternal)(AFortPickup * This) = decltype(SetupForMovementCompTossInternal)(ImageBase + Finder::FindAFortPickup_SetupForMovementCompToss());
	SetupForMovementCompTossInternal(this);
}

void AFortPickup::SetPickupItems(FFortItemEntry* PrimaryEntry, TArray<FFortItemEntry>* AdditionalEntries, uint8 SourceTypeFlags, bool bInSplitOnPickup, uint8 SpawnSource)
{
	if (Version::Fortnite_Version >= 16) {
		void (*&SetPickupItemsInternal)(AFortPickup * This, FFortItemEntry * PrimaryEntry, TArray<FFortItemEntry>*AdditionalEntries, uint8 SourceTypeFlags, bool bInSplitOnPickup, uint8 SpawnSource) = decltype(SetPickupItemsInternal)(VTable[Finder::FindAFortPickup_SetPickupItemsVFT()]);
		SetPickupItemsInternal(this, PrimaryEntry, AdditionalEntries, SourceTypeFlags, bInSplitOnPickup, SpawnSource);
	}
	else {
		void (*&SetPickupItemsInternal)(AFortPickup * This, FFortItemEntry * PrimaryEntry, TArray<FFortItemEntry>*AdditionalEntries, bool bInSplitOnPickup) = decltype(SetPickupItemsInternal)(VTable[Finder::FindAFortPickup_SetPickupItemsVFT()]);
		SetPickupItemsInternal(this, PrimaryEntry, AdditionalEntries, bInSplitOnPickup);
	}
}

void AFortPickup::GivePickupTo(AFortPickup* This, IFortInventoryOwnerInterface* InventoryOwner, bool DestroyAfterPickup) {
	GivePickupToOG(This, InventoryOwner, DestroyAfterPickup);

	if (This->PickupLocationData.PickupTarget || This->PickupLocationData.ItemOwner) {
		AFortPawn* PickupTargetPawn = This->PickupLocationData.PickupTarget ? This->PickupLocationData.PickupTarget : This->PickupLocationData.ItemOwner;
		if (!PickupTargetPawn->Controller) {
			Log("AFortPickup::GivePickupTo: PickupTargetPawn->Controller is null!");
			return;
		}

		AFortPlayerController* PickupTargetController = PickupTargetPawn->Controller->Cast<AFortPlayerController>();
		if (!PickupTargetController) {
			Log("AFortPickup::GivePickupTo: PickupTargetPawn->Controller is not a FortPlayerController!");
			return;
		}

		PickupTargetController->WorldInventory->AddItemAndHandleOverflow(This->PrimaryPickupItemEntry);
	}
	else {
		Log("AFortPickup::GivePickupTo: No valid inventory owner found for pickup!");
	}
}