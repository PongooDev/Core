#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Set.h"

#include "FortniteGame/Public/FortEnums.h"

class UFortItemDefinition;
class UFortItem;
class UFortWorldItem;
class AFortPlayerController;

struct FQuickBarSlot {
public:
	DefineUnrealStruct(FQuickBarSlot);

	TArray<FGuid> Items;
	uint8 bEnabled : 1; // Even tho this is a bool on the older versions, we can still use the bitfield and it will give us the correct value
};

struct FQuickBarSlotData
{
public:
	DefineUnrealStruct(FQuickBarSlotData);

	DefineStructProperty(bool, bStaticSlot);
};

struct FQuickBarData {
public:
	DefineUnrealStruct(FQuickBarData);

	TArray<FQuickBarSlotData> QuickbarSlots;
};

struct FQuickBar {
public:
	DefineUnrealStruct(FQuickBar);

	int32 CurrentFocusedSlot;
	int32 PreviousFocusedSlot;
	int32 SecondaryFocusedSlot;
	uint8 Pad_C[0x4];
	TArray<FQuickBarSlot> Slots;
	FQuickBarData DataDefinition;
	TSet<UFortItemDefinition*> EquippedItemDefinitions;
	TArray<int32> SharedVisibleSlotIndicesWhenUsingGamepad;
};

class AFortQuickBars : public AActor {
public:
	DefineUnrealClass(AFortQuickBars);

	DefineUProperty(FQuickBar, PrimaryQuickBar);
	DefineUProperty(FQuickBar, SecondaryQuickBar);
public:
	void ServerAddItemInternal(const FGuid& Item, uint8 InQuickBar, int32 Slot);

	void ServerRemoveItemInternal(const FGuid& Item, bool bFindReplacement, bool bForce);

	void EmptySlot(uint8 InQuickBar, int32 SlotIndex);

	void ServerActivateSlotInternal(uint8 InQuickBar, int32 Slot, float ActivateDelay, bool bUpdatePreviousFocusedSlot);

	void EnableSlot(uint8 InQuickBar, int32 SlotIndex);

	int32 FindQuickBarSlotForItem(uint8 QuickBar, FGuid Guid) const;

	void EmptyQuickbarSlot(FGuid Guid);

	AFortPlayerController* GetOwnerPlayerController() const;

	void EquipHarvestingTool();

	void AddItemToQuickBar(FGuid Guid, uint8 QuickBar);

	void OnRep_PrimaryQuickBar();

	void OnRep_SecondaryQuickBar();

	bool Update();
};