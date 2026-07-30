#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

enum EContextCredentials : int32
{
	CXC_Client = 0,
	CXC_DedicatedServer = 1,
	CXC_Cheater = 2,
	CXC_Public = 3,
};

class UMcpProfileGroup : public UObject {
public:
	DefineUnrealClass(UMcpProfileGroup);
public:
	static inline void (*SendRequestNowOG)(UMcpProfileGroup* This, void* HttpRequest, EContextCredentials ContextCredentials);
	static void SendRequestNow(UMcpProfileGroup* This, void* HttpRequest, EContextCredentials ContextCredentials);
public:
	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindUMcpProfileGroup_SendRequestNow()), SendRequestNow, (LPVOID*)&SendRequestNowOG);

		Log("Hooked UMcpProfileGroup");
	}
};
