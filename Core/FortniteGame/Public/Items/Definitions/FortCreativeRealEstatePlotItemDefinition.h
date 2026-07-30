#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class ERealEstateOffsetType {
public:
	DefineUnrealEnum(ERealEstateOffsetType);

	DefineEnumProperty(CustomOffsetFromCorner);
	DefineEnumProperty(Center);
};

class UFortCreativeRealEstatePlotItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortCreativeRealEstatePlotItemDefinition);

	DefineUProperty(FText, UIDisplayName);
	DefineUProperty(FString, InitialSaveFileName);
	DefineUProperty(int32, SizeX);
	DefineUProperty(int32, SizeY);
	DefineUProperty(FVector, LocationOffset);
	DefineUProperty(FRotator, DefaultRotation);
	DefineUProperty(ERealEstateOffsetType, OffsetType);
	DefineUProperty(FSlateBrush, MapImage);
	DefineUProperty(int32, SortIndex);
	DefineUProperty(bool, bIsAvailableToUsers);
};
