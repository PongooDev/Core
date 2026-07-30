#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class AFortPlayerController;
class UFortItemDefinition;
class UFortAccountItemDefinition;

struct FFortItemEntry;

struct FItemAndCount {
public:
	DefineUnrealStruct(FItemAndCount);
public:
	int32 Count;
	uint8 Pad_4[0x4]; // im pretty sure this is an int32, but i dont know what it is
	UFortItemDefinition* Item;
};