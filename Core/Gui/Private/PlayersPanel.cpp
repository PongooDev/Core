#include "pch.h"

#include "Gui/Public/GuiInternal.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameStateBase.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerState.h"
#include "FortniteGame/Public/Player/FortPlayerStateAthena.h"

#include <vector>
#include <algorithm>

namespace
{
	struct FPlayerRow
	{
		std::string Name;
		std::string AccountName;
		std::string NetName;
		const char* NameSource = "none";
		std::string DeathCause;
		int PlayerId = -1;
		int PingMs = 0;
		int Team = -1;
		int Squad = -1;
		int Kills = -1;
		int Place = -1;
		bool bIsBot = false;
		bool bDead = false;
	};

	enum : ImGuiID
	{
		ColIndex = 1,
		ColName,
		ColTeam,
		ColSquad,
		ColKills,
		ColStatus,
		ColPing,
	};

	class PlayersPanel : public GuiPanel
	{
	public:
		PlayersPanel()
		{
			GuiDetail::PrimeEngineClasses();
			AFortPlayerStateAthena::StaticClass();
			EDeathCause::StaticEnum();
			FDeathInfo::StaticStruct();
		}

		const char* Name() const override { return "Players"; }
		const char* Category() const override { return "SERVER"; }
		void Render() override;
		void Tick() override;

		int BadgeCount() const override { return (int)Rows.size(); }

	private:
		void Refresh(UWorld* World);
		void SortRows();
		void CopyToClipboard();

		ULONGLONG LastRefresh = 0;
		std::vector<FPlayerRow> Rows;
		int Bots = 0;
		int Alive = 0;

		ImGuiID SortColumn = ColName;
		bool bSortAscending = true;

		ImGuiTextFilter Filter;
	};

	void PlayersPanel::Refresh(UWorld* World)
	{
		Rows.clear();
		Bots = 0;
		Alive = 0;

		AGameStateBase* GameState = World->GameState;
		if (!GameState)
			return;

		TArray<APlayerState*>& PlayerArray = GameState->PlayerArray;
		const int32 Count = min(PlayerArray.Num(), 256);

		for (int32 i = 0; i < Count; i++)
		{
			if (!PlayerArray.IsValidIndex(i))
				break;

			APlayerState* PlayerState = PlayerArray.GetUnsafe(i);
			if (!PlayerState)
				continue;

			FPlayerRow& Row = Rows.emplace_back();

			const GuiDetail::FPlayerNameInfo NameInfo = GuiDetail::ReadPlayerName(PlayerState);
			Row.Name = NameInfo.Name;
			Row.AccountName = NameInfo.AccountName;
			Row.NetName = NameInfo.NetName;
			Row.NameSource = NameInfo.Source;

			Row.PingMs = (int)PlayerState->Ping * 4;
			Row.bIsBot = PlayerState->bIsABot;

			if (PlayerState->_HasPlayerId())
				Row.PlayerId = PlayerState->PlayerId;

			if (AFortPlayerStateAthena* Athena = PlayerState->Cast<AFortPlayerStateAthena>())
			{
				if (Athena->_HasTeamIndex())
					Row.Team = Athena->TeamIndex;
				if (Athena->_HasSquadId())
					Row.Squad = Athena->SquadId;
				if (Athena->_HasKillScore())
					Row.Kills = Athena->KillScore;
				if (Athena->_HasPlace())
					Row.Place = Athena->Place;

				if (Athena->_HasDeathInfo())
				{
					FDeathInfo& Death = Athena->DeathInfo;
					if (Death.bInitialized)
					{
						Row.bDead = true;
						if (EDeathCause::StaticEnum())
							Row.DeathCause = EDeathCause::ToString(Death.DeathCause);
					}
				}
			}

			if (Row.bIsBot)
				++Bots;
			if (!Row.bDead)
				++Alive;
		}
	}

