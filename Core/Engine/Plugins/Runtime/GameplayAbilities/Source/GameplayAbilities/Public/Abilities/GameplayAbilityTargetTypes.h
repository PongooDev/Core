#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

namespace EAbilityGenericReplicatedEvent
{
	enum Type : int
	{
		/** A generic confirmation to commit the ability */
		GenericConfirm = 0,
		/** A generic cancellation event. Not necessarily a canellation of the ability or targeting. Could be used to cancel out of a channelling portion of ability. */
		GenericCancel,
		/** Additional input presses of the ability (Press X to activate ability, press X again while it is active to do other things within the GameplayAbility's logic) */
		InputPressed,
		/** Input release event of the ability */
		InputReleased,
		/** A generic event from the client */
		GenericSignalFromClient,
		/** A generic event from the server */
		GenericSignalFromServer,
		/** Custom events for game use */
		GameCustom1,
		GameCustom2,
		GameCustom3,
		GameCustom4,
		GameCustom5,
		GameCustom6,
		MAX
	};
}

struct FGameplayAbilityTargetDataHandle {
public:
	DefineUnrealStruct(FGameplayAbilityTargetDataHandle);
};

/** Payload for generic replicated events */
struct FAbilityReplicatedData
{
	FAbilityReplicatedData() : bTriggered(false), VectorPayload(FVector(0, 0, 0)) {}

	/** Event has triggered */
	bool bTriggered;

	/** Optional Vector payload for event */
	FVector VectorPayload;

	/** Delegate that will be called on replication */
	uint8 Delegate[0x10];
};