#include "pch.h"
#include "../Public/Abilities/GameplayAbilityTypes.h"
#include "../Public/GameplayAbilitySpec.h"

FGameplayAbilitySpec::FGameplayAbilitySpec(TSubclassOf<UGameplayAbility> InAbilityClass, int32 InLevel, int32 InInputID, UObject* InSourceObject)
{
	FGameplayAbilitySpec* (*ConstructorInternal)(FGameplayAbilitySpec*, TSubclassOf<UGameplayAbility>, int32, int32, UObject*) = decltype(ConstructorInternal)(ImageBase + Finder::FindAbilitySpecClassConstructor());
	ConstructorInternal(this, InAbilityClass, InLevel, InInputID, InSourceObject);
}

FGameplayAbilitySpec::FGameplayAbilitySpec(UGameplayAbility* InAbility, int32 InLevel, int32 InInputID, UObject* InSourceObject)
{
	if (Finder::FindAbilitySpecCDOConstructor() > 0) {
		FGameplayAbilitySpec* (*ConstructorInternal)(FGameplayAbilitySpec*, UGameplayAbility*, int32, int32, UObject*) = decltype(ConstructorInternal)(ImageBase + Finder::FindAbilitySpecCDOConstructor());
		ConstructorInternal(this, InAbility, InLevel, InInputID, InSourceObject);
	}
	else {
		memset(this, 0, FGameplayAbilitySpec::GetSize());

		Ability = InAbility;
		Level = InLevel;
		InputID = InInputID;
		SourceObject = InSourceObject;
		ActiveCount = 0;
		InputPressed = false;
		RemoveAfterActivation = false;
		PendingRemove = false;
		MostRecentArrayReplicationKey = -1;
		ReplicationID = -1;
		ReplicationKey = -1;

		Handle.GenerateNewHandle();
	}
}

TSharedPtr<FAbilityReplicatedDataCache> FGameplayAbilityReplicatedDataContainer::Find(const FGameplayAbilitySpecHandleAndPredictionKey& Key) const
{
	for (const FKeyDataPair& Pair : InUseData)
	{
		if (Pair.Key() == Key)
		{
			return Pair.Value();
		}
	}

	return TSharedPtr<FAbilityReplicatedDataCache>();
}