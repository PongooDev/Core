#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class ULevelSaveRecord;
class UWorld;

class EPlaysetOffsetType {
public:
	DefineUnrealEnum(EPlaysetOffsetType);

	DefineEnumProperty(CustomOffsetFromCorner);
	DefineEnumProperty(Center);
};

class UFortPlaysetItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortPlaysetItemDefinition);

	DefineUProperty(TSoftObjectPtr<UWorld>, PlaysetToSpawn);
	DefineUProperty(FString, OptionFullSavePath);
	DefineUProperty(int32, SizeX);
	DefineUProperty(int32, SizeY);
	DefineUProperty(int32, SizeZ);
	DefineUProperty(float, ZSnapTolerance);
	DefineUProperty(EPlaysetOffsetType, OffsetType);
	DefineUProperty(FVector, LocationOffset);
	DefineUProperty(FRotator, DefaultRotation);
	DefineUProperty(FText, UIDisplayName);
	DefineUProperty(TSoftClassPtr<UClass>, MinigameClass);
	DefineUProperty(ULevelSaveRecord*, LevelSaveRecord);
	DefineUProperty(bool, bHasBeenCooked);
	DefineUProperty(int32, TotalCreativeActorSize);
	DefineUProperty(int32, TotalCreativeActorInstanceSize);
	DefineUProperty(FName, PlaysetName);
};
