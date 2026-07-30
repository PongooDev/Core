#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Mcp/FortMcpProfile.h"

class AFortInventory;
class UFortWorldItem;

class UFortMcpProfileWorld : public UFortMcpProfile {
public:
	DefineUnrealClass(UFortMcpProfileWorld);

	DefineUProperty(AFortInventory*, FortInventory);
	DefineUProperty(TArray<UFortWorldItem*>, WorldItemsCachedForRejoin);
	DefineUProperty(bool, bIsInventoryInitialized);
};
