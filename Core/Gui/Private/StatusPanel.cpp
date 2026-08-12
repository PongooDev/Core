#include "pch.h"

#include "Gui/Public/GuiInternal.h"
#include "Core/Public/Version.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameStateBase.h"
#include "FortniteGame/Public/FortGameStateAthena.h"

namespace
{
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

		ULONGLONG StartedAt = 0;
		ULONGLONG LastRefresh = 0;
		int PlayerCount = 0;
	};

	void StatusPanel::RenderServer()
	{
		GuiDetail::SectionLabel("SERVER");

		FCoreConfig& Config = ConfigurationManager::GetConfig();

		GuiDetail::LabelledValue("Uptime", GuiDetail::FormatDuration((GetTickCount64() - StartedAt) / 1000));
		GuiDetail::LabelledValue("Fortnite", std::format("{:.2f}  (CL {})", Version::Fortnite_Version, Version::Fortnite_CL));
		GuiDetail::LabelledValue("Engine", std::format("{:.2f}", Version::Engine_Version));
		GuiDetail::LabelledValue("Playlist", Config.Playlist);
		GuiDetail::LabelledValue("Map", Config.MapToLoad.empty() ? "(playlist default)" : Config.MapToLoad);
		GuiDetail::LabelledValue("Listen server", Config.bListenServer ? "yes" : "no");
		GuiDetail::LabelledValue("Game sessions", Config.bUseGameSessions ? "yes" : "no");
		GuiDetail::LabelledValue("Console mirror", Gui::IsCapturingConsole() ? "active" : "off");

		ImGui::Spacing();
		ImGui::TextDisabled("%s", Version::VersionString.c_str());
	}

	void StatusPanel::RenderMatch(UWorld* World)
	{
		GuiDetail::SectionLabel("MATCH");

		GuiDetail::LabelledValue("World", World->GetName().ToString());

		AGameStateBase* Base = World->GameState;
		AFortGameStateAthena* GameState = Base ? Base->Cast<AFortGameStateAthena>() : nullptr;

		if (!GameState)
		{
			ImGui::TextDisabled("No game state yet.");
			return;
		}

		if (GameState->_HasGamePhase())
			GuiDetail::LabelledValue("Phase", std::format("{} ({})",
				EAthenaGamePhase::ToString(GameState->GamePhase), (int)GameState->GamePhase));

		if (GameState->_HasGamePhaseStep())
			GuiDetail::LabelledValue("Phase step", std::format("{} ({})",
				EAthenaGamePhaseStep::ToString(GameState->GamePhaseStep), (int)GameState->GamePhaseStep));

		if (GameState->_HasSafeZonePhase())
			GuiDetail::LabelledValue("Safe zone", std::to_string((int)GameState->SafeZonePhase));

		if (GameState->_HasCurrentPlaylistId())
			GuiDetail::LabelledValue("Playlist id", std::to_string(GameState->CurrentPlaylistId));

		if (GameState->_HasPlayersLeft())
			GuiDetail::LabelledValue("Alive", std::format("{} players, {} teams",
				GameState->PlayersLeft, GameState->TeamsLeft));
	}

	void StatusPanel::RenderCards(UWorld* World)
	{
		const float Scale = GuiDetail::GetUiScale();
		const float Spacing = ImGui::GetStyle().ItemSpacing.x;
		const float Width = (ImGui::GetContentRegionAvail().x - Spacing * 2.0f) / 3.0f;

		GuiDetail::StatCard("UPTIME", GuiDetail::FormatDuration((GetTickCount64() - StartedAt) / 1000), Theme::Text, Width);
		ImGui::SameLine();

		std::string PhaseText = "Loading";
		ImVec4 PhaseColor = Theme::TextDim;

		AGameStateBase* Base = World ? World->GameState : nullptr;
		AFortGameStateAthena* GameState = Base ? Base->Cast<AFortGameStateAthena>() : nullptr;

		if (GameState && GameState->_HasGamePhase())
		{
			PhaseText = EAthenaGamePhase::ToString(GameState->GamePhase);
			PhaseColor = Theme::Accent;
		}

		GuiDetail::StatCard("PLAYERS", std::to_string(PlayerCount), PlayerCount > 0 ? Theme::Accent : Theme::Text, Width);
		ImGui::SameLine();
		GuiDetail::StatCard("PHASE", PhaseText, PhaseColor, Width);

		ImGui::Dummy(ImVec2(0, 4.0f * Scale));
	}

	void StatusPanel::Render()
	{
		UWorld* World = UWorld::GetWorld();

		const ULONGLONG Now = GetTickCount64();
		if (World && Now - LastRefresh >= 250)
		{
			LastRefresh = Now;

			AGameStateBase* GameState = World->GameState;
			PlayerCount = GameState ? GameState->PlayerArray.Num() : 0;
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
	}
}

GuiPanel* GuiDetail::CreateStatusPanel()
{
	return new StatusPanel();
}
