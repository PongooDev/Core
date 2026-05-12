#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ObjectMacros.h"

struct FSoftObjectPath
{
public:
	FName AssetPathName;
	FString SubPathString;
};