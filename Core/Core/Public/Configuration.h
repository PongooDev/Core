#pragma once
#include "pch.h"

struct FCoreConfig {
	bool bIsProd = false;

	bool bEnableClientConsole = false;
	bool bIsClient = false;
	bool bSetClientLogVerbosity = false;
	bool bUseGameSessions = false;
	bool bSkipSessionValidation = false;
	bool bListenServer = false;
	bool bDevSameTeam = false;
	bool bDisableGui = false;

	std::string Playlist = "2";
	std::string MapToLoad = "";

	FCoreConfig() = default;
};

class ConfigurationManager {
public:
	static FCoreConfig Config;
public:
	static void LoadConfig();

	static FCoreConfig& GetConfig();
};