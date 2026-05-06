#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

enum class EFortItemEntryState : uint8;

struct FFortItemEntryStateValue {
public:
	DefineUnrealStruct(FFortItemEntryStateValue);

	DefineStructProperty(int32, IntValue);

	DefineStructProperty(FName, NameValue);

	DefineStructProperty(EFortItemEntryState, StateType);
public:
	bool operator==(FFortItemEntryStateValue& Other) const {
		return StateType == Other.StateType
			&& IntValue == Other.IntValue
			&& NameValue.IsEqual(Other.NameValue);
	}
};