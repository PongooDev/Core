#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"

class UGameplayEffect;

struct FGameplayEffectApplicationInfo {
public:
	DefineUnrealStruct(FGameplayEffectApplicationInfo);

	DefineStructProperty(TSoftClassPtr<UClass>, GameplayEffect);
	DefineStructProperty(float, Level);
public:
	uint8 Padding[0x30];
};

struct FGameplayEffectApplicationInfoHard {
public:
	DefineUnrealStruct(FGameplayEffectApplicationInfoHard);

	DefineStructProperty(TSubclassOf<UGameplayEffect>, GameplayEffect);
	DefineStructProperty(float, Level);
public:
	uint8 Padding[0x10];
};