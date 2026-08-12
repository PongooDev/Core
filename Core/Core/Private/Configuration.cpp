#include "pch.h"
#include "../Public/Configuration.h"

#include "Engine/Source/Runtime/Core/Public/Misc/Parse.h"

FCoreConfig ConfigurationManager::Config;

void ConfigurationManager::LoadConfig() {
	const wchar_t* CmdLine = GetCommandLineW();
	if (!CmdLine) {
		Log("Failed to get command line arguments!");
		return;
	}

	FCoreConfig NewConfig = FCoreConfig();

	NewConfig.bIsProd = FParse::Param(CmdLine, TEXT("bIsProd"));
	NewConfig.bEnableClientConsole = FParse::Param(CmdLine, TEXT("bEnableClientConsole"));
	NewConfig.bIsClient = FParse::Param(CmdLine, TEXT("bIsClient"));
	NewConfig.bSetClientLogVerbosity = FParse::Param(CmdLine, TEXT("bSetClientLogVerbosity"));
	NewConfig.bUseGameSessions = FParse::Param(CmdLine, TEXT("bUseGameSessions"));
	NewConfig.bSkipSessionValidation = FParse::Param(CmdLine, TEXT("bSkipSessionValidation"));
	NewConfig.bListenServer = FParse::Param(CmdLine, TEXT("bListenServer"));
	NewConfig.bDevSameTeam = FParse::Param(CmdLine, TEXT("bDevSameTeam"));
	NewConfig.bDisableGui = FParse::Param(CmdLine, TEXT("bDisableGui"));

	const wchar_t* PlaylistParam = wcsstr(CmdLine, L"Playlist=");
	if (PlaylistParam) {
		PlaylistParam += 9;
		std::wstring PlaylistWide;
		while (*PlaylistParam && *PlaylistParam != L' ' && *PlaylistParam != L'\0') {
			PlaylistWide += *PlaylistParam;
			PlaylistParam++;
		}
		if (!PlaylistWide.empty()) {
			NewConfig.Playlist = std::string(PlaylistWide.begin(), PlaylistWide.end());
		}
	}

	const wchar_t* MapToLoadParam = wcsstr(CmdLine, L"MapToLoad=");
	if (MapToLoadParam) {
		MapToLoadParam += 10;
		std::wstring MapToLoadWide;
		while (*MapToLoadParam && *MapToLoadParam != L' ' && *MapToLoadParam != L'\0') {
			MapToLoadWide += *MapToLoadParam;
			MapToLoadParam++;
		}
		if (!MapToLoadWide.empty()) {
			NewConfig.MapToLoad = std::string(MapToLoadWide.begin(), MapToLoadWide.end());
		}
	}

	Log("Configuration Loaded:");
	Log("bIsProd: " + std::to_string(NewConfig.bIsProd));
	Log("bEnableClientConsole: " + std::to_string(NewConfig.bEnableClientConsole));
	Log("bIsClient: " + std::to_string(NewConfig.bIsClient));
	Log("bSetClientLogVerbosity: " + std::to_string(NewConfig.bSetClientLogVerbosity));
	Log("bUseGameSessions: " + std::to_string(NewConfig.bUseGameSessions));
	Log("bSkipSessionValidation: " + std::to_string(NewConfig.bSkipSessionValidation));
	Log("bListenServer: " + std::to_string(NewConfig.bListenServer));
	Log("bDevSameTeam: " + std::to_string(NewConfig.bDevSameTeam));
	Log("bDisableGui: " + std::to_string(NewConfig.bDisableGui));
	Log("Playlist: " + NewConfig.Playlist);
	Log("MapToLoad: " + NewConfig.MapToLoad);

	Config = NewConfig;
}

FCoreConfig& ConfigurationManager::GetConfig() {
	return Config;
}