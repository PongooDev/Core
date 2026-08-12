#include "pch.h"
#include "FortniteGame/Public/FortGameStateAthena.h"

#include "FortniteGame/Public/Heroes/FortHero.h"
#include "FortniteGame/Public/Heroes/FortHeroSpecialization.h"
#include "FortniteGame/Public/Heroes/FortHeroType.h"
#include "FortniteGame/Public/Playlists/FortPlaylistAthena.h"
#include "FortniteGame/Public/Playlists/FortPlaylistManager.h"
#include "FortniteGame/Public/Player/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortGameModeAthena.h"
#include "FortniteGame/Public/Athena/FortSafeZoneIndicator.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_Heist.h"
#include "FortniteGame/Public/Athena/FortAthenaAircraft.h"

void AFortGameStateAthena::OnRep_CurrentPlaylistId()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_CurrentPlaylistId"));

	Call(Func);
}

void AFortGameStateAthena::OnPlaylistDataLoadCompleted() {
	void (*OnPlaylistDataLoadCompletedInternal)(AFortGameStateAthena*) = decltype(OnPlaylistDataLoadCompletedInternal)(ImageBase + Finder::FindAFortGameStateAthena_OnPlaylistDataLoadCompleted());
	OnPlaylistDataLoadCompletedInternal(this);
}

void AFortGameStateAthena::UpdatePlaylistDependentData() {
	void (*UpdatePlaylistDependentDataInternal)(AFortGameStateAthena*) = decltype(UpdatePlaylistDependentDataInternal)(ImageBase + Finder::FindAFortGameStateAthena_UpdatePlaylistDependentData());
	UpdatePlaylistDependentDataInternal(this);
}

void AFortGameStateAthena::SetCurrentPlaylistId(int InPlaylistId) {
	if (!UFortPlaylistAthena::StaticClass() || !UFortPlaylistAthena::GetDefaultObj()->_HasAirCraftBehavior()) {
		if (InPlaylistId == 50 || InPlaylistId == 11)
			AirCraftBehavior = EAirCraftBehavior::GetOpposingAirCraftForEachTeam();

		bDrawSafeZoneFinalPosIconEnabled = (InPlaylistId == 11 || InPlaylistId == 26 || InPlaylistId == 50);
	}

	bStormReachedFinalPosition = false;

	if (CurrentPlaylistId == InPlaylistId && (bPlaylistDataIsLoaded || bPlaylistDataIsActivelyLoading))
		return;

	CurrentPlaylistId = InPlaylistId;
	OnRep_CurrentPlaylistId();

	UFortPlaylistManager* PlaylistManager = UFortPlaylistManager::Get();
	if (!PlaylistManager)
		return;

	UFortPlaylistAthena* Playlist = PlaylistManager->GetPlaylist(InPlaylistId);
	if (Playlist) {
		if (_HasCurrentPlaylistInfo()) {
			CurrentPlaylistInfo.SetBasePlaylist(Playlist);
			CurrentPlaylistInfo.MarkArrayDirty();
		}
		else if (_HasCurrentPlaylistData()) {
			CurrentPlaylistData = Playlist;
		}

		if (_HasTeamSize()) {
			TeamSize = Playlist->MaxTeamSize;
		}

		if (_HasTeamCount()) {
			TeamCount = Playlist->MaxTeamCount;
		}

		if (_HasCachedSafeZoneStartUp() && Playlist->_HasSafeZoneStartUp()) {
			CachedSafeZoneStartUp = Playlist->SafeZoneStartUp;
		}

		if (Playlist->_HasAirCraftBehavior())
			AirCraftBehavior = Playlist->AirCraftBehavior;

		if (_HasbUseSameDirectionForOpposingAircraft() && Playlist->_HasbUseSameDirectionForOpposingAircraft())
			bUseSameDirectionForOpposingAircraft = Playlist->bUseSameDirectionForOpposingAircraft;

		if (Playlist->AISettings) {
			if (AFortGameModeAthena* GameMode = GetWorld()->AuthorityGameMode->Cast<AFortGameModeAthena>())
				GameMode->OverrideAISettings(Playlist->AISettings);
		}

		TeamCount = Playlist->MaxTeamCount;

		if (!bPlaylistDataIsLoaded && !bPlaylistDataIsActivelyLoading) {
			InitializePlaylistDataPreDataLoad();
			LoadCurrentPlaylistData();
		}
	}
}

