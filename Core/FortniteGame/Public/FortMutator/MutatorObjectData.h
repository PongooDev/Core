#pragma once
#include "pch.h"

// Split out of MutatorTypes.h so AFortGameStateAthena does not drag the whole mutator struct set
// into every translation unit that includes it.

struct FGameplayMutatorEventData {
public:
	DefineUnrealStruct(FGameplayMutatorEventData);

	DefineStructProperty(int32, EventId);
	DefineStructProperty(int32, EventParam1);
	DefineStructProperty(int32, EventParam2);
	DefineStructProperty(int32, EventParam3);
public:
	uint8 Padding[0x10];
};

struct FGameplayMutatorObjectData {
public:
	DefineUnrealStruct(FGameplayMutatorObjectData);

	DefineStructProperty(UObject*, TheObject);
	DefineStructProperty(int32, ObjectId);
	DefineStructProperty(int32, ObjectValue1);
	DefineStructProperty(int32, ObjectValue2);
public:
	uint8 Padding[0x28];
};

struct FGameplayMutatorObjectDataArray {
public:
	DefineUnrealStruct(FGameplayMutatorObjectDataArray);

	DefineStructProperty(TArray<FGameplayMutatorObjectData>, ObjectDataList);
public:
	uint8 Padding[0x118];
};
