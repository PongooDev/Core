#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"
#include "Engine/Source/Runtime/Core/Public/Math/Color.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/PrimaryAssetId.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

class UFortAbilitySet;

class UFortItemDefinition;
class UMaterialInterface;
class UParticleSystem;

struct FFortConditionalIncludeTags {
public:
	DefineUnrealStruct(FFortConditionalIncludeTags);

	DefineStructProperty(FGameplayTagContainer, ConditionTags);
	DefineStructProperty(FGameplayTagContainer, IncludeTags);
public:
	uint8 Padding[0x40];
};

struct FFortItemQuantityPair {
public:
	DefineUnrealStruct(FFortItemQuantityPair);

	DefineStructProperty(FPrimaryAssetId, ItemPrimaryAssetId);
	DefineStructProperty(TSoftObjectPtr<UFortItemDefinition>, ItemDefinition);
	DefineStructProperty(int32, Quantity);
public:
	uint8 Padding[0x3C];
};

struct FFortCosmeticModification {
public:
	DefineUnrealStruct(FFortCosmeticModification);

	DefineStructProperty(TSoftObjectPtr<UMaterialInterface>, CosmeticMaterial);
	DefineStructProperty(TSoftObjectPtr<UParticleSystem>, AmbientParticleSystem);
	DefineStructProperty(TSoftObjectPtr<UParticleSystem>, MuzzleParticleSystem);
	DefineStructProperty(TSoftObjectPtr<UParticleSystem>, ReloadParticleSystem);
	DefineStructProperty(TSoftObjectPtr<UParticleSystem>, BeamParticleSystem);
	DefineStructProperty(TSoftClassPtr<UClass>, TracerTemplate);
	DefineStructProperty(bool, bModifyColor);
	DefineStructProperty(FLinearColor, ColorAlteration);
	DefineStructProperty(FName, ColorParameterName);
	DefineStructProperty(bool, bModifyDecalColour);
	DefineStructProperty(FLinearColor, DecalColourAlterationStart);
	DefineStructProperty(FLinearColor, DecalColourAlterationEnd);
	DefineStructProperty(bool, bModifyShellColour);
	DefineStructProperty(FLinearColor, ShellColourAlteration);
public:
	uint8 Padding[0x554];
};

class EFortAlteration {
public:
	DefineUnrealEnum(EFortAlteration);

	DefineEnumProperty(AttributeSlot);
	DefineEnumProperty(GameplaySlot);
	DefineEnumProperty(ComplexCosmeticSlot);
	DefineEnumProperty(UserPickedCosmeticSlot);
	DefineEnumProperty(ColorSlot);
	DefineEnumProperty(HeroSpecializationTier1Slot);
	DefineEnumProperty(HeroSpecializationTier2Slot);
	DefineEnumProperty(HeroSpecializationTier3Slot);
	DefineEnumProperty(HeroSpecializationTier4Slot);
	DefineEnumProperty(HeroSpecializationTier5Slot);
};

class UFortAlterationItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortAlterationItemDefinition);

	DefineUProperty(EFortAlteration, AlterationType);
	DefineUProperty(FFortCosmeticModification, DefaultCosmetic);
	DefineUProperty(TArray<FFortConditionalIncludeTags>, AdditionalGameplayTags);
	DefineUProperty(TSoftObjectPtr<UFortAbilitySet>, AlterationAbilitySet);
	DefineUProperty(TArray<FFortItemQuantityPair>, AdditionalRespecCosts);
};