void AFortGameStateAthena::ApplyHomebaseEffectsOnPlayerSetup(AFortGameStateAthena* This, FUniqueNetIdRepl* SourceAccountID, UFortMcpProfileCampaign* McpProfile, IAbilitySystemInterface* AbilityObject, UFortHero* Hero, bool bApplyTeamEffect, bool bApplyTeamEffectToOtherPlayers, bool bIgnoreStatClamp)
{
	if (Version::Fortnite_Version > 2.42) {
		return ApplyHomebaseEffectsOnPlayerSetupOG(This, SourceAccountID, McpProfile, AbilityObject, Hero, bApplyTeamEffect, bApplyTeamEffectToOtherPlayers, bIgnoreStatClamp);
	}

	UFortHeroType* ItemDefinition = StaticLoadObject<UFortHeroType>("/Game/Athena/Heroes/HID_Commando_Athena_01.HID_Commando_Athena_01");

	static TArray<UObject*> HeroTypes = FUObjectArray::GetObjectsOfClass(UFortHeroType::StaticClass(), "Athena");
	if (HeroTypes.Num() > 0)
	{
		ItemDefinition = (UFortHeroType*)HeroTypes[UKismetMathLibrary::RandomIntegerInRange(0, HeroTypes.Num() - 1)];
	}

	if (ItemDefinition)
	{
		Hero->ItemDefinition = ItemDefinition;
		Log("AFortGameStateAthena::ApplyHomebaseEffectsOnPlayerSetup: Set hero item definition to " + ItemDefinition->GetName().ToString());
	}

	ApplyHomebaseEffectsOnPlayerSetupOG(This, SourceAccountID, McpProfile, AbilityObject, Hero, bApplyTeamEffect, bApplyTeamEffectToOtherPlayers, bIgnoreStatClamp);
}

void AFortGameStateAthena::OnRep_WinningPlayerName()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_WinningPlayerName");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortGameStateAthena::OnRep_WinningTeam()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_WinningTeam");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortGameStateAthena::OnRep_WinningPlayerList()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_WinningPlayerList");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortGameStateAthena::OnRep_WinningPlayerState()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_WinningPlayerState");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortGameStateAthena::OnRep_CurrentPlaylistData()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_CurrentPlaylistData");

	Call(Func);
}

void AFortGameStateAthena::OnRep_CurrentPlaylistInfo()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_CurrentPlaylistInfo");

	Call(Func);
}

void AFortGameStateAthena::OnRep_Aircraft() {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_Aircraft");

	Call(Func);
}

UDataTable* AFortGameStateAthena::GetLootTierData() {
	UFortPlaylistAthena* Playlist = GetPlaylist();
	if (!Playlist) {
		return nullptr;
	}

	return Playlist->GetLootTierData();
}

UDataTable* AFortGameStateAthena::GetLootPackages() {
	UFortPlaylistAthena* Playlist = GetPlaylist();
	if (!Playlist) {
		return nullptr;
	}

	return Playlist->GetLootPackages();
}

UCurveTable* AFortGameStateAthena::GetGameData() {
	UFortPlaylistAthena* Playlist = GetPlaylist();
	if (!Playlist) {
		return nullptr;
	}

	return Playlist->GetGameData();
}

UFortPlaylistAthena* AFortGameStateAthena::GetPlaylist() {
	if (CurrentPlaylistData) {
		return CurrentPlaylistData;
	}

	if (CurrentPlaylistInfo.BasePlaylist) {
		return CurrentPlaylistInfo.BasePlaylist;
	}

	return nullptr;
}

void AFortGameStateAthena::LoadCurrentPlaylistData() {
	if (Finder::FindAFortGameStateAthena_LoadCurrentPlaylistData()) {
		void (*Fn)(AFortGameStateAthena*) = decltype(Fn)(ImageBase + Finder::FindAFortGameStateAthena_LoadCurrentPlaylistData());
		return Fn(this);
	}
}

void AFortGameStateAthena::InitializePlaylistDataPreDataLoad() {
	
}

float AFortGameStateAthena::GetServerWorldTimeSeconds() {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetServerWorldTimeSeconds");

	if (!Func) {
		return UGameplayStatics::GetTimeSeconds(UWorld::GetWorld());
	}

	return Call<float>(Func);
}

uint8 AFortGameStateAthena::GetGamePhaseStep(float& OutTimeRemaining) {
	uint8 Step = EAthenaGamePhaseStep::GetNone();
	OutTimeRemaining = 0.0f;

	const float Now = GetServerWorldTimeSeconds();

	const uint8 Phase = GamePhase;

	if (Phase == EAthenaGamePhase::GetSetup()) {
		Step = EAthenaGamePhaseStep::GetSetup();
	}
	else if (Phase == EAthenaGamePhase::GetWarmup()) {
		if (_HasWarmupCountdownStartTime() && WarmupCountdownStartTime < 0.0f) {
			return EAthenaGamePhaseStep::GetSetup();
		}

		if (_HasWarmupCountdownEndTime() && WarmupCountdownEndTime > Now) {
			OutTimeRemaining = (float)(int32)(WarmupCountdownEndTime - Now);
			Step = OutTimeRemaining <= 10.0f
				? EAthenaGamePhaseStep::GetGetReady()
				: EAthenaGamePhaseStep::GetWarmup();
		}
		else {
			Step = EAthenaGamePhaseStep::GetGetReady();
		}
	}
	else if (Phase == EAthenaGamePhase::GetAircraft()) {
		Step = EAthenaGamePhaseStep::GetBusFlying();

		for (AFortAthenaAircraft* Aircraft : Aircrafts) {
			if (!Aircraft || !Aircraft->_HasDropStartTime())
				continue;

			const float DropStartTime = Aircraft->DropStartTime;
			if (DropStartTime > Now) {
				Step = EAthenaGamePhaseStep::GetBusLocked();
				OutTimeRemaining = (std::max)((DropStartTime - Now) + 1.0f, OutTimeRemaining);
			}
		}
	}
	else if (Phase == EAthenaGamePhase::GetSafeZones()) {
		if (_HasbIsInFinalCountdown() && bIsInFinalCountdown) {
			return EAthenaGamePhaseStep::GetFinalCountdown();
		}

		if (_HasbIsInCountdown() && bIsInCountdown) {
			return EAthenaGamePhaseStep::GetCountdown();
		}

		if (SafeZoneIndicator) {
			const float StartShrinkTime = SafeZoneIndicator->SafeZoneStartShrinkTime;
			const float FinishShrinkTime = SafeZoneIndicator->SafeZoneFinishShrinkTime;

			if (StartShrinkTime - Now > 0.0f) {
				Step = EAthenaGamePhaseStep::GetStormHolding();
				OutTimeRemaining = StartShrinkTime - Now;
			}
			else if (FinishShrinkTime - Now > 0.0f) {
				Step = EAthenaGamePhaseStep::GetStormShrinking();
				OutTimeRemaining = FinishShrinkTime - Now;
			}
			else {
				Step = EAthenaGamePhaseStep::GetStormHolding();
			}
		}
		else {
			Step = EAthenaGamePhaseStep::GetStormForming();
			if (_HasSafeZonesStartTime() && SafeZonesStartTime > Now) {
				OutTimeRemaining = SafeZonesStartTime - Now;
			}
		}
	}
	else if (Phase == EAthenaGamePhase::GetEndGame()) {
		Step = EAthenaGamePhaseStep::GetEndGame();
		if (_HasEndGameKickPlayerTime() && EndGameKickPlayerTime > Now) {
			OutTimeRemaining = EndGameKickPlayerTime - Now;
		}
	}

	return Step;
}

void AFortGameStateAthena::UpdateGamePhaseStep() {
	if (!_HasGamePhaseStep() || Version::Fortnite_Version <= 1.72)
		return;

	AFortGameModeAthena* FortGMAthena = AuthorityGameMode->Cast<AFortGameModeAthena>();
	if (!FortGMAthena) {
		return; // the gamemode should be athena
	}

	float TimeRemaining = 0.0f;
	const uint8 NewStep = GetGamePhaseStep(TimeRemaining);

	if (_HasGamePhaseStepTimeRemaining()) {
		GamePhaseStepTimeRemaining = TimeRemaining;
	}

	if (NewStep == GamePhaseStep)
		return;

	Log("AFortGameStateAthena::UpdateGamePhaseStep: GamePhaseStep " + std::to_string((uint8)GamePhaseStep) + " -> " + std::to_string((uint8)NewStep));
	GamePhaseStep = NewStep;
	ForceNetUpdate();

	bool bBroadcast = false;
	if (_HasGamePhaseStepChanged() && GamePhaseStepChanged.IsBound()) {
		struct { uint8 NewGamePhaseStep; } Params{ NewStep };

		GamePhaseStepChanged.ProcessMulticastDelegate(&Params);
		bBroadcast = true;
	}

	if (!bBroadcast) {
		for (AFortAthenaMutator* Mutator : GameplayMutators) {
			if (!Mutator)
				continue;

			AFortAthenaMutator_Heist* HeistMutator = Mutator->Cast<AFortAthenaMutator_Heist>();
			if (HeistMutator) {
				HeistMutator->OnGamePhaseStepChanged(NewStep);
			}
		}
	}

	FortGMAthena->UpdateSpawnActorListDuringSafeZone((EAthenaGamePhaseStep)NewStep);
}

FVector AFortGameStateAthena::GetSafeZoneCenter() {
	if (!SafeZoneIndicator)
		return *FVector::Allocate();

	return SafeZoneIndicator->GetSafeZoneCenter();
}

void AFortGameStateAthena::Tick(AFortGameStateAthena* This, float DeltaSeconds) {
	TickOG(This, DeltaSeconds);
	This->UpdateGamePhaseStep();
}

void AFortGameStateAthena::Hook() {
	HookEveryVTableIdx(
		AFortGameStateAthena::StaticClass(),
		Finder::FindAFortGameState_ApplyHomebaseEffectsOnPlayerSetupVFT(),
		ApplyHomebaseEffectsOnPlayerSetup,
		(LPVOID*)&ApplyHomebaseEffectsOnPlayerSetupOG
	);

	if (Finder::FindAActor_TickVFT()) {
		MH_CreateHook(
			(LPVOID)(GetOffsetFromVTable(AFortGameStateAthena::GetDefaultObj(), Finder::FindAActor_TickVFT())),
			Tick,
			(LPVOID*)&TickOG
		);
	}

	Log("Hooked AFortGameStateAthena");
}
