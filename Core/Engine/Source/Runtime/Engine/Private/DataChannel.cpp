#include "pch.h"
#include "Engine/Source/Runtime/Engine/Public/Net/DataChannel.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/ActorChannel.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Channel.h"

#include "Engine/Source/Runtime/Core/Public/Misc/AssertionMacros.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"
#include "Engine/Source/Runtime/Engine/Public/Net/DataBunch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/CoreNet.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"

void UActorChannel::Close()
{
	void (*CloseInternal)(UActorChannel*) = decltype(CloseInternal)(ImageBase + Finder::FindUActorChannel_Close());
	CloseInternal(this);
}

void UActorChannel::StartBecomingDormant()
{
	void (*StartBecomingDormantInternal)(UActorChannel*) = decltype(StartBecomingDormantInternal)(ImageBase + Finder::FindUActorChannel_StartBecomingDormant());
	StartBecomingDormantInternal(this);
}

void UActorChannel::SetChannelActorForDestroy(FActorDestructionInfo* DestructInfo)
{
	if (Finder::FindUActorChannel_SetChannelActorForDestroy()) {
		void (*SetChannelActorForDestroyInternal)(UActorChannel*, FActorDestructionInfo*) = decltype(SetChannelActorForDestroyInternal)(ImageBase + Finder::FindUActorChannel_SetChannelActorForDestroy());
		SetChannelActorForDestroyInternal(this, DestructInfo);
	}
	else {
		if (Version::Engine_Version == 4.16) {
			if (!GetClosing() && (Connection->State == USOCK_Open || Connection->State == USOCK_Pending))
			{
				// Send a close notify, and wait for ack.
				FOutBunch CloseBunch(this, 1);
				CloseBunch.SetbReliable(1);
				CloseBunch.SetbDormant(0);

				// Serialize DestructInfo
				Connection->PackageMap->WriteObject(CloseBunch, DestructInfo->ObjOuter().Get(), DestructInfo->NetGUID(), DestructInfo->PathName());

				Log("SetChannelActorForDestroy");

				SendBunch(&CloseBunch, 0);
			}
		}
	}
}

void UActorChannel::SetChannelActor(AActor* InActor)
{
	static void (*SetChannelActorInternal)(UActorChannel*, AActor*) = decltype(SetChannelActorInternal)(ImageBase + Finder::FindUActorChannel_SetChannelActor());
	return SetChannelActorInternal(this, InActor);
}

int32 UChannel::IsNetReady(bool Saturate)
{
	if (Version::Engine_Version == 4.16) {
		// If saturation allowed, ignore queued byte count.
		if (NumOutRec >= RELIABLE_BUFFER - 1)
		{
			return 0;
		}
		return Connection->IsNetReady(Saturate);
	}

	return 0;
}

bool UActorChannel::ReplicateActor()
{
	bool (*ReplicateActorInternal)(UActorChannel*) = decltype(ReplicateActorInternal)(ImageBase + Finder::FindUActorChannel_ReplicateActor());
	return ReplicateActorInternal(this);
}

FPacketIdRange UChannel::SendBunch(FOutBunch* Bunch, bool Merge)
{
	FPacketIdRange(*SendBunchInternal)(UChannel*, FOutBunch*, bool) = decltype(SendBunchInternal)(ImageBase + Finder::FindUChannel_SendBunch());
	return SendBunchInternal(this, Bunch, Merge);
}