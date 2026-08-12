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
	};

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
		void Render() override;

	private:
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
		if (!ImGui::CollapsingHeader("Server", ImGuiTreeNodeFlags_DefaultOpen))
			return;

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
		if (!ImGui::CollapsingHeader("Match", ImGuiTreeNodeFlags_DefaultOpen))
			return;

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
			Row.Name = ReadNameBounded(PlayerState->PlayerName);
			Row.PingMs = (int)PlayerState->Ping * 4;
		}
	}

	void StatusPanel::RenderPlayers(UWorld* World)
	{
		if (!ImGui::CollapsingHeader("Players", ImGuiTreeNodeFlags_DefaultOpen))
			return;

		const ULONGLONG Now = GetTickCount64();
		if (Now - LastRefresh >= 250)
		{
			LastRefresh = Now;
			RefreshPlayers(World);
		}

		if (Players.empty())
		{
			ImGui::TextDisabled("No players connected.");
			return;
		}

		ImGui::Text("Connected: %d", (int)Players.size());

		constexpr ImGuiTableFlags Flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
			ImGuiTableFlags_SizingStretchProp;

		if (!ImGui::BeginTable("Players", 2, Flags))
			return;

		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Ping", ImGuiTableColumnFlags_WidthFixed, 70.0f);
		ImGui::TableHeadersRow();

		for (const FPlayerRow& Row : Players)
		{
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			if (!Row.Name.empty())
				ImGui::TextUnformatted(Row.Name.c_str());
			else
				ImGui::TextDisabled("(unnamed)");

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d ms", Row.PingMs);
		}

		ImGui::EndTable();
	}

	void StatusPanel::Render()
	{
		RenderServer();

		UWorld* World = UWorld::GetWorld();
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
