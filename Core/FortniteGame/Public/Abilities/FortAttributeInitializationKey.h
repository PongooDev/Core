#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FFortAttributeInitializationKey {
public:
	DefineUnrealStruct(FFortAttributeInitializationKey);

	DefineStructProperty(FName, AttributeInitCategory);
	DefineStructProperty(FName, AttributeInitSubCategory);
public:
	uint8 Padding[0x10];
};
