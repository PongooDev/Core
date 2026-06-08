#include "pch.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAthena.h"

#include "FortniteGame/Public/Mcp/McpProfileSys.h"

void UFortMcpProfileAthena::EndBattleRoyaleGame(const TArray<FFortQuestObjectiveCompletion>& Advance, uint8 MatchType, int32 MinutesAlive, int32 PersonalKills, int32 TeamKills, int32 Placement, const TArray<FString>& Medals, FDedicatedServerUrlContext* Context)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("EndBattleRoyaleGame");

	struct FortMcpProfileAthena_EndBattleRoyaleGame final
	{
	public:
		TArray<FFortQuestObjectiveCompletion> Advance;
		uint8 MatchType;
		uint8 Pad_11[0x3];
		int32 MinutesAlive;
		int32 PersonalKills;
		int32 TeamKills;
		int32 Placement;
		uint8 Pad_24[0x4];
		TArray<FString> Medals;
		uint8 Pad_38[0x8];
		FDedicatedServerUrlContext Context;
	};

	FortMcpProfileAthena_EndBattleRoyaleGame Parms{};

	Parms.Advance = std::move(Advance);
	Parms.MatchType = MatchType;
	Parms.MinutesAlive = MinutesAlive;
	Parms.PersonalKills = PersonalKills;
	Parms.TeamKills = TeamKills;
	Parms.Placement = Placement;
	Parms.Medals = std::move(Medals);

	ProcessEvent(Func, &Parms);

	if (Context != nullptr)
		*Context = std::move(Parms.Context);
}