#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

struct FSkyCapTargetData {
public:
	DefineUnrealStruct(FSkyCapTargetData);

	DefineStructProperty(float, TargetHeight);
	DefineStructProperty(float, MoveTime);
public:
	uint8 Padding[0x8];
};

class ESkyCapState {
public:
	DefineUnrealEnum(ESkyCapState);

	DefineEnumProperty(Hidden);
	DefineEnumProperty(StormStarting);
};

class AAthenaSkyCap : public AActor {
public:
	DefineUnrealClass(AAthenaSkyCap);

	DefineUProperty(FSkyCapTargetData, TargetData);
	DefineUProperty(ESkyCapState, SkyCapState);
};
