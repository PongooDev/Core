#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Components/ActorComponent.h"

class AFortGameplayMutator;
class UPlaylistUserOptions;

class EMutatorListInitState {
public:
	DefineUnrealEnum(EMutatorListInitState);

	DefineEnumProperty(Default);
	DefineEnumProperty(Enabled);
	DefineEnumProperty(Disabled);
};

class UFortMutatorListComponent : public UActorComponent {
public:
	DefineUnrealClass(UFortMutatorListComponent);

	DefineUProperty(bool, bShouldMakeMutatorsDormant);
	DefineUProperty(EMutatorListInitState, InitState);
	DefineUProperty(TArray<TSoftClassPtr<UClass>>, MutatorDefs);
	DefineUProperty(UPlaylistUserOptions*, UserOptions);
	DefineUProperty(TArray<AFortGameplayMutator*>, Mutators);
};
