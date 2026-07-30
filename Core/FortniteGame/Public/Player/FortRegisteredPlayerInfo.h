#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UFortMcpProfileWorld;

class UFortRegisteredPlayerInfo : public UObject {
public:
	DefineUnrealClass(UFortRegisteredPlayerInfo);

	DefineUProperty(int32, PlayerIndex);
	DefineUProperty(UFortMcpProfileWorld*, OutpostProfile);
};