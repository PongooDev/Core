#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UFortChallengeBundleItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortChallengeBundleItemDefinition);

	DefineUProperty(FString, CalendarEventTag);
	DefineUProperty(FString, CalendarEventName);
	DefineUProperty(int32, MaxChainDepth);
};
