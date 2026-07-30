#include "pch.h"
#include "FortniteGame/Public/Inventory/FortQuickBars.h"

#include "FortniteGame/Public/Weapons/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/Items/FortWorldItem.h"
#include "FortniteGame/Public/Player/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/Inventory/FortInventory.h"
#include "FortniteGame/Public/Inventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortGameModeAthena.h"
#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"
#include "FortniteGame/Public/FortEnums.h"

void AFortQuickBars::ServerAddItemInternal(const FGuid& Item, uint8 InQuickBar, int32 Slot)
{
	if (!this) return;

	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerAddItemInternal");

	if (!Func) {
		return;
	}

	return Call(Func, Item, InQuickBar, Slot);
}

void AFortQuickBars::ServerRemoveItemInternal(const FGuid& Item, bool bFindReplacement, bool bForce)
{
	if (!this) return;
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerRemoveItemInternal");

	if (!Func) {
		return;
	}

	return Call(Func, Item, bFindReplacement, bForce);
}

void AFortQuickBars::EmptySlot(uint8 InQuickBar, int32 SlotIndex)
{
	if (!this) return;
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("EmptySlot");

	if (!Func) {
		return;
	}

	return Call(Func, InQuickBar, SlotIndex);
}

void AFortQuickBars::ServerActivateSlotInternal(uint8 InQuickBar, int32 Slot, float ActivateDelay, bool bUpdatePreviousFocusedSlot)
{
	if (!this) return;
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerActivateSlotInternal");

	if (!Func) {
		return;
	}

	return Call(Func, InQuickBar, Slot, ActivateDelay, bUpdatePreviousFocusedSlot);
}

void AFortQuickBars::EnableSlot(uint8 InQuickBar, int32 SlotIndex)
{
	if (!this) return;
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("EnableSlot");

	if (!Func) {
		return;
	}

	return Call(Func, InQuickBar, SlotIndex);
}

int32 AFortQuickBars::FindQuickBarSlotForItem(uint8 QuickBar, FGuid Guid) const
{
	if (!this) return -1;
	if (!Guid.IsValid())
		return -1;

	if (QuickBar == EFortQuickBars::GetPrimary()) {
		for (int i = 0; i < PrimaryQuickBar.Slots.Num(); i++)
		{
			const FQuickBarSlot& Slot = PrimaryQuickBar.Slots.GetWithSize(i, FQuickBarSlot::GetSize());
			for (const FGuid& ItemGuid : Slot.Items)
			{
				if (ItemGuid == Guid)
					return i;
			}
		}
	}
	else if (QuickBar == EFortQuickBars::GetSecondary()) {
		for (int i = 0; i < SecondaryQuickBar.Slots.Num(); i++)
		{
			const FQuickBarSlot& Slot = SecondaryQuickBar.Slots.GetWithSize(i, FQuickBarSlot::GetSize());
			for (const FGuid& ItemGuid : Slot.Items)
			{
				if (ItemGuid == Guid)
					return i;
			}
		}
	}
	else {
		Log("AFortQuickBars::FindQuickBarSlotForItem: Unhandled QuickBar value: " + std::to_string(QuickBar));
	}

	return -1;
}

void AFortQuickBars::EmptyQuickbarSlot(FGuid Guid, bool bEquipHarvestingTool, bool bFindReplacement)
{
	if (!this) return;
	if (!Guid.IsValid())
		return;

	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return;

	bool bIsCurrentItem = PC->WorldInventory->IsCurrentItem(Guid);

	ServerRemoveItemInternal(Guid, bFindReplacement, true);

	const int32 PrimarySlotIndex = FindQuickBarSlotForItem(EFortQuickBars::GetPrimary(), Guid);
	if (PrimarySlotIndex != -1)
	{
		EmptySlot(EFortQuickBars::GetPrimary(), PrimarySlotIndex);
	}

	const int32 SecondarySlotIndex = FindQuickBarSlotForItem(EFortQuickBars::GetSecondary(), Guid);
	if (SecondarySlotIndex != -1)
	{
		EmptySlot(EFortQuickBars::GetSecondary(), SecondarySlotIndex);
	}

	if (bIsCurrentItem && bEquipHarvestingTool)
	{
		EquipHarvestingTool();
	}
}

AFortPlayerController* AFortQuickBars::GetOwnerPlayerController() const
{
	if (!this) return nullptr;
	return Owner ? Owner->Cast<AFortPlayerController>() : nullptr;
}

void AFortQuickBars::EquipHarvestingTool() {
	if (!this) return;
	ServerActivateSlotInternal(EFortQuickBars::GetPrimary(), 0, 0.f, true);
}

void AFortQuickBars::AddItemToQuickBar(FGuid Guid, uint8 QuickBar, int32 Slot)
{
	if (!this) return;
	if (!Guid.IsValid())
		return;

	ServerAddItemInternal(Guid, QuickBar, Slot);
}

void AFortQuickBars::Update()
{
	if (!this) return;

	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC) {
		return;
	}

	OnRep_PrimaryQuickBar();
	PC->ClientForceUpdateQuickbar(EFortQuickBars::GetPrimary());

	OnRep_SecondaryQuickBar();
	PC->ClientForceUpdateQuickbar(EFortQuickBars::GetSecondary());

	ForceNetUpdate();
}

void AFortQuickBars::OnRep_PrimaryQuickBar()
{
	if (!this) return;
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_PrimaryQuickBar");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortQuickBars::OnRep_SecondaryQuickBar()
{
	if (!this) return;
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_SecondaryQuickBar");

	if (!Func) {
		return;
	}

	Call(Func);
}

bool AFortQuickBars::EquipItem(FGuid Guid) {
	if (!this) return false;
	if (!Guid.IsValid())
		return false;

	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return false;

	FFortItemEntry* ItemEntry = PC->FindItemEntry(Guid);
	if (!ItemEntry) {
		Log("AFortQuickBars::EquipItem: Failed to find item entry for GUID: " + Guid.FormatGuid());
		return false;
	}

	UFortItemDefinition* ItemDef = ItemEntry->ItemDefinition;
	if (!ItemDef) {
		Log("AFortQuickBars::EquipItem: ItemDefinition is null for GUID: " + Guid.FormatGuid());
		return false;
	}

	uint8 ItemDefQuickBar = ItemDef->GetQuickBarForItem();
	int32 ResolvedSlot = FindQuickBarSlotForItem(ItemDefQuickBar, Guid);
	if (ResolvedSlot == -1) {
		return true;
	}
	ServerActivateSlotInternal(ItemDefQuickBar, ResolvedSlot, 0.f, true);

	return true;
}

UClass* AFortQuickBars::GetDefaultQuickBarsClass()
{
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		return AFortQuickBars::StaticClass();
	}

	AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	if (FortGameModeAthena) {
		if (!AFortQuickBarsAthena::StaticClass()) {
			return AFortQuickBars::StaticClass();
		}

		return AFortQuickBarsAthena::StaticClass();
	}

	return AFortQuickBars::StaticClass();
}
