#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/Core/Public/Math/Color.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/OnlineReplStructs.h"
#include "FortniteGame/Public/Building/BuildingGameplayActor.h"

class AFortPlayerStart;
class USoundCue;

class ESpawnMachineState {
public:
	DefineUnrealEnum(ESpawnMachineState);

	DefineEnumProperty(Default);
	DefineEnumProperty(WaitingForUse);
	DefineEnumProperty(Active);
	DefineEnumProperty(Complete);
	DefineEnumProperty(OnCooldown);
};
class ESpawnMachineSubTextState {
public:
	DefineUnrealEnum(ESpawnMachineSubTextState);

	DefineEnumProperty(NoCards);
	DefineEnumProperty(VanInUse);
	DefineEnumProperty(None);
};

class ABuildingGameplayActorSpawnMachine : public ABuildingGameplayActor {
public:
	DefineUnrealClass(ABuildingGameplayActorSpawnMachine);

	DefineUProperty(AFortPlayerStart*, ResurrectLocation);
	DefineUProperty(TArray<FUniqueNetIdRepl>, PlayerIdsForResurrection);
	DefineUProperty(uint8, SquadId);
	DefineUProperty(uint8, ActiveTeam);
	DefineUProperty(ESpawnMachineState, SpawnMachineState);
	DefineUProperty(FScalableFloat, ResurrectionStartDelay);
	DefineUProperty(FScalableFloat, ResurrectionNextPlayerDelay);
	DefineUProperty(USoundCue*, InteractSoundCueLoop);
	DefineUProperty(FScalableFloat, CooldownLengthRow);
	DefineUProperty(FText, InteractNoCardsSubText);
	DefineUProperty(FText, InteractNoVanLockSubText);
	DefineUProperty(FLinearColor, InteractSubTextColor);
	DefineUProperty(ESpawnMachineSubTextState, SpawnMachineSubTextState);
	DefineUProperty(int32, IndexIntoGameState);
};
