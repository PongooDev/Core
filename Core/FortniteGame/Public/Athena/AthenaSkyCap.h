#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class ESkyCapState {
public:
	DefineUnrealEnum(ESkyCapState);

	DefineEnumProperty(Hidden);
	DefineEnumProperty(StormStarting);
};

class AAthenaSkyCap : public AActor {
public:
	DefineUnrealClass(AAthenaSkyCap);

	DefineUProperty(ESkyCapState, SkyCapState);
};
