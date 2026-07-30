#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/AthenaCosmeticItemDefinition.h"

class UParticleSystem;
class USkeletalMesh;
class USoundBase;

class EFortGliderType {
public:
	DefineUnrealEnum(EFortGliderType);

	DefineEnumProperty(Glider);
	DefineEnumProperty(Umbrella);
};

class UAthenaGliderItemDefinition : public UAthenaCosmeticItemDefinition {
public:
	DefineUnrealClass(UAthenaGliderItemDefinition);

	DefineUProperty(EFortGliderType, GliderType);
	DefineUProperty(bool, bActivateTrailsOnRotationalMovement);
	DefineUProperty(TSoftClassPtr<UClass>, ParachutePrefabClass);
	DefineUProperty(TSoftObjectPtr<USkeletalMesh>, SkeletalMesh);
	DefineUProperty(TSoftClassPtr<UClass>, AnimClass);
	DefineUProperty(TSoftObjectPtr<UParticleSystem>, TrailParticles);
	DefineUProperty(TSoftObjectPtr<UParticleSystem>, OwnerTrailParticles);
	DefineUProperty(FName, TrailParamName);
	DefineUProperty(TSoftObjectPtr<USoundBase>, OpenSound);
	DefineUProperty(TSoftObjectPtr<USoundBase>, CloseSound);
};
