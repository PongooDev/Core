#pragma once
#include "pch.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FPrimaryAssetType {
public:
	DefineUnrealStruct(FPrimaryAssetType);

	DefineStructProperty(FName, Name);
public:
	uint8 Padding[0x8];
};

struct FPrimaryAssetId {
public:
	DefineUnrealStruct(FPrimaryAssetId);

	DefineStructProperty(FPrimaryAssetType, PrimaryAssetType);
	DefineStructProperty(FName, PrimaryAssetName);
public:
	uint8 Padding[0x10];
};