	void PlayersPanel::SortRows()
	{
		if (Rows.size() < 2)
			return;

		std::stable_sort(Rows.begin(), Rows.end(), [&](const FPlayerRow& A, const FPlayerRow& B)
		{
			int Delta = 0;
			switch (SortColumn)
			{
			case ColName:   Delta = _stricmp(A.Name.c_str(), B.Name.c_str()); break;
			case ColTeam:   Delta = A.Team - B.Team; break;
			case ColSquad:  Delta = A.Squad - B.Squad; break;
			case ColKills:  Delta = A.Kills - B.Kills; break;
			case ColStatus: Delta = (int)A.bDead - (int)B.bDead; if (Delta == 0) Delta = A.Place - B.Place; break;
			case ColPing:   Delta = A.PingMs - B.PingMs; break;
			default: break;
			}

			if (Delta == 0)
				Delta = _stricmp(A.Name.c_str(), B.Name.c_str());

			return bSortAscending ? Delta < 0 : Delta > 0;
		});
	}

	void PlayersPanel::CopyToClipboard()
	{
		std::string Out = "Player\tTeam\tSquad\tKills\tStatus\tPing\n";

		for (const FPlayerRow& Row : Rows)
		{
			if (!Filter.PassFilter(Row.Name.c_str()))
				continue;

			std::string Status = Row.bDead
				? (Row.Place > 0 ? std::format("Dead #{}", Row.Place) : std::string("Dead"))
				: std::string("Alive");

			if (Row.bDead && !Row.DeathCause.empty())
				Status += " (" + Row.DeathCause + ")";

			Out += std::format("{}{}\t{}\t{}\t{}\t{}\t{}\n",
				Row.Name.empty() ? "(unnamed)" : Row.Name,
				Row.bIsBot ? " [BOT]" : "",
				Row.Team >= 0 ? std::to_string(Row.Team) : "-",
				Row.Squad >= 0 ? std::to_string(Row.Squad) : "-",
				Row.Kills >= 0 ? std::to_string(Row.Kills) : "-",
				Status,
				Row.bIsBot ? std::string("-") : std::format("{} ms", Row.PingMs));
		}

		ImGui::SetClipboardText(Out.c_str());
	}

	void PlayersPanel::Tick()
	{
		UWorld* World = UWorld::GetWorld();

		const ULONGLONG Now = GetTickCount64();
		if (!World || Now - LastRefresh < 250)
			return;

		LastRefresh = Now;
		Refresh(World);
		SortRows();
	}

