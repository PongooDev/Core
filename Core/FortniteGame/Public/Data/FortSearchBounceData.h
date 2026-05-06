#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

struct FFortSearchBounceData {
public:
	DefineUnrealStruct(FFortSearchBounceData);

	DefineStructProperty(FVector, BounceNormal);
	DefineStructProperty(uint32, SearchAnimationCount);
};