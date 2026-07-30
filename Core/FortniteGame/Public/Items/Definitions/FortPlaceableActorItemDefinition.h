#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"
#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"

class UFortPlaceableActorItemDefinition : public UFortItemDefinition {
public:
	DefineUnrealClass(UFortPlaceableActorItemDefinition);

	DefineUProperty(UClass*, BasePlaceableActorClass);
	DefineUProperty(FSlateBrush, DisplayImage);
	DefineUProperty(FName, ActorName);
};
