#include "pch.h"
#include "FortniteGame/Public/FortInventory/FortQuickBars.h"

#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortItem/FortWorldItem.h"

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