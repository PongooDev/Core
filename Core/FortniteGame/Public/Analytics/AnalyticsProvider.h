#pragma once
#include "pch.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

enum EAnalyticsProviderType : int32
{
	EPT_Client = 0,
    EPT_Server = 1,
    EPT_ServerGameInstance = 2,
    EPT_EsportsServerGameInstance = 3,
    EPT_MMRServerGameInstance = 4,
};

struct IAnalyticsProvider {
public:
	void** VTable;
};
