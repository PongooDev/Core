#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FFortItemEntryStateValue {
public:
	DefineUnrealStruct(FFortItemEntryStateValue);

	DefineStructProperty(int32, IntValue);

	DefineStructProperty(FName, NameValue);

	DefineStructProperty(uint8, StateType);
public:
	uint8 Padding[0x18];
public:
	bool operator==(FFortItemEntryStateValue& Other) const {
		return StateType == Other.StateType
			&& IntValue == Other.IntValue
			&& NameValue.IsEqual(Other.NameValue);
	}
};