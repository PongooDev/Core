#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/CoreOnline/Public/Online/CoreOnline.h"

struct FUniqueNetIdRepl : public FUniqueNetIdWrapper {
public:
	DefineUnrealStruct(FUniqueNetIdRepl);
public:
	uint8 Padding[0x27];
};