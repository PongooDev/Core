#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Character.h"

#include "FortniteGame/Public/FortEnums.h"

class AFortWeapon;
class UFortWeaponItemDefinition;
class UFortItemDefinition;
class AFortPickup;
struct FFortPickupRequestInfo;
class UCustomCharacterPart;
class UFortHealthSet;

class AFortPawn : public ACharacter {
public:
	DefineUnrealClass(AFortPawn);

	DefineUProperty(AFortWeapon*, CurrentWeapon);
	DefineUProperty(TArray<AFortWeapon*>, CurrentWeaponList);

	DefineBitfieldUProperty(bMovingEmote);
	DefineBitfieldUProperty(bMovingEmoteForwardOnly);
	DefineBitfieldUProperty(bMovingEmoteFollowingOnly);
	DefineBitfieldUProperty(bMovingEmoteSkipLandingFX);

	DefineUProperty(int32, EmoteCount);
	DefineUProperty(float, LastEmoteTime);
	DefineUProperty(float, LastEmoteEndTime);

	DefineUProperty(UFortItemDefinition*, LastEmoteItemDef);
	DefineUProperty(UFortItemDefinition*, LastReplicatedEmoteExecuted);
	DefineUProperty(bool, bFireBlockedByEmoteCooldown);

	DefineUProperty(float, EmoteToFireCooldownTime);
	DefineUProperty(float, EmoteWalkSpeed);

	DefineUProperty(TArray<AFortPickup*>, IncomingPickups);

	DefineUProperty(UFortHealthSet*, HealthSet);
	DefineBitfieldUProperty(bIsDBNO);
	DefineBitfieldUProperty(bWasDBNOOnDeath);
	DefineUProperty(bool, bPlayedDying);
	DefineUProperty(bool, bIsDying);
public:
	AFortWeapon* EquipWeaponDefinition(const UFortWeaponItemDefinition* WeaponData, const FGuid& ItemEntryGuid);

	void OnRep_CurrentWeapon(AFortWeapon* OldWeapon);

	void SetHealth(float NewHealthVal);

	void SetMaxHealth(float NewHealthVal);

	void SetShield(float NewShieldValue);

	void SetMaxShield(float NewValue);

	float GetMaxHealth() const;

	float GetMaxShield() const;

	AFortWeapon* FindWeapon(UFortItemDefinition* ItemDef);
	AFortWeapon* FindWeapon(FGuid ItemDef);

	void OnRep_IsDBNO();

	bool IsDBNO() const;
};