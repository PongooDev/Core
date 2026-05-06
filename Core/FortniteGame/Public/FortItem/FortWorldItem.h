#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

#include "FortItemEntry.h"
#include "FortItem.h"

class AFortPlayerController;
class UFortItemDefinition;
class UFortAccountItemDefinition;

class UFortWorldItem : public UFortItem {
public:
	DefineUnrealClass(UFortWorldItem);

	DefineUProperty(FFortItemEntry, ItemEntry);

public:
	operator FFortItemEntry& () {
		return ItemEntry;
	}

	operator FFortItemEntry* () {
		return &ItemEntry;
	}
};