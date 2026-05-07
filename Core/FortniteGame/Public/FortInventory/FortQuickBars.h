#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

#include "FortniteGame/Public/FortEnums.h"

class UFortItemDefinition;
class UFortItem;
class UFortWorldItem;

struct FQuickBarSlot {
public:
	DefineUnrealStruct(FQuickBarSlot);

	DefineStructProperty(TArray<FGuid>, Items);
	DefineStructProperty(bool, bEnabled);
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

	DefineStructProperty(TArray<FQuickBarSlotData>, QuickbarSlots);
};

struct FQuickBar {
public:
	DefineUnrealStruct(FQuickBar);

	DefineStructProperty(TArray<FQuickBarSlot>, Slots);
	DefineStructProperty(int32, CurrentFocusedSlot);
	DefineStructProperty(int32, PreviousFocusedSlot);
	DefineStructProperty(int32, SecondaryFocusedSlot);
	DefineStructProperty(TArray<int32>, SharedVisibleSlotIndicesWhenUsingGamepad);
	DefineStructProperty(FQuickBarData, DataDefinition);
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
};