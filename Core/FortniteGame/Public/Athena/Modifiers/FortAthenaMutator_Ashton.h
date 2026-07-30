#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class UFortGadgetItemDefinition;
class UFortWorldItemDefinition;

class AFortAthenaMutator_Ashton : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Ashton);

	DefineUProperty(FScalableFloat, GameOverDelay);
	DefineUProperty(TArray<FAshtonStoneState>, StoneList);
	DefineUProperty(TArray<FAshtonStoneState>, CachedStoneList);
	DefineUProperty(FVector, VillainLeaderLocationForSnapSound);
	DefineUProperty(FVector, VillainLeaderLocationForDeathSound);
	DefineUProperty(FVector, VillainLeaderLocationForSound);
	DefineUProperty(FGameplayTag, JimInventoryTag);
	DefineUProperty(FGameplayTag, JimPickupTag);
	DefineUProperty(FGameplayTag, VillainTeamTag);
	DefineUProperty(FGameplayTag, HeroTeamTag);
	DefineUProperty(FGameplayTagContainer, ForceEquipTags);
	DefineUProperty(FSlateBrush, MapEnemyBrush);
	DefineUProperty(FVector2D, MapEnemyScale);
	DefineUProperty(AFortPlayerController*, AutoEquipController);
	DefineUProperty(UFortGadgetItemDefinition*, VillainLeaderItemDef);
	DefineUProperty(TArray<UFortWorldItemDefinition*>, VillainItemDefs);
	DefineUProperty(AFortPlayerControllerAthena*, VillainLeaderPC);
	DefineUProperty(FScalableFloat, NumVillainRespawns);
	DefineUProperty(int32, NumVillainRespawnsRemaining);
	DefineUProperty(FScalableFloat, PromoteNextVillainLeaderDelay);
	DefineUProperty(FScalableFloat, VillainsDisplayStoneIcons);
	DefineUProperty(int32, CachedNumCapturedStones);
	DefineUProperty(FAthenaGameMessageData, GameMsg_IntroVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_IntroHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_RespawnVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_RespawnHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_HeroFollowMap);
	DefineUProperty(FAthenaGameMessageData, GameMsg_FirstStoneComingVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_FirstStoneComingHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_StoneComingVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_StoneComingHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_FirstStoneCapturedVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_FirstStoneCapturedHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_StoneCapturedVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_StoneCapturedHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_OneStoneLeftVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_OneStoneLeftHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_FinalStoneComingVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_FinalStoneComingHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_EliminatedThanosVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_EliminatedThanosHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_RespawnThanosVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_RespawnThanosHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_StonesCompleteVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_StonesCompleteHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_VillainArmyDepletedVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_VillainArmyDepletedHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_NoMoreRespawns);
};
