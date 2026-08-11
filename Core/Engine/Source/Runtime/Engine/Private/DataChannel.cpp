#include "pch.h"
#include "Engine/Source/Runtime/Engine/Public/Net/DataChannel.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/ActorChannel.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Channel.h"

#include "Engine/Source/Runtime/Core/Public/Misc/AssertionMacros.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"
#include "Engine/Source/Runtime/Engine/Public/Net/DataBunch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/CoreNet.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"
#include "Engine/Source/Runtime/Engine/Public/EngineLogs.h"

void UChannel::Close()
{
	if (!this) {
		return;
	}

	if (Finder::FindUChannel_CloseVFT()) {
		void (*&CloseInternal)(UChannel*) = decltype(CloseInternal)(VTable[Finder::FindUChannel_CloseVFT()]);
		CloseInternal(this);
	}
}

void UChannel::StartBecomingDormant()
{
	if (Finder::FindUChannel_StartBecomingDormantVFT()) {
		void (*&StartBecomingDormantInternal)(UChannel*) = decltype(StartBecomingDormantInternal)(VTable[Finder::FindUChannel_StartBecomingDormantVFT()]);
		StartBecomingDormantInternal(this);
	}
}

void UActorChannel::SetChannelActorForDestroy(FActorDestructionInfo* DestructInfo)
{
	if (Finder::FindUActorChannel_SetChannelActorForDestroy()) {
		void (*SetChannelActorForDestroyInternal)(UActorChannel*, FActorDestructionInfo*) = decltype(SetChannelActorForDestroyInternal)(ImageBase + Finder::FindUActorChannel_SetChannelActorForDestroy());
		SetChannelActorForDestroyInternal(this, DestructInfo);
		return;
	}

	if (!this || !DestructInfo)
		return;

	UNetConnection* Conn = this->Connection;
	if (!Conn || !Conn->PackageMap)
		return;

	if (this->Closing)
		return;

	const EConnectionState S = Conn->State;
	if (S != USOCK_Open && S != USOCK_Pending)
		return;

	if (!Finder::FindFOutBunch_Constructor() || !Finder::FindUPackageMap_WriteObject() || !Finder::FindUChannel_SendBunch())
	{
		UE_LOG(LogNetTraffic, Warning,
			TEXT("SetChannelActorForDestroy fallback unavailable: FOutBunch ctor / WriteObject / SendBunch offset not found on this build."));
		return;
	}

	FOutBunch CloseBunch(this, true);

	if (!CloseBunch.MarkReliable(this, this->ChIndex))
	{
		UE_LOG(LogNetTraffic, Warning,
			TEXT("SetChannelActorForDestroy fallback: could not locate the FOutBunch reliable flag on this build; sending the close bunch unreliably."));
	}

	Conn->PackageMap->WriteObject(CloseBunch, DestructInfo->ObjOuter.Get(), FNetworkGUID(DestructInfo->NetGUID), DestructInfo->PathName);

	UE_LOG(LogNetTraffic, Log,
		TEXT("SetChannelActorForDestroy fallback: Channel %d. NetGUID <%u> Path: %s."),
		this->ChIndex, DestructInfo->NetGUID, *DestructInfo->PathName);

	SendBunch(&CloseBunch, false);
}

void UActorChannel::SetChannelActor(AActor* InActor)
{
	static void (*SetChannelActorInternal)(UActorChannel*, AActor*) = decltype(SetChannelActorInternal)(ImageBase + Finder::FindUActorChannel_SetChannelActor());
	return SetChannelActorInternal(this, InActor);
}

int32 UChannel::IsNetReady(bool Saturate)
{
	// If saturation allowed, ignore queued byte count.
	if (NumOutRec >= RELIABLE_BUFFER - 1)
	{
		return 0;
	}
	return Connection->IsNetReady(Saturate);
}

bool UActorChannel::ReplicateActor()
{
	bool (*ReplicateActorInternal)(UActorChannel*) = decltype(ReplicateActorInternal)(ImageBase + Finder::FindUActorChannel_ReplicateActor());
	return ReplicateActorInternal(this);
}

FPacketIdRange UChannel::SendBunch(FOutBunch* Bunch, bool Merge)
{
	if (!this || !Bunch)
		return FPacketIdRange();

	typedef void(*SendBunchInternalFn)(UChannel*, FPacketIdRange*, FOutBunch*, bool);

	FPacketIdRange Result;
	if (Finder::FindUChannel_SendBunchVFT())
	{
		SendBunchInternalFn Fn = reinterpret_cast<SendBunchInternalFn>(VTable[Finder::FindUChannel_SendBunchVFT()]);
		Fn(this, &Result, Bunch, Merge);
	}
	else if (Finder::FindUChannel_SendBunch())
	{
		SendBunchInternalFn Fn = reinterpret_cast<SendBunchInternalFn>(ImageBase + Finder::FindUChannel_SendBunch());
		Fn(this, &Result, Bunch, Merge);
	}

	return Result;
}