#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

class AActor;
class UActorComponent;
class UAssetUserData;
class ULevel;
class UWorld;
class UPrimitiveComponent;

class UActorComponent : public UObject {
public:
	DefineUnrealClass(UActorComponent);
public:
	AActor* GetOwner() const;
};

FORCEINLINE AActor* UActorComponent::GetOwner() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetOwner"));

	return const_cast<UActorComponent*>(this)->Call<AActor*>(Func);
}