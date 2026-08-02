#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/ActorChannel.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/ChildConnection.h"

int32 UNetConnection::IsNetReady(bool Saturate)
{
	if (!this) {
		return 0;
	}

	int32(*&IsNetReadyInternal)(UNetConnection*, bool) = decltype(IsNetReadyInternal)(VTable[Finder::FindUNetConnection_IsNetReadyVFT()]);
	return IsNetReadyInternal(this, Saturate);
}

UChildConnection* UNetConnection::GetUChildConnection()
{
	return NULL;
}

UChannel* UNetConnection::CreateChannel(EChannelType ChType, bool bOpenedLocally, int32 ChIndex)
{
	UChannel* (*CreateChannelInternal)(UNetConnection*, EChannelType, bool, int32) = decltype(CreateChannelInternal)(ImageBase + Finder::FindUNetConnection_CreateChannel());
	return CreateChannelInternal(this, ChType, bOpenedLocally, ChIndex);
}

void UNetConnection::CleanUp()
{
	void (*&CleanUpInternal)(UNetConnection*) = decltype(CleanUpInternal)(VTable[Finder::FindUNetConnection_CleanUpVFT()]);
	CleanUpInternal(this);
}

void UNetConnection::SendChallengeControlMessage(UNetConnection* This, void* Response)
{
	FCoreConfig& Config = ConfigurationManager::GetConfig();
	if (Config.bUseGameSessions && !Config.bSkipSessionValidation)
		return SendChallengeControlMessageOG(This, Response);

	if (Response)
	{
		uint8& ResponseCode = *(uint8*)Response;
		if (ResponseCode != 1)
		{
			ResponseCode = 1;
		}
	}

	return SendChallengeControlMessageOG(This, Response);
}

void UNetConnection::Hook()
{
	MH_CreateHook((LPVOID)(ImageBase + Finder::FindUNetConnection_SendChallengeControlMessage()), SendChallengeControlMessage, (LPVOID*)&SendChallengeControlMessageOG);

	Log("UNetConnection Hooked");
}
