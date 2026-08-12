#include "pch.h"

#include "Gui/Public/GuiInternal.h"
#include "Core/Public/Version.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameStateBase.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerState.h"
#include "FortniteGame/Public/FortGameStateAthena.h"

#include "../../../includes/imgui/imgui.h"

#include <vector>

namespace
{
	std::string ReadNameBounded(const FString& Name)
	{
		const TArray<TCHAR>& Chars = Name.GetCharArray();

		const int32 Num = Chars.Num();
		if (Num <= 1 || Num > 128)
			return std::string();

		std::wstring Wide;
		Wide.reserve(Num - 1);

		for (int32 i = 0; i < Num - 1; i++)
		{
			if (!Chars.IsValidIndex(i))
				break;

			const TCHAR Char = Chars.GetUnsafe(i);
			if (!Char)
				break;

			Wide.push_back(Char);
		}

		if (Wide.empty())
			return std::string();

		const int Size = WideCharToMultiByte(CP_UTF8, 0, Wide.data(), (int)Wide.size(), nullptr, 0, nullptr, nullptr);
		if (Size <= 0)
			return std::string();

		std::string Out(Size, 0);
		WideCharToMultiByte(CP_UTF8, 0, Wide.data(), (int)Wide.size(), Out.data(), Size, nullptr, nullptr);
		return Out;
	}

	std::string FormatUptime(ULONGLONG Milliseconds)
	{
		const ULONGLONG Total = Milliseconds / 1000;
		return std::format("{:02}:{:02}:{:02}", Total / 3600, (Total / 60) % 60, Total % 60);
	}

