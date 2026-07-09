#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class UAthenaGliderItemDefinition;
class UAthenaPickaxeItemDefinition;
class UAthenaCharacterItemDefinition;
class UAthenaHatItemDefinition;
class UAthenaBackpackItemDefinition;
class UAthenaLoadingScreenItemDefinition;
class UAthenaBattleBusItemDefinition;
class UAthenaDanceItemDefinition;
class UAthenaVictoryPoseItemDefinition;

struct FFortAthenaLoadout {
public:
	DefineUnrealStruct(FFortAthenaLoadout);

	DefineStructProperty(FString, BannerIconId);
	DefineStructProperty(FString, BannerColorId);
	DefineStructProperty(UAthenaGliderItemDefinition*, Glider);
	DefineStructProperty(UAthenaPickaxeItemDefinition*, Pickaxe);
	DefineStructProperty(UAthenaCharacterItemDefinition*, Character);
	DefineStructProperty(UAthenaHatItemDefinition*, Hat);
	DefineStructProperty(UAthenaBackpackItemDefinition*, Backpack);
	DefineStructProperty(UAthenaLoadingScreenItemDefinition*, LoadingScreen);
	DefineStructProperty(UAthenaBattleBusItemDefinition*, BattleBus);
	DefineStructProperty(UAthenaDanceItemDefinition*, Dance);
	DefineStructProperty(UAthenaVictoryPoseItemDefinition*, VictoryPose);
public:
	uint8 Padding[0x88];
};