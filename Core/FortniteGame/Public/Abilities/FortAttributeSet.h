#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AttributeSet.h"

struct FFortGameplayAttributeData : public FGameplayAttributeData {
public:
	DefineUnrealStruct(FFortGameplayAttributeData);

	DefineStructProperty(float, Minimum);
	DefineStructProperty(float, Maximum);
public:
	uint8 Padding[0x10];
};

class UFortAttributeSet : public UAttributeSet {
public:
	DefineUnrealClass(UFortAttributeSet);
};