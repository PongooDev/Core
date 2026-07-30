#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/Engine.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Core/Public/Templates/SharedPointer.h"

#include "FortniteGame/Public/Analytics/AnalyticsProvider.h"

class UFortAnalytics {
public:
	static inline void (*SetGameSessionIDOG)(void** Provider, FString GameSessionID, bool bSendEventIfChanged);
	static void SetGameSessionID(void** Provider, FString GameSessionID, bool bSendEventIfChanged);

	static inline void (*SetGameStateClassNameOG)(void** Provider, FString GameStateClassName, bool bSendEventIfChanged);
	static void SetGameStateClassName(void** Provider, FString GameStateClassName, bool bSendEventIfChanged);
public:
	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindUFortAnalytics_SetGameSessionID()), SetGameSessionID, (LPVOID*)&SetGameSessionIDOG);

		MH_CreateHook((LPVOID)(ImageBase + Finder::FindUFortAnalytics_SetGameStateClassName()), SetGameStateClassName, (LPVOID*)&SetGameStateClassNameOG);

		Log("UFortAnalytics Hooked!");
	}
};