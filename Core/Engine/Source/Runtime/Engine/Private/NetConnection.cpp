#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/ActorChannel.h"

int32 UNetConnection::IsNetReady(bool Saturate)
{
	int32(*IsNetReadyInternal)(UNetConnection*, bool) = decltype(IsNetReadyInternal)(ImageBase + Finder::FindUNetConnection_IsNetReady());
	return IsNetReadyInternal(this, Saturate);
}

UChannel* UNetConnection::CreateChannel(EChannelType ChType, bool bOpenedLocally, int32 ChIndex)
{
	UChannel* (*CreateChannelInternal)(UNetConnection*, EChannelType, bool, int32) = decltype(CreateChannelInternal)(ImageBase + Finder::FindUNetConnection_CreateChannel());
	return CreateChannelInternal(this, ChType, bOpenedLocally, ChIndex);
}

void UNetConnection::CleanUp()
{
	void (*CleanUpInternal)(UNetConnection*) = decltype(CleanUpInternal)(ImageBase + Finder::FindUNetConnection_CleanUp());
	CleanUpInternal(this);
}

UActorChannel* UNetConnection::FindActorChannelRef(const TWeakObjectPtr<AActor>& Actor)
{
	if (!this)
		return nullptr;

	if (Version::Engine_Version <= 4.19) {
		if (!OpenChannels.IsValid()) {
			return nullptr;
		}

		for (int i = 0; i < OpenChannels.Num(); i++)
		{
			if (!OpenChannels.IsValidIndex(i))
				continue;

			UActorChannel* Channel = OpenChannels[i]->Cast<UActorChannel>();
			if (!Channel)
				continue;

			if (Channel->Actor == Actor.Get())
				return Channel;
		}

		return nullptr;
	}
	else {
		return nullptr;
	}
}