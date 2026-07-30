#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Templates/SharedPointer.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameInstance.h"

#include "FortniteGame/Public/Analytics/AnalyticsProvider.h"

class UFortGameInstance : public UGameInstance {
public:
	DefineUnrealClass(UFortGameInstance);
public:
	bool ServerTravel(const FString& URL, bool bAbsolute = false, bool bShouldSkipGameNotify = false);

	static void Hook() {
		Log("UFortGameInstance Hooked!");
	}
};