#include "pch.h"
#include "FortniteGame/Public/Playlists/FortPlaylist.h"

#include "FortniteGame/Public/Playlists/FortPlaylistAthena.h"
#include "FortniteGame/Public/FortGameState.h"
#include "FortniteGame/Public/World/AdditionalLevelStreamed.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/LevelStreaming.h"

int32& UFortPlaylist::GetPlaylistId() {
	if (UProperty* PlaylistIdProperty = UFortPlaylistAthena::StaticClass()->FindPropertyByName("PlaylistId")) {
		return *(int32*)((uintptr_t)this + PlaylistIdProperty->Offset_Internal);
	}

	return PlaylistId;
}

FName& UFortPlaylist::GetPlaylistName() {
	if (UProperty* PlaylistNameProperty = UFortPlaylistAthena::StaticClass()->FindPropertyByName("PlaylistName")) {
		return *(FName*)((uintptr_t)this + PlaylistNameProperty->Offset_Internal);
	}

	return PlaylistName;
}

UDataTable* UFortPlaylist::GetLootTierData() {
	return LootTierData.Get();
}

UDataTable* UFortPlaylist::GetLootPackages() {
	return LootPackages.Get();
}

UCurveTable* UFortPlaylist::GetGameData() {
	if (GameData.Get()) {
		return GameData.Get();
	}

	return nullptr;
}

static void AddStreamedPlaylistLevel(AFortGameState* FortGameState, const FName& LevelName, bool bIsServerOnly) {
	if (!FortGameState) {
		return;
	}

	if (FAdditionalLevelStreamed::StaticStruct()) {
		FAdditionalLevelStreamed LevelStreamed{};
		LevelStreamed.LevelName = LevelName;
		LevelStreamed.bIsServerOnly = bIsServerOnly;

		FortGameState->AdditionalPlaylistLevelsStreamed.Add(LevelStreamed, FAdditionalLevelStreamed::GetSize());
	}
	else {
		reinterpret_cast<TArray<FName>&>(FortGameState->AdditionalPlaylistLevelsStreamed).Add(LevelName);
	}
}

void UFortPlaylist::LoadAdditionalLevels() {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		return;
	}

	AFortGameState* FortGameState = World->GetGameState<AFortGameState>();
	if (!FortGameState) {
		Log("UFortPlaylist::LoadAdditionalLevels: GameState is null");
		return;
	}

	if (AdditionalLevels.Num() == 0) {
		Log("UFortPlaylist::LoadAdditionalLevels: No AdditionalLevels to load for this playlist.");
		return;
	}

	Log(std::format("Loading {} AdditionalLevels", AdditionalLevels.Num()));

	for (auto& Level : AdditionalLevels)
	{
		std::string LevelPath = Level.ObjectID.AssetPathName.ToString().ToString();
		if (LevelPath == "None") {
			continue;
		}

		Log("Loading AdditionalLevels: " + LevelPath);
		bool Success = false;
		ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(World, Level, FVector(), FRotator(), &Success, FString());
		if (Success) {
			Log("Successfully loaded AdditionalLevels: " + LevelPath);
			AddStreamedPlaylistLevel(FortGameState, Level.ObjectID.AssetPathName, false);
			FortGameState->OnFinishedStreamingAdditionalPlaylistLevel();
		}
	}
}

void UFortPlaylist::LoadAdditionalLevelsServerOnly() {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		return;
	}

	AFortGameState* FortGameState = World->GetGameState<AFortGameState>();
	if (!FortGameState) {
		Log("UFortPlaylist::LoadAdditionalLevelsServerOnly: GameState is null");
		return;
	}

	if (AdditionalLevelsServerOnly.Num() == 0) {
		Log("UFortPlaylist::LoadAdditionalLevelsServerOnly: No AdditionalLevelsServerOnly to load for this playlist.");
		return;
	}

	Log(std::format("Loading {} AdditionalLevelsServerOnly", AdditionalLevelsServerOnly.Num()));

	for (auto& Level : AdditionalLevelsServerOnly)
	{
		std::string LevelPath = Level.ObjectID.AssetPathName.ToString().ToString();
		if (LevelPath == "None") {
			continue;
		}

		Log("Loading AdditionalLevelsServerOnly: " + LevelPath);
		bool Success = false;
		ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(World, Level, FVector(), FRotator(), &Success, FString());
		if (Success) {
			Log("Successfully loaded AdditionalLevelsServerOnly: " + LevelPath);
			FortGameState->OnFinishedStreamingAdditionalPlaylistLevel();
		}
	}
}