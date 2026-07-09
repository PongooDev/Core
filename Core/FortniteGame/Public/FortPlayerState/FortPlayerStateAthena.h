#pragma once
#include "pch.h"

#include "FortPlayerStateZone.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

#include "FortniteGame/Public/FortPlayer/FortPlayerDeathReport.h"
#include "FortniteGame/Public/Info/DeathInfo.h"
#include "FortniteGame/Public/FortEnums.h"

class AFortTeamInfo;

class AFortPlayerStateAthena final : public AFortPlayerStateZone {
public:
	DefineUnrealClass(AFortPlayerStateAthena);

	DefineUProperty(FDeathInfo, DeathInfo);
	DefineUProperty(uint8, TeamIndex);
	DefineUProperty(int32, SecondsAlive);
	DefineUProperty(int32, KillScore);
	DefineUProperty(int32, TeamKillScore);
	DefineUProperty(int32, DownScore);
	DefineUProperty(int32, Place);
	DefineUProperty(AFortTeamInfo*, PlayerTeam);
	DefineUProperty(bool, bHasWonAGame);
	DefineUProperty(uint8, ReplicatedTeamMemberState);
	DefineUProperty(uint8, TeamMemberState);
	DefineUProperty(float, TeamMemberStateRepTime);
	DefineUProperty(uint8, SquadId);
public:
	void OnRep_TeamKillScore();

	void OnRep_TeamScore();

	void ClientReportDBNO(const AFortPlayerStateAthena* Player);
	void ClientReportDBNO(const FString& DBNOPlayersName);

	void ClientReportKill(const AFortPlayerStateAthena* Player);
	void ClientReportKill(const FString& KilledPlayersName);

	void ClientReportTeamKill(int32 TeamKills);

	void OnRep_Kills();

	void OnRep_DeathInfo();

	void OnRep_Downs();

	void OnRep_Place();

	void OnRep_TeamIndex();

	static uint8 ToDeathCause(const FGameplayTagContainer& InTags, bool bWasDBNO);

	void OnRep_ReplicatedTeamMemberState();

	void OnRep_SquadId();

	static void Hook() {
		Log("AFortPlayerStateAthena Hooked!");
	}
};