	void PlayersPanel::Render()
	{
		UWorld* World = UWorld::GetWorld();

		const float Scale = GuiDetail::GetUiScale();

		if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_F))
			ImGui::SetKeyboardFocusHere();
		Filter.Draw("Filter", 200.0f * Scale);

		ImGui::SameLine();
		if (ImGui::Button("Copy"))
			CopyToClipboard();

		ImGui::SameLine();
		ImGui::TextDisabled("%d players (%d bots)", (int)Rows.size(), Bots);

		if (!Rows.empty())
		{
			ImGui::SameLine();
			ImGui::TextColored(Alive > 0 ? Theme::Good : Theme::TextDim, "%d alive", Alive);
		}

		ImGui::Separator();

		if (!World)
		{
			ImGui::TextDisabled("Waiting for the world to load...");
			return;
		}

		if (Rows.empty())
		{
			ImGui::TextDisabled("No players connected.");
			return;
		}

		constexpr ImGuiTableFlags Flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH |
			ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PadOuterX |
			ImGuiTableFlags_Sortable | ImGuiTableFlags_ScrollY;

		if (!ImGui::BeginTable("Players", 7, Flags))
			return;

		ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort, 26.0f * Scale, ColIndex);
		ImGui::TableSetupColumn("Player", ImGuiTableColumnFlags_DefaultSort, 0.0f, ColName);
		ImGui::TableSetupColumn("Team", ImGuiTableColumnFlags_WidthFixed, 52.0f * Scale, ColTeam);
		ImGui::TableSetupColumn("Squad", ImGuiTableColumnFlags_WidthFixed, 56.0f * Scale, ColSquad);
		ImGui::TableSetupColumn("Kills", ImGuiTableColumnFlags_WidthFixed, 52.0f * Scale, ColKills);
		ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_None, 0.0f, ColStatus);
		ImGui::TableSetupColumn("Ping", ImGuiTableColumnFlags_WidthFixed, 74.0f * Scale, ColPing);
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();

		if (ImGuiTableSortSpecs* Specs = ImGui::TableGetSortSpecs())
		{
			if (Specs->SpecsDirty)
			{
				if (Specs->SpecsCount > 0)
				{
					SortColumn = Specs->Specs[0].ColumnUserID;
					bSortAscending = Specs->Specs[0].SortDirection != ImGuiSortDirection_Descending;
				}

				SortRows();
				Specs->SpecsDirty = false;
			}
		}

		int Shown = 0;
		for (const FPlayerRow& Row : Rows)
		{
			if (!Filter.PassFilter(Row.Name.c_str()))
				continue;

			ImGui::TableNextRow();
			++Shown;

			ImGui::TableSetColumnIndex(0);
			ImGui::TextDisabled("%d", Shown);

			ImGui::TableSetColumnIndex(1);
			if (!Row.Name.empty())
				ImGui::TextUnformatted(Row.Name.c_str());
			else
				ImGui::TextDisabled("(unnamed)");

			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				if (Row.PlayerId >= 0)
					ImGui::Text("Player id %d", Row.PlayerId);
				ImGui::Text("Account name: %s", Row.AccountName.empty() ? "-" : Row.AccountName.c_str());
				ImGui::Text("Connection name: %s", Row.NetName.empty() ? "-" : Row.NetName.c_str());
				ImGui::TextDisabled("showing the %s name", Row.NameSource);
				ImGui::EndTooltip();
			}

			if (Row.bIsBot)
			{
				ImGui::SameLine();
				ImGui::TextDisabled("BOT");
			}

			ImGui::TableSetColumnIndex(2);
			if (Row.Team >= 0)
				ImGui::Text("%d", Row.Team);
			else
				ImGui::TextDisabled("-");

			ImGui::TableSetColumnIndex(3);
			if (Row.Squad >= 0)
				ImGui::Text("%d", Row.Squad);
			else
				ImGui::TextDisabled("-");

			ImGui::TableSetColumnIndex(4);
			if (Row.Kills >= 0)
				ImGui::Text("%d", Row.Kills);
			else
				ImGui::TextDisabled("-");

			ImGui::TableSetColumnIndex(5);
			if (!Row.bDead)
			{
				ImGui::TextColored(Theme::Good, "Alive");
			}
			else
			{
				std::string Status = Row.Place > 0 ? std::format("#{}", Row.Place) : std::string("Dead");
				if (!Row.DeathCause.empty())
					Status += " (" + Row.DeathCause + ")";

				ImGui::TextColored(Theme::Bad, "%s", Status.c_str());
			}

			ImGui::TableSetColumnIndex(6);
			if (Row.bIsBot)
			{
				ImGui::TextDisabled("-");
			}
			else
			{
				const ImVec4 Color = GuiDetail::PingColor(Row.PingMs);
				const ImVec2 Dot = ImGui::GetCursorScreenPos();
				const float Radius = 3.0f * Scale;

				ImGui::GetWindowDrawList()->AddCircleFilled(
					ImVec2(Dot.x + Radius, Dot.y + ImGui::GetTextLineHeight() * 0.5f),
					Radius, ImGui::GetColorU32(Color));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + Radius * 2.0f + 7.0f * Scale);
				ImGui::TextColored(Color, "%d ms", Row.PingMs);
			}
		}

		ImGui::EndTable();
	}
}

GuiPanel* GuiDetail::CreatePlayersPanel()
{
	return new PlayersPanel();
}
