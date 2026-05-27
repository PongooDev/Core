#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/ActorChannel.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/ChildConnection.h"

int32 UNetConnection::IsNetReady(bool Saturate)
{
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