	void StatCard(const char* Label, const std::string& Value, const ImVec4& ValueColor, float Width)
	{
		const float Scale = GuiDetail::GetUiScale();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(13.0f * Scale, 10.0f * Scale));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 7.0f * Scale);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.11f, 0.12f, 0.14f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.19f, 0.21f, 0.24f, 1.0f));

		ImGui::BeginChild(Label, ImVec2(Width, 64.0f * Scale),
			ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysUseWindowPadding,
			ImGuiWindowFlags_NoScrollbar);

		ImGui::TextDisabled("%s", Label);

		ImGui::PushFont(GuiDetail::GetHeadingFont(), 0.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ValueColor);
		ImGui::TextUnformatted(Value.c_str());
		ImGui::PopStyleColor();
		ImGui::PopFont();

		ImGui::EndChild();

		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(2);
	}

	void SectionLabel(const char* Text)
	{
		ImGui::Spacing();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.07f, 0.84f, 0.63f, 0.85f));
		ImGui::TextUnformatted(Text);
		ImGui::PopStyleColor();

		const ImVec2 Pos = ImGui::GetCursorScreenPos();
		ImGui::GetWindowDrawList()->AddLine(
			ImVec2(Pos.x, Pos.y + 1.0f),
			ImVec2(Pos.x + ImGui::GetContentRegionAvail().x, Pos.y + 1.0f),
			ImGui::GetColorU32(ImVec4(0.19f, 0.21f, 0.24f, 1.0f)));

		ImGui::Dummy(ImVec2(0, 6.0f));
	}

	void LabelledValue(const char* Label, const std::string& Value)
	{
		ImGui::TextDisabled("%s", Label);
		ImGui::SameLine(180.0f);
		ImGui::TextUnformatted(Value.c_str());
	}

	struct FPlayerRow
	{
		std::string Name;
		int PingMs = 0;
		bool bIsBot = false;
	};

	ImVec4 PingColor(int Ping)
	{
		if (Ping <= 60)
			return ImVec4(0.35f, 0.85f, 0.45f, 1.0f);
		if (Ping <= 140)
			return ImVec4(0.95f, 0.78f, 0.30f, 1.0f);

		return ImVec4(0.94f, 0.42f, 0.42f, 1.0f);
	}

	class StatusPanel : public GuiPanel
	{
	public:
		StatusPanel()
			: StartedAt(GetTickCount64())
		{
			EAthenaGamePhase::StaticEnum();
			EAthenaGamePhaseStep::StaticEnum();
		}

		const char* Name() const override { return "Status"; }
		const char* Category() const override { return "SERVER"; }
		void Render() override;

	private:
		void RenderCards(UWorld* World);
		void RenderServer();
		void RenderMatch(UWorld* World);
		void RenderPlayers(UWorld* World);
		void RefreshPlayers(UWorld* World);

		ULONGLONG StartedAt = 0;
		ULONGLONG LastRefresh = 0;
		std::vector<FPlayerRow> Players;
	};

	void StatusPanel::RenderServer()
	{
		SectionLabel("SERVER");

		FCoreConfig& Config = ConfigurationManager::GetConfig();

		LabelledValue("Uptime", FormatUptime(GetTickCount64() - StartedAt));
		LabelledValue("Fortnite", std::format("{:.2f}  (CL {})", Version::Fortnite_Version, Version::Fortnite_CL));
		LabelledValue("Engine", std::format("{:.2f}", Version::Engine_Version));
		LabelledValue("Playlist", Config.Playlist);
		LabelledValue("Map", Config.MapToLoad.empty() ? "(playlist default)" : Config.MapToLoad);
		LabelledValue("Listen server", Config.bListenServer ? "yes" : "no");
		LabelledValue("Game sessions", Config.bUseGameSessions ? "yes" : "no");
		LabelledValue("Console mirror", Gui::IsCapturingConsole() ? "active" : "off");

		ImGui::Spacing();
		ImGui::TextDisabled("%s", Version::VersionString.c_str());
	}

	void StatusPanel::RenderMatch(UWorld* World)
	{
		SectionLabel("MATCH");

		LabelledValue("World", World->GetName().ToString());

		AGameStateBase* Base = World->GameState;
		AFortGameStateAthena* GameState = Base ? Base->Cast<AFortGameStateAthena>() : nullptr;

		if (!GameState)
		{
			ImGui::TextDisabled("No game state yet.");
			return;
		}

		if (GameState->_HasGamePhase())
			LabelledValue("Phase", std::format("{} ({})",
				EAthenaGamePhase::ToString(GameState->GamePhase), (int)GameState->GamePhase));

		if (GameState->_HasGamePhaseStep())
			LabelledValue("Phase step", std::format("{} ({})",
				EAthenaGamePhaseStep::ToString(GameState->GamePhaseStep), (int)GameState->GamePhaseStep));

		if (GameState->_HasSafeZonePhase())
			LabelledValue("Safe zone", std::to_string((int)GameState->SafeZonePhase));

		if (GameState->_HasCurrentPlaylistId())
			LabelledValue("Playlist id", std::to_string(GameState->CurrentPlaylistId));

		if (GameState->_HasPlayersLeft())
			LabelledValue("Alive", std::format("{} players, {} teams",
				GameState->PlayersLeft, GameState->TeamsLeft));
	}

	void StatusPanel::RefreshPlayers(UWorld* World)
	{
		Players.clear();

		AGameStateBase* GameState = World->GameState;
		TArray<APlayerState*>* PlayerArray = GameState ? &GameState->PlayerArray : nullptr;

		const int32 Count = PlayerArray ? min(PlayerArray->Num(), 256) : 0;
		for (int32 i = 0; i < Count; i++)
		{
			if (!PlayerArray->IsValidIndex(i))
				break;

			APlayerState* PlayerState = PlayerArray->GetUnsafe(i);
			if (!PlayerState)
				continue;

			FPlayerRow& Row = Players.emplace_back();
			Row.Name = ReadNameBounded(PlayerState->_HasPlayerNamePrivate()
				? PlayerState->PlayerNamePrivate
				: PlayerState->PlayerName);
			Row.PingMs = (int)PlayerState->Ping * 4;
			Row.bIsBot = PlayerState->bIsABot;
		}
	}

	void StatusPanel::RenderPlayers(UWorld* World)
	{
		SectionLabel("PLAYERS");

		if (Players.empty())
		{
			ImGui::TextDisabled("No players connected.");
			return;
		}

		const float Scale = GuiDetail::GetUiScale();

		constexpr ImGuiTableFlags Flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH |
			ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PadOuterX;

		if (!ImGui::BeginTable("Players", 3, Flags))
			return;

		ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 26.0f * Scale);
		ImGui::TableSetupColumn("Player");
		ImGui::TableSetupColumn("Ping", ImGuiTableColumnFlags_WidthFixed, 74.0f * Scale);
		ImGui::TableHeadersRow();

		for (int i = 0; i < (int)Players.size(); i++)
		{
			const FPlayerRow& Row = Players[i];

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::TextDisabled("%d", i + 1);

			ImGui::TableSetColumnIndex(1);
			if (!Row.Name.empty())
				ImGui::TextUnformatted(Row.Name.c_str());
			else
				ImGui::TextDisabled("(unnamed)");

			if (Row.bIsBot)
			{
				ImGui::SameLine();
				ImGui::TextDisabled("BOT");
			}

			ImGui::TableSetColumnIndex(2);

			const ImVec4 Color = PingColor(Row.PingMs);
			const ImVec2 Dot = ImGui::GetCursorScreenPos();
			const float Radius = 3.0f * Scale;

			ImGui::GetWindowDrawList()->AddCircleFilled(
				ImVec2(Dot.x + Radius, Dot.y + ImGui::GetTextLineHeight() * 0.5f),
				Radius, ImGui::GetColorU32(Color));

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + Radius * 2.0f + 7.0f * Scale);
			ImGui::TextColored(Color, "%d ms", Row.PingMs);
		}

		ImGui::EndTable();
	}

	void StatusPanel::RenderCards(UWorld* World)
	{
		const float Scale = GuiDetail::GetUiScale();
		const float Spacing = ImGui::GetStyle().ItemSpacing.x;
		const float Width = (ImGui::GetContentRegionAvail().x - Spacing * 2.0f) / 3.0f;

		const ImVec4 Accent = ImVec4(0.07f, 0.84f, 0.63f, 1.0f);
		const ImVec4 Plain = ImVec4(0.90f, 0.92f, 0.94f, 1.0f);

		StatCard("UPTIME", FormatUptime(GetTickCount64() - StartedAt), Plain, Width);
		ImGui::SameLine();

		std::string PlayerText = "-";
		std::string PhaseText = "Loading";
		ImVec4 PhaseColor = ImVec4(0.48f, 0.52f, 0.57f, 1.0f);

		AGameStateBase* Base = World ? World->GameState : nullptr;
		AFortGameStateAthena* GameState = Base ? Base->Cast<AFortGameStateAthena>() : nullptr;

		PlayerText = std::to_string((int)Players.size());

		if (GameState && GameState->_HasGamePhase())
		{
			PhaseText = EAthenaGamePhase::ToString(GameState->GamePhase);
			PhaseColor = Accent;
		}

		StatCard("PLAYERS", PlayerText, Players.empty() ? Plain : Accent, Width);
		ImGui::SameLine();
		StatCard("PHASE", PhaseText, PhaseColor, Width);

		ImGui::Dummy(ImVec2(0, 4.0f * Scale));
	}

	void StatusPanel::Render()
	{
		UWorld* World = UWorld::GetWorld();

		const ULONGLONG Now = GetTickCount64();
		if (World && Now - LastRefresh >= 250)
		{
			LastRefresh = Now;
			RefreshPlayers(World);
		}

		RenderCards(World);
		RenderServer();

		if (!World)
		{
			ImGui::Spacing();
			ImGui::TextDisabled("Waiting for the world to load...");
			return;
		}

		RenderMatch(World);
		RenderPlayers(World);
	}
}

GuiPanel* GuiDetail::CreateStatusPanel()
{
	return new StatusPanel();
}
