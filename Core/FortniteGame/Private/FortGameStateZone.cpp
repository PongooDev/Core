#include "pch.h"
#include "FortniteGame/Public/FortGameStateZone.h"

#include "FortniteGame/Public/Heroes/FortHeroType.h"
#include "FortniteGame/Public/Heroes/FortHero.h"
#include "FortniteGame/Public/FortGameStateAthena.h"
#include "FortniteGame/Public/Playlists/FortPlaylistAthena.h"
#include "FortniteGame/Public/Player/FortPlayerStateAthena.h"
#include "FortniteGame/Public/Player/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/Athena/FortSafeZoneIndicator.h"

void AFortGameStateZone::OnRep_GameDifficulty()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_GameDifficulty");

	Call(Func);
}

bool AFortGameStateZone::IsRespawningAllowed(AFortPlayerState* PlayerState)
{
	if (Version::Fortnite_Version > 6.22) {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("IsRespawningAllowed");

		if (!Func) {
			return false;
		}

		return Call<bool>(Func, PlayerState);
	}
	else {
		AFortGameStateAthena* GameStateAthena = Cast<AFortGameStateAthena>();
		if (GameStateAthena) {
			AFortPlayerStateAthena* PlayerStateAthena = PlayerState->Cast<AFortPlayerStateAthena>();
			if (PlayerStateAthena) {
				UFortPlaylistAthena* Playlist = GameStateAthena->GetPlaylist();
				if (Playlist) {
					if (Playlist->RespawnType == EAthenaRespawnType::GetInfiniteRespawn()) {
						return true;
					}

					if (Playlist->RespawnType == EAthenaRespawnType::GetInfiniteRespawnExceptStorm()) {
						AFortSafeZoneIndicator* SafeZoneIndicator = GameStateAthena->SafeZoneIndicator;
						if (!SafeZoneIndicator) {
							return true;
						}

						if (!PlayerStateAthena->DeathInfo.bInitialized) {
							return false;
						}

						if (SafeZoneIndicator->IsInCurrentSafeZone(PlayerStateAthena->DeathInfo.DeathLocation)) {
							return true;
						}
					}
				}
			}

			return false;
		}

		return true;
	}
}

void AFortGameStateZone::Hook() {
	Log("Hooked AFortGameStateZone");
}