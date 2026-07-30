#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Math/Color.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/AthenaCosmeticItemDefinition.h"

class UObject;
class UTexture2D;

class UAthenaLoadingScreenItemDefinition : public UAthenaCosmeticItemDefinition {
public:
	DefineUnrealClass(UAthenaLoadingScreenItemDefinition);

	DefineUProperty(TSoftObjectPtr<UTexture2D>, BackgroundImage);
	DefineUProperty(TSoftObjectPtr<UObject>, BackgroundMaterialOrTexture);
	DefineUProperty(TSoftClassPtr<UClass>, BackgroundWidget);
	DefineUProperty(FVector2D, BackgroundDesiredSize);
	DefineUProperty(FLinearColor, BackgroundColor);
};
