#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "GameplayAbilitySpecHandle.h"

class UAbilitySystemComponent;
class UGameplayAbility;

struct FGameplayAbilityActivationInfo {
public:
	DefineUnrealStruct(FGameplayAbilityActivationInfo);
};

struct FGameplayAbilitySpec : public FFastArraySerializerItem {
public:
	DefineUnrealStruct(FGameplayAbilitySpec);

	DefineStructProperty(FGameplayAbilitySpecHandle, Handle);

	DefineStructProperty(UGameplayAbility*, Ability);

	DefineBitfieldStructProperty(InputPressed);

	DefineStructProperty(int32, Level);

	DefineStructProperty(int32, InputID);

	DefineStructProperty(UObject*, SourceObject);

	DefineStructProperty(uint8, ActiveCount);

	DefineBitfieldStructProperty(RemoveAfterActivation);

	DefineBitfieldStructProperty(PendingRemove);

	DefineStructProperty(FGameplayAbilityActivationInfo, ActivationInfo)
public:
	uint8 Padding[0xBC];
public:
	FGameplayAbilitySpec()
	{
		static FGameplayAbilitySpec* (*ConstructorInternal)(FGameplayAbilitySpec*) = decltype(ConstructorInternal)(ImageBase + Finder::FindAbilitySpecDefaultConstructor());
		ConstructorInternal(this);
	}

	FGameplayAbilitySpec(TSubclassOf<UGameplayAbility> InAbilityClass, int32 InLevel = 1, int32 InInputID = INDEX_NONE, UObject* InSourceObject = nullptr);

	static FGameplayAbilitySpec* ConstructAbilitySpec(UGameplayAbility* InAbility, int32 InLevel = 1, int32 InInputID = INDEX_NONE, UObject* InSourceObject = nullptr);
};

struct FGameplayAbilitySpecContainer : public FFastArraySerializer {
public:
	DefineUnrealStruct(FGameplayAbilitySpecContainer);

	DefineStructProperty(TArray<FGameplayAbilitySpec>, Items);
	DefineStructProperty(UAbilitySystemComponent*, Owner);
};