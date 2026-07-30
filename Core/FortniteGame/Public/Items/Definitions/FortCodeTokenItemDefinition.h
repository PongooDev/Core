#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"
#include "FortniteGame/Public/Items/Definitions/FortConsumableAccountItemDefinition.h"

class ECodeTokenPlatform {
public:
	DefineUnrealEnum(ECodeTokenPlatform);

	DefineEnumProperty(PC);
	DefineEnumProperty(PS4);
	DefineEnumProperty(XBOX);
};

class UFortCodeTokenItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortCodeTokenItemDefinition);

	DefineUProperty(FString, RedemptionCodeRule);
	DefineUProperty(FString, RedemptionCodeGroup);
	DefineUProperty(TArray<ECodeTokenPlatform>, AllowedPlatforms);
	DefineUProperty(EItemProfileType, ProfileType);
};
