#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"

#include "AthenaQuickChatLeafEntry.h"

class UAthenaQuickChatBank : public UDataAsset {
public:
	DefineUnrealClass(UAthenaQuickChatBank);

	DefineUProperty(TArray<FAthenaQuickChatLeafEntry>, ChatOptions);
};