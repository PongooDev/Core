#include "pch.h"
#include "FortniteGame/Public/FortInventory/FortQuickBars.h"

#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortItem/FortWorldItem.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"

void AFortQuickBars::ServerAddItemInternal(const FGuid& Item, uint8 InQuickBar, int32 Slot)
{
	if (!this) return;

	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerAddItemInternal");

	if (!Func) {
		Log("AFortQuickBars::ServerAddItemInternal: Failed to find function!");
		return;
	}

	struct FortQuickBars_ServerAddItemInternal
	{
	public:
		FGuid Item;
		uint8 InQuickBar;
		int32 Slot;
	};

	FortQuickBars_ServerAddItemInternal Parms{};

	Parms.Item = std::move(Item);
	Parms.InQuickBar = InQuickBar;
	Parms.Slot = Slot;

	ProcessEvent(Func, &Parms);
}

void AFortQuickBars::ServerRemoveItemInternal(const FGuid& Item, bool bFindReplacement, bool bForce)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerRemoveItemInternal");

	struct FortQuickBars_ServerRemoveItemInternal final
	{
	public:
		FGuid Item;
		bool bFindReplacement;
		bool bForce;
	};

	FortQuickBars_ServerRemoveItemInternal Parms{};

	Parms.Item = std::move(Item);
	Parms.bFindReplacement = bFindReplacement;
	Parms.bForce = bForce;

	ProcessEvent(Func, &Parms);
}

void AFortQuickBars::EmptySlot(uint8 InQuickBar, int32 SlotIndex)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("EmptySlot");

	struct FortQuickBars_EmptySlot final
	{
	public:
		uint8 InQuickBar;
		uint8 Pad_1[0x3];
		int32 SlotIndex;
	};

	FortQuickBars_EmptySlot Parms{};

	Parms.InQuickBar = InQuickBar;
	Parms.SlotIndex = SlotIndex;

	ProcessEvent(Func, &Parms);
}

void AFortQuickBars::ServerActivateSlotInternal(uint8 InQuickBar, int32 Slot, float ActivateDelay, bool bUpdatePreviousFocusedSlot)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerActivateSlotInternal");

	struct FortQuickBars_ServerActivateSlotInternal final
	{
	public:
		uint8 InQuickBar;
		uint8 Pad_1[0x3];
		int32 Slot;
		float ActivateDelay;
		bool bUpdatePreviousFocusedSlot;
		uint8 Pad_D[0x3];
	};

	FortQuickBars_ServerActivateSlotInternal Parms{};

	Parms.InQuickBar = InQuickBar;
	Parms.Slot = Slot;
	Parms.ActivateDelay = ActivateDelay;
	Parms.bUpdatePreviousFocusedSlot = bUpdatePreviousFocusedSlot;

	ProcessEvent(Func, &Parms);
}

void AFortQuickBars::EnableSlot(uint8 InQuickBar, int32 SlotIndex)
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("EnableSlot");

	struct FortQuickBars_EnableSlot final
	{
	public:
		uint8 InQuickBar;
		uint8 Pad_1[0x3];
		int32 SlotIndex;
	};

	FortQuickBars_EnableSlot Parms{};

	Parms.InQuickBar = InQuickBar;
	Parms.SlotIndex = SlotIndex;

	ProcessEvent(Func, &Parms);
}

int32 AFortQuickBars::FindQuickBarSlotForItem(uint8 QuickBar, FGuid Guid) const
{
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

void AFortQuickBars::EmptyQuickbarSlot(FGuid Guid)
{
	if (!Guid.IsValid())
		return;

	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return;

	if (PC->WorldInventory->IsCurrentItem(Guid))
	{
		PC->WorldInventory->EquipHarvestingTool();
	}

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

	ServerRemoveItemInternal(Guid, false, true);
}

AFortPlayerController* AFortQuickBars::GetOwnerPlayerController() const
{
	return Owner ? Owner->Cast<AFortPlayerController>() : nullptr;
}

void AFortQuickBars::EquipHarvestingTool() {
	ServerActivateSlotInternal(EFortQuickBars::GetPrimary(), 0, 0.f, true);
}

void AFortQuickBars::AddItemToQuickBar(FGuid Guid, uint8 QuickBar)
{
	if (!Guid.IsValid())
		return;

	ServerAddItemInternal(Guid, QuickBar, -1);
}

void AFortQuickBars::OnRep_PrimaryQuickBar()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_PrimaryQuickBar");

	ProcessEvent(Func, nullptr);
}

void AFortQuickBars::OnRep_SecondaryQuickBar()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_SecondaryQuickBar");

	ProcessEvent(Func, nullptr);
}

bool AFortQuickBars::Update() {
	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return false;

	if (PC->IsUsingOldQuickBars())
	{
		OnRep_PrimaryQuickBar();
		OnRep_SecondaryQuickBar();
		PC->OnRep_QuickBar();
	}
	else
	{
		PC->ClientForceUpdateQuickbar(EFortQuickBars::GetPrimary());
		PC->ClientForceUpdateQuickbar(EFortQuickBars::GetSecondary());
	}

	ForceNetUpdate();

	return true;
}