#include "pch.h"
#include "FortniteGame/Public/FortMutator/FortAthenaMutator_Barrier.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "FortniteGame/Public/BuildingActor/AthenaBarrierObjective.h"
#include "FortniteGame/Public/FortPlaylist/FortPlaylistAthena.h"

void AFortAthenaMutator_Barrier::CheckHealthThreshold(uint8 TeamNum)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("CheckHealthThreshold");

	if (!Func) {
		Log("AFortAthenaMutator_Barrier::CheckHealthThreshold: Failed to find function!");
		return;
	}

	return Call<void>(Func, TeamNum);
}

void AFortAthenaMutator_Barrier::OnMutatorGameplayEvent(int32 EventId, int32 EventParam1, int32 EventParam2, int32 EventParam3)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnMutatorGameplayEvent");

	if (!Func) {
		Log("AFortAthenaMutator_Barrier::OnMutatorGameplayEvent: Failed to find function!");
		return;
	}

	return Call<void>(Func, EventId, EventParam1, EventParam2, EventParam3);
}

void AFortAthenaMutator_Barrier::SpawnModeObjectives()
{
	if (!CachedGameState)
		return;

	AFortGameStateAthena* GameState = (AFortGameStateAthena*)UGameplayStatics::GetGameState(this);
	 
	FAircraftFlightInfo& FlightPath = GameState->FlightPathMidLine;

	FVector WallStart = FlightPath.FlightStartLocation;
	WallStart.Z = 0.0;

	FVector WallEnd = FlightPath.GetFlightEnd();
	WallEnd.Z = 0.0;

	SpawnBarrier(&WallStart, &WallEnd);
}

void AFortAthenaMutator_Barrier::SpawnBarrier(const FVector* WallStart, const FVector* WallEnd)
{
	if (!BigBaseWallClass || !GetWorld())
		return;

	FVector MidPoint = (*WallStart + *WallEnd) * 0.5f;
	FVector GroundLocation = UFortKismetLibrary::FindStaticGroundLocationAt(GetWorld(), MidPoint, nullptr, -9800.0f, 20000.0f);
	float ZLevel = GroundLocation.Z;

	BigBaseWall = AAthenaBigBaseWall::CreateWall(GetWorld(), BigBaseWallClass, *WallStart, *WallEnd, ZLevel);
}

void AFortAthenaMutator_Barrier::SetupTeamStates()
{
	if (!CachedGameState)
		return;

	UFortPlaylistAthena* CurrentPlaylistData = CachedGameState->GetPlaylist();
	if (!CurrentPlaylistData)
		return;

	Team_0_State.TeamNum = CurrentPlaylistData->DefaultFirstTeam;
	Team_0_State.FoodTeam = EBarrierFoodTeam::GetBurger();

	Team_1_State.TeamNum = (uint8)(CurrentPlaylistData->DefaultFirstTeam + 1);
	Team_1_State.FoodTeam = EBarrierFoodTeam::GetTomato();
}

void AFortAthenaMutator_Barrier::BeginPlay(AFortAthenaMutator_Barrier* This)
{
	BeginPlayOG(This);

	This->SetupTeamStates();

	if (!This->BigBaseWall) // bSpawnedModeObjects was added later or its just unreflected ig
		This->SpawnModeObjectives();
}

