#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Guid.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/GameplayAbilitySpecHandle.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"

#include "FortniteGame/Public/Items/FortItemEntry.h"

class UFortItemDefinition;
class UFortWorldItem;
class AFortInventory;
class UFortAlterationItemDefinition;
class UAthenaItemWrapDefinition;

struct FFortItemList : public FFastArraySerializer {
public:
	DefineUnrealStruct(FFortItemList);
public:
	DefineStructProperty(TArray<FFortItemEntry>, ReplicatedEntries);

	DefineStructProperty(TArray<UFortWorldItem*>, ItemInstances);
public:
	uint8 Padding[0xC8];
};