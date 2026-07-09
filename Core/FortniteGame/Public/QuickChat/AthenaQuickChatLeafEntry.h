#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

class UAthenaEmojiItemDefinition;

struct FAthenaQuickChatLeafEntry {
public:
	DefineUnrealStruct(FAthenaQuickChatLeafEntry);

	DefineStructProperty(bool, bPopulateBrushFromContextObject);
	DefineStructProperty(uint8, FilterType);
	DefineStructProperty(UAthenaEmojiItemDefinition*, EmojiItemDefinition);
	DefineStructProperty(uint8, TeamCommType);
	DefineStructProperty(FGameplayTag, OptionGameplayTag);
};