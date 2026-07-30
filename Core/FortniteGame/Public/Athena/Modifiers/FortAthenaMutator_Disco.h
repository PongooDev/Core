#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"
#include "FortniteGame/Public/Athena/Modifiers/ControlPointTypes.h"
#include "FortniteGame/Public/Athena/AthenaGameMessageData.h"

class AAthenaCapturePoint;
class AFortPlayerStateAthena;

class AFortAthenaMutator_Disco : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Disco);

	DefineUProperty(FScalableFloat, GoalScoreToWin);
	DefineUProperty(FScalableFloat, PointAccrualFrequency);
	DefineUProperty(FScalableFloat, LastRespawnableSafeZone);
	DefineUProperty(FScalableFloat, LastSafeZone);
	DefineUProperty(FFortRespawnLogicData, RespawnLogicData);
	DefineUProperty(int32, LastRespawnableSafeZoneIndex);
	DefineUProperty(bool, bRespawnWarningGiven);
	DefineUProperty(int32, LastSafeZoneIndex);
	DefineUProperty(FControlPointAssetData, ControlPointAssets);
	DefineUProperty(TArray<FControlPointSpawnData>, ControlPointSpawnData);
	DefineUProperty(TArray<FControlPointInstanceData>, SpawnedControlPoints);
	DefineUProperty(TArray<AFortPlayerStateAthena*>, DancingPlayerList);
	DefineUProperty(TArray<AFortPlayerStateAthena*>, PrevDancingPlayerList);
	DefineUProperty(float, TimeUntilPointAccrual);
	DefineUProperty(int32, SentDiscoOpenedMessagesIndex);
	DefineUProperty(FAthenaGameMessageData, GameMsg_Intro);
	DefineUProperty(FAthenaGameMessageData, GameMsg_FirstCapturePointsEnabled);
	DefineUProperty(FAthenaGameMessageData, GameMsg_SecondCapturePointsEnabled);
	DefineUProperty(FAthenaGameMessageData, GameMsg_ThirdCapturePointsEnabled);
	DefineUProperty(FAthenaGameMessageData, GameMsg_LastCapturePointsEnabled);
	DefineUProperty(FAthenaGameMessageData, GameMsg_CapturePointsEnabled);
	DefineUProperty(FAthenaGameMessageData, GameMsg_CapturePointsDisabled);
	DefineUProperty(FAthenaGameMessageData, GameMsg_EnemyCapturedAllPoints);
	DefineUProperty(FAthenaGameMessageData, GameMsg_AlliesCapturedAllPoints);
	DefineUProperty(FAthenaGameMessageData, GameMsg_AllyCapturePointLost);
	DefineUProperty(FAthenaGameMessageData, GameMsg_EnemyAboutToWin);
	DefineUProperty(FAthenaGameMessageData, GameMsg_AlliesAboutToWin);
	DefineUProperty(FAthenaGameMessageData, GameMsg_NoMoreRespawnsWarning);
	DefineUProperty(FAthenaGameMessageData, GameMsg_NoMoreRespawns);
public:
	bool IsRespawningAllowed();

	void OnCaptureLock(AAthenaCapturePoint* CapturePoint, uint8 LockTeam, bool bIsNewLock);

	void OnCaptureLockBroken(AAthenaCapturePoint* CapturePoint, uint8 LockTeam, uint8 BreakTeam);

	void OnGamePhaseChanged(uint8 GamePhase);

	void OnGamePhaseStepChanged(uint8 GamePhaseStep);

	void OnMutatorGameplayEvent(int32 EventId, int32 EventParam1, int32 EventParam2, int32 EventParam3);
};
