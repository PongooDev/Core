#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Mcp/FortMcpProfile.h"
#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "FortniteGame/Public/Inventory/FortQuickBars.h"

class AFortInventory;
class UFortWorldItem;

struct FFortWorldPlayerLoadout {
public:
	DefineUnrealStruct(FFortWorldPlayerLoadout);

	DefineStructProperty(bool, bPlayerIsNew);
	DefineStructProperty(TArray<FString>, PinnedSchematicInstances);
	DefineStructProperty(FQuickBar, PrimaryQuickBarRecord);
	DefineStructProperty(FQuickBar, SecondaryQuickBarRecord);
	DefineStructProperty(int32, ZonesCompleted);
public:
	uint8 Padding[0x13C];
};

class UFortMcpProfileWorld : public UFortMcpProfile {
public:
	DefineUnrealClass(UFortMcpProfileWorld);

	DefineUProperty(FFortWorldPlayerLoadout, CachedPlayerLoadout);
	DefineUProperty(AFortInventory*, FortInventory);
	DefineUProperty(TArray<UFortWorldItem*>, WorldItemsCachedForRejoin);
	DefineUProperty(bool, bIsInventoryInitialized);
};