void AFortAthenaMutator_Barrier::SpawnObjectiveActor(TSubclassOf<AAthenaBarrierFlag> InActorClass, FVector InActorLocation, FRotator InActorRotation, FBarrierTeamState* TeamState)
{
	if (!TeamState)
		return;

	FTransform Transform(FRotator(), InActorLocation, FVector(1, 1, 1));
	auto Flag = (AAthenaBarrierFlag*)GetWorld()->SpawnActor(InActorClass.Class, Transform, this);

	if (!Flag)
		return;

	const uint8 TeamNum = TeamState->TeamNum;
	const EBarrierFoodTeam FoodTeam = TeamState->FoodTeam;

	Flag->Team = TeamNum;
	Flag->TeamIndex = TeamNum;
	Flag->OnRep_TeamVFT();

	Flag->SetFoodTeam(FoodTeam);

	auto Objective = AAthenaBarrierFlag::GetObjectiveActor(Flag);
	if (Objective)
	{
		Objective->Team = TeamNum;
		Objective->TeamIndex = TeamNum;
		Objective->OnRep_TeamVFT();

		Objective->SetFoodTeam(FoodTeam);

		Objective->bAllowDamage = true;
		Objective->HeadRotationYaw = InActorRotation.Yaw;
		Objective->OnRep_HeadRotationYaw();
	}

	TeamState->ObjectiveFlag = Flag;
	TeamState->ObjectiveObject = Objective;
	TeamState->bRespawnEnabled = true;
}

void AFortAthenaMutator_Barrier::OnGamePhaseStepChanged(EAthenaGamePhaseStep GamePhaseStep)
{
	AFortGameStateAthena* GameState = (AFortGameStateAthena*)UGameplayStatics::GetGameState(this);

	if (GamePhaseStep == EAthenaGamePhaseStep::BusLocked)
	{
		FVector Center{};
		UFortKismetLibrary::GetSafeZoneLocation(GetWorld(), 3, &Center);

		FVector Right = BigBaseWall->GetActorRightVector();
		float Dist = ObjectiveDistanceFromWall.Evaluate();
		float ZOff = ObjectiveZOffset.Evaluate();

		FVector Loc0 = Center + Right * Dist;
		FVector Loc1 = Center - Right * Dist;

		FVector Ground0 = UFortKismetLibrary::FindStaticGroundLocationAt(UWorld::GetWorld(), Loc0, nullptr, 20000.0f, -9800.0f);
		FVector Ground1 = UFortKismetLibrary::FindStaticGroundLocationAt(UWorld::GetWorld(), Loc1, nullptr, 20000.0f, -9800.0f);

		constexpr float ObjectiveZOffsetScale = 512.f;

		Ground0.Z += ZOff * ObjectiveZOffsetScale;
		Ground1.Z += ZOff * ObjectiveZOffsetScale;

		FVector Dir01 = Ground1 - Ground0;
		FVector Dir10 = Ground0 - Ground1;

		FRotator Rot0(0.f, FMath::RadiansToDegrees(FMath::Atan2(Dir01.Y, Dir01.X)), 0.f);
		FRotator Rot1(0.f, FMath::RadiansToDegrees(FMath::Atan2(Dir10.Y, Dir10.X)), 0.f);

		SpawnObjectiveActor(ObjectiveFlag, Ground0, Rot0, &Team_0_State);
		SpawnObjectiveActor(ObjectiveFlag, Ground1, Rot1, &Team_1_State);
	}
}

void AFortAthenaMutator_Barrier::execOnGamePhaseStepChanged(AFortAthenaMutator_Barrier* Context, FFrame& Stack) {
	execOnGamePhaseStepChangedOG(Context, Stack);

	struct FortAthenaMutator_Barrier_OnGamePhaseStepChanged
	{
	public:
		EAthenaGamePhaseStep GamePhaseStep;
	};
	FortAthenaMutator_Barrier_OnGamePhaseStepChanged* Parms = (FortAthenaMutator_Barrier_OnGamePhaseStepChanged*)Stack.Locals;

	Context->OnGamePhaseStepChanged(Parms->GamePhaseStep);
}

void AFortAthenaMutator_Barrier::Hook()
{
	if (Finder::FindAActor_BeginPlayVFT()) {
		HookEveryVTableIdx(AFortAthenaMutator_Barrier::StaticClass(), Finder::FindAActor_BeginPlayVFT(), BeginPlay, (LPVOID*)&BeginPlayOG);
	}

	ExecHook("Function /Script/FortniteGame.FortAthenaMutator_Barrier.OnGamePhaseStepChanged", execOnGamePhaseStepChanged, execOnGamePhaseStepChangedOG);
}