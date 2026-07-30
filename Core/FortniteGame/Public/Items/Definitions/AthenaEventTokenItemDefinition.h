#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class EEventTokenType {
public:
	DefineUnrealEnum(EEventTokenType);

	DefineEnumProperty(Invite);
	DefineEnumProperty(Creation);
};

class UAthenaEventTokenItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UAthenaEventTokenItemDefinition);

	DefineUProperty(EEventTokenType, TokenType);
};
