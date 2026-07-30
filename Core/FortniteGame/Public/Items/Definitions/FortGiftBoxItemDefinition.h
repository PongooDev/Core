#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/FortGlobals.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UObject;

class EFortGiftWrapType {
public:
	DefineUnrealEnum(EFortGiftWrapType);

	DefineEnumProperty(System);
	DefineEnumProperty(UserFree);
	DefineEnumProperty(UserUnlock);
	DefineEnumProperty(UserConsumable);
	DefineEnumProperty(Message);
};

class UFortGiftBoxItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortGiftBoxItemDefinition);

	DefineUProperty(FText, DefaultHeaderText);
	DefineUProperty(FText, DefaultBodyText);
	DefineUProperty(FString, HeaderAssetString);
	DefineUProperty(FString, BodyAssetString);
	DefineUProperty(EFortGiftWrapType, GiftWrapType);
	DefineUProperty(bool, bReuseExistingBoxIfPossible);
	DefineUProperty(ESubGame, RestrictToSubgame);
	DefineUProperty(int32, SortPriority);
	DefineUProperty(TSoftObjectPtr<UObject>, ItemDisplayAsset);
	DefineUProperty(TSoftClassPtr<UClass>, GiftBoxPreMessageWidgetRef);
	DefineUProperty(TSoftClassPtr<UClass>, GiftBoxHeaderSubWidgetRef);
};
