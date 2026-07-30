#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"

class AFortGameplayMutator;
class UPlaylistUserOptions;

class UObject;

struct FPropertyOverrideMk2 : public FFastArraySerializerItem {
public:
	DefineUnrealStruct(FPropertyOverrideMk2);

	DefineStructProperty(FString, PropertyScope);
	DefineStructProperty(FString, PropertyName);
	DefineStructProperty(FString, PropertyData);
	DefineStructProperty(FString, DefaultPropertyData);
public:
	uint8 Padding[0x50];
};

class EPropertyOverrideTargetType {
public:
	DefineUnrealEnum(EPropertyOverrideTargetType);

	DefineEnumProperty(None);
	DefineEnumProperty(Default);
	DefineEnumProperty(ImmutableTarget);
};

struct FPropertyOverrideData : public FFastArraySerializer {
public:
	DefineUnrealStruct(FPropertyOverrideData);

	DefineStructProperty(TArray<FPropertyOverrideMk2>, PropertyOverrides);
	DefineStructProperty(int32, TargetObjectReplicationKey);
	DefineStructProperty(EPropertyOverrideTargetType, OverrideMode);
	DefineStructProperty(UObject*, BaseObject);
	DefineStructProperty(UObject*, MutableObject);
public:
	uint8 Padding[0x130];
};

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
	DefineUProperty(FPropertyOverrideData, PropertyOverrideArray);
};
