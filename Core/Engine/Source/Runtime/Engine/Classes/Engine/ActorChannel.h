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
	void Close();

	void StartBecomingDormant();

	void SetChannelActorForDestroy(struct FActorDestructionInfo* DestructInfo);

	void SetChannelActor(class AActor* InActor);

	bool ReplicateActor();
};