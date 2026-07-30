#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "FortniteGame/Public/Items/Definitions/FortAlterationItemDefinition.h"

class UFortAbilityKit;

struct FHomebaseSquadSlotId {
public:
	DefineUnrealStruct(FHomebaseSquadSlotId);

	DefineStructProperty(FName, SquadId);
	DefineStructProperty(int32, SquadSlotIndex);
public:
	uint8 Padding[0xC];
};

struct FHomebaseNodeLevel {
public:
	DefineUnrealStruct(FHomebaseNodeLevel);

	DefineStructProperty(FName, DisplayDataId);
	DefineStructProperty(int32, MinCommanderLevel);
	DefineStructProperty(TArray<FFortItemQuantityPair>, Cost);
	DefineStructProperty(TArray<FName>, GameplayEffectRowNames);
	DefineStructProperty(TSoftObjectPtr<UFortAbilityKit>, AbilityKit);
	DefineStructProperty(TArray<FHomebaseSquadSlotId>, UnlockedSquadSlots);
public:
	uint8 Padding[0x68];
};

class EHomebaseNodeType {
public:
	DefineUnrealEnum(EHomebaseNodeType);

	DefineEnumProperty(Gadget);
	DefineEnumProperty(Utility);
	DefineEnumProperty(Hidden);
};

class UFortHomebaseNodeItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortHomebaseNodeItemDefinition);

	DefineUProperty(TArray<FHomebaseNodeLevel>, LevelData);
	DefineUProperty(EHomebaseNodeType, DisplayType);
};
