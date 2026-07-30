#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/AthenaCosmeticItemDefinition.h"

class UNiagaraSystem;
class UParticleSystem;

class UAthenaSkyDiveContrailItemDefinition : public UAthenaCosmeticItemDefinition {
public:
	DefineUnrealClass(UAthenaSkyDiveContrailItemDefinition);

	DefineUProperty(TSoftObjectPtr<UParticleSystem>, ContrailEffect);
	DefineUProperty(TSoftObjectPtr<UNiagaraSystem>, NiagaraContrailEffect);
};
