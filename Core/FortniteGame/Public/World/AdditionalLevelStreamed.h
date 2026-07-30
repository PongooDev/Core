#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

struct FAdditionalLevelStreamed
{
public:
	DefineUnrealStruct(FAdditionalLevelStreamed);

	DefineStructProperty(FName, LevelName);
	DefineStructProperty(bool, bIsServerOnly);
};