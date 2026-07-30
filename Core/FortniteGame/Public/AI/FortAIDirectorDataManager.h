#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class UObject;

class AFortAIDirectorDataManager : public AActor {
public:
	DefineUnrealClass(AFortAIDirectorDataManager);

	DefineUProperty(UObject*, OwnerObject);
};
