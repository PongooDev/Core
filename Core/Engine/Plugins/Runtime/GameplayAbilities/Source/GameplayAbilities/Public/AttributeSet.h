#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"

class UProperty;
class UStruct;

struct FGameplayAttribute {
public:
	DefineUnrealStruct(FGameplayAttribute);

	DefineStructProperty(FString, AttributeName);
	DefineStructProperty(UProperty*, Attribute);
	DefineStructProperty(UStruct*, AttributeOwner);
public:
	uint8 Padding[0x20];
};

struct FGameplayAttributeData {
public:
	DefineUnrealStruct(FGameplayAttributeData);

	FGameplayAttributeData()
		: BaseValue(0.f)
		, CurrentValue(0.f)
	{
	}

	FGameplayAttributeData(float DefaultValue)
		: BaseValue(DefaultValue)
		, CurrentValue(DefaultValue)
	{
	}

public:
	void** VTable;
public:
	float BaseValue;

	float CurrentValue;
};

class UAttributeSet : public UObject {
public:
	DefineUnrealClass(UAttributeSet);
public:
	FORCEINLINE AActor* GetOwningActor() const { return GetTypedOuter<AActor>(); }
};
