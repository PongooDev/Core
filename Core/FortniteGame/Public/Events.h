#pragma once
#include "pch.h"

#include <vector>

class AActor;
class UFunction;

struct FEventFunction {
	bool bCallOnLoader;
	const char* FunctionPath;
};

struct FEvent {
	double Version;
	const char* LoaderClassPath;
	const char* ScriptingClassPath;
	const char* LoadLevelFuncPath;
	std::vector<FEventFunction> Functions;
};

class Events {
public:
	static inline std::vector<FEvent> Registry = {
		// 4.5 -- Blast Off (Geode): the rocket launch.
		{
			4.5,
			nullptr,
			"/Game/Athena/Maps/Test/Events/BP_GeodeScripting.BP_GeodeScripting_C",
			nullptr,
			{
				{ false, "/Game/Athena/Maps/Test/Events/BP_GeodeScripting.BP_GeodeScripting_C.LaunchSequence" },
			}
		},
		// 6.21 -- Butterfly event: cube explodes
		{
			6.21,
			"/Game/Athena/Prototype/Blueprints/Island/BP_Butterfly.BP_Butterfly_C",
			nullptr,
			"/Game/Athena/Prototype/Blueprints/Island/BP_Butterfly.BP_Butterfly_C.LoadButterflySublevel",
			{
				{ true, "/Game/Athena/Prototype/Blueprints/Island/BP_Butterfly.BP_Butterfly_C.ButterflySequence" },
			}
		}
	};

	static const FEvent* GetEventForCurrentVersion();

	static AActor* FindOrSpawnEventActor(const char* ClassPath);

	static bool StartEvent();
};
