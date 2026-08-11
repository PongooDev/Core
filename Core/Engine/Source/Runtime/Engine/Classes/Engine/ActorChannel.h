#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Channel.h"

class UActorChannel : public UChannel {
public:
	DefineUnrealClass(UActorChannel);

	DefineUProperty(AActor*, Actor);

	DefineCustomProperty(double, RelevantTime, ServerOffsets::UActorChannel__RelevantTime);

	DefineCustomProperty(double, LastUpdateTime, ServerOffsets::UActorChannel__LastUpdateTime);

public:
	static FORCEINLINE uintptr_t ActorOffset()
	{
		static uintptr_t Cached = 0;

		if (!Cached)
		{
			const uintptr_t Offset = UActorChannel::StaticClass()->GetPropertyOffset("Actor");
			if (Offset != (uintptr_t)-1 && Offset > 0)
				Cached = Offset;
		}

		return Cached;
	}

	DefineCustomProperty(FNetworkGUID, ActorNetGUID, UActorChannel::ActorOffset() ? UActorChannel::ActorOffset() + 0x08 : 0);
	DefineCustomProperty(float, CustomTimeDilation, UActorChannel::ActorOffset() ? UActorChannel::ActorOffset() + 0x0C : 0);

public:
	static FORCEINLINE uintptr_t ChannelFlagsOffset()
	{
		const uintptr_t Base = ActorOffset();
		return Base ? Base + 0x20 : 0;
	}

	FORCEINLINE bool GetActorChannelFlag(int32 BitIndex) const
	{
		const uintptr_t Offset = ChannelFlagsOffset();
		if (!Offset)
			return false;

		return (*reinterpret_cast<uint32*>((uintptr_t)this + Offset) & (1u << BitIndex)) != 0;
	}

	FORCEINLINE bool _GetSpawnAcked() const { return GetActorChannelFlag(0); }
	FORCEINLINE bool _GetbForceCompareProperties() const { return GetActorChannelFlag(1); }
	FORCEINLINE bool _GetbIsReplicatingActor() const { return GetActorChannelFlag(2); }

	__declspec(property(get = _GetSpawnAcked)) bool SpawnAcked;
	__declspec(property(get = _GetbForceCompareProperties)) bool bForceCompareProperties;
	__declspec(property(get = _GetbIsReplicatingActor)) bool bIsReplicatingActor;

public:
	void SetChannelActorForDestroy(struct FActorDestructionInfo* DestructInfo);

	void SetChannelActor(class AActor* InActor);

	bool ReplicateActor();
};