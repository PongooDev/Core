#include "pch.h"

#include "Gui/Public/GuiInternal.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerState.h"

#include <vector>

namespace
{
	const char* StateName(EConnectionState State)
	{
		switch (State)
		{
		case USOCK_Invalid: return "Invalid";
		case USOCK_Closed:  return "Closed";
		case USOCK_Pending: return "Pending";
		case USOCK_Open:    return "Open";
		default:            return "Unknown";
		}
	}

	struct FConnectionRow
	{
		std::string PlayerName;
		std::string State;
		int32 Channels = 0;
		int32 QueuedBits = 0;
		int32 MaxPacket = 0;
		int PingMs = -1;
		float AgeSeconds = -1.0f;
		float IdleSeconds = -1.0f;
		bool bOpen = false;
	};

	struct FDriverInfo
	{
		bool bValid = false;
		std::string DriverName;
		int32 MaxTickRate = 0;
		int32 MaxClientRate = 0;
		int32 MaxInternetClientRate = 0;
		float ConnectionTimeout = 0.0f;
		float KeepAliveTime = 0.0f;
		float RelevantTimeout = 0.0f;
		float Time = -1.0f;
		uint32 ReplicationFrame = 0;
	};

	class NetworkPanel : public GuiPanel
	{
	public:
		const char* Name() const override { return "Network"; }
		const char* Category() const override { return "DIAGNOSTICS"; }
		void Render() override;
		void Tick() override;

	private:
		void Refresh();

		FDriverInfo Driver;
		std::vector<FConnectionRow> Connections;
		ULONGLONG LastRefresh = 0;

		uint32 RateLastFrame = 0;
		ULONGLONG RateLastTick = 0;
		float ReplicationRate = -1.0f;
	};

	void NetworkPanel::Tick()
	{
		const ULONGLONG Now = GetTickCount64();
		if (Now - LastRefresh < 250)
			return;

		LastRefresh = Now;
		Refresh();

		if (!Driver.bValid || !ServerOffsets::UNetDriver__ReplicationFrame)
		{
			RateLastTick = 0;
			ReplicationRate = -1.0f;
			return;
		}

		if (!RateLastTick || Driver.ReplicationFrame < RateLastFrame)
		{
			RateLastTick = Now;
			RateLastFrame = Driver.ReplicationFrame;
		}
		else if (Now - RateLastTick >= 1000)
		{
			const float Seconds = (float)(Now - RateLastTick) / 1000.0f;
			ReplicationRate = (float)(Driver.ReplicationFrame - RateLastFrame) / Seconds;

			RateLastTick = Now;
			RateLastFrame = Driver.ReplicationFrame;
		}
	}

	void NetworkPanel::Refresh()
	{
		Driver = FDriverInfo();
		Connections.clear();

		UWorld* World = UWorld::GetWorld();
		if (!World || !World->_HasNetDriver())
			return;

		UNetDriver* NetDriver = World->NetDriver;
		if (!NetDriver)
			return;

		Driver.bValid = true;

		if (NetDriver->_HasNetDriverName())
			Driver.DriverName = NetDriver->NetDriverName.ToString().ToString();
		if (NetDriver->_HasNetServerMaxTickRate())
			Driver.MaxTickRate = NetDriver->NetServerMaxTickRate;
		if (NetDriver->_HasMaxClientRate())
			Driver.MaxClientRate = NetDriver->MaxClientRate;
		if (NetDriver->_HasMaxInternetClientRate())
			Driver.MaxInternetClientRate = NetDriver->MaxInternetClientRate;
		if (NetDriver->_HasConnectionTimeout())
			Driver.ConnectionTimeout = NetDriver->ConnectionTimeout;
		if (NetDriver->_HasKeepAliveTime())
			Driver.KeepAliveTime = NetDriver->KeepAliveTime;
		if (NetDriver->_HasRelevantTimeout())
			Driver.RelevantTimeout = NetDriver->RelevantTimeout;
		if (NetDriver->_HasTime())
			Driver.Time = NetDriver->Time;

		if (ServerOffsets::UNetDriver__ReplicationFrame)
			Driver.ReplicationFrame = NetDriver->ReplicationFrame;

		if (!NetDriver->_HasClientConnections())
			return;

		TArray<UNetConnection*>& Clients = NetDriver->ClientConnections;
		const int32 Count = min(Clients.Num(), 128);

		for (int32 i = 0; i < Count; i++)
		{
			if (!Clients.IsValidIndex(i))
				break;

			UNetConnection* Connection = Clients.GetUnsafe(i);
			if (!Connection)
				continue;

			FConnectionRow& Row = Connections.emplace_back();

			if (ServerOffsets::UNetConnection__State)
			{
				const EConnectionState State = Connection->State;
				Row.State = StateName(State);
				Row.bOpen = State == USOCK_Open;
			}
			else
			{
				Row.State = "-";
			}

			if (Connection->_HasOpenChannels())
				Row.Channels = Connection->OpenChannels.Num();
			if (Connection->_HasMaxPacket())
				Row.MaxPacket = Connection->MaxPacket;

			Row.QueuedBits = Connection->QueuedBits;

			if (Connection->_HasOwningActor())
			{
				AActor* Owner = Connection->OwningActor;
				APlayerController* Controller = Owner ? Owner->Cast<APlayerController>() : nullptr;

				if (Controller && Controller->_HasPlayerState())
				{
					if (APlayerState* PlayerState = Controller->PlayerState)
					{
						Row.PlayerName = GuiDetail::ReadNameBounded(PlayerState->_HasPlayerNamePrivate()
							? PlayerState->PlayerNamePrivate
							: PlayerState->PlayerName);
						Row.PingMs = (int)PlayerState->Ping * 4;
					}
				}
			}

			if (Driver.Time >= 0.0f)
			{
				if (UNetConnection::ConnectTimeOffset())
				{
					const float Age = Driver.Time - Connection->ConnectTime;
					Row.AgeSeconds = Age > 0.0f ? Age : 0.0f;
				}

				if (Connection->_HasLastReceiveTime())
				{
					const float Idle = Driver.Time - (float)Connection->LastReceiveTime;
					Row.IdleSeconds = Idle > 0.0f ? Idle : 0.0f;
				}
			}
		}
	}

	void NetworkPanel::Render()
	{
		if (!Driver.bValid)
		{
			ImGui::TextDisabled("No net driver yet - the server is not listening.");
			return;
		}

		GuiDetail::SectionLabel("DRIVER");

		GuiDetail::LabelledValue("Name", Driver.DriverName.empty() ? "-" : Driver.DriverName);
		GuiDetail::LabelledValue("Max tick rate", std::to_string(Driver.MaxTickRate));
		GuiDetail::LabelledValue("Max client rate", std::to_string(Driver.MaxClientRate));
		GuiDetail::LabelledValue("Max internet rate", std::to_string(Driver.MaxInternetClientRate));
		GuiDetail::LabelledValue("Connection timeout", std::format("{:.0f}s", Driver.ConnectionTimeout));
		GuiDetail::LabelledValue("Keep alive", std::format("{:.1f}s", Driver.KeepAliveTime));
		GuiDetail::LabelledValue("Relevant timeout", std::format("{:.0f}s", Driver.RelevantTimeout));
		GuiDetail::LabelledValue("Driver time", Driver.Time >= 0.0f
			? GuiDetail::FormatDuration((ULONGLONG)Driver.Time)
			: std::string("-"));
		GuiDetail::LabelledValue("Replication frame", std::to_string(Driver.ReplicationFrame));
		GuiDetail::LabelledValue("Replication rate", ReplicationRate >= 0.0f
			? std::format("{:.1f} /s", ReplicationRate)
			: std::string("-"));

		const std::string ConnectionsLabel = Connections.empty()
			? std::string("CONNECTIONS")
			: std::format("CONNECTIONS ({})", Connections.size());
		GuiDetail::SectionLabel(ConnectionsLabel.c_str());

		if (Connections.empty())
		{
			ImGui::TextDisabled("No client connections.");
			return;
		}

		const float Scale = GuiDetail::GetUiScale();

		constexpr ImGuiTableFlags Flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH |
			ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_ScrollY;

		if (!ImGui::BeginTable("Connections", 8, Flags))
			return;

		ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 26.0f * Scale);
		ImGui::TableSetupColumn("Player");
		ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_WidthFixed, 66.0f * Scale);
		ImGui::TableSetupColumn("Age", ImGuiTableColumnFlags_WidthFixed, 76.0f * Scale);
		ImGui::TableSetupColumn("Idle", ImGuiTableColumnFlags_WidthFixed, 62.0f * Scale);
		ImGui::TableSetupColumn("Channels", ImGuiTableColumnFlags_WidthFixed, 74.0f * Scale);
		ImGui::TableSetupColumn("Queued bits", ImGuiTableColumnFlags_WidthFixed, 92.0f * Scale);
		ImGui::TableSetupColumn("MTU", ImGuiTableColumnFlags_WidthFixed, 56.0f * Scale);
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();

		for (int i = 0; i < (int)Connections.size(); i++)
		{
			const FConnectionRow& Row = Connections[i];

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::TextDisabled("%d", i + 1);

			ImGui::TableSetColumnIndex(1);
			if (!Row.PlayerName.empty())
				ImGui::TextUnformatted(Row.PlayerName.c_str());
			else
				ImGui::TextDisabled("(no player)");

			if (Row.PingMs >= 0 && ImGui::IsItemHovered())
				ImGui::SetTooltip("Ping %d ms", Row.PingMs);

			ImGui::TableSetColumnIndex(2);
			ImGui::TextColored(Row.bOpen ? Theme::Good : Theme::Warn, "%s", Row.State.c_str());

			ImGui::TableSetColumnIndex(3);
			if (Row.AgeSeconds >= 0.0f)
				ImGui::TextUnformatted(GuiDetail::FormatDuration((ULONGLONG)Row.AgeSeconds).c_str());
			else
				ImGui::TextDisabled("-");

			ImGui::TableSetColumnIndex(4);
			if (Row.IdleSeconds >= 0.0f)
			{
				const ImVec4 Color = Row.IdleSeconds < 1.0f ? Theme::Good
					: Row.IdleSeconds < 5.0f ? Theme::Warn : Theme::Bad;
				ImGui::TextColored(Color, "%.1fs", Row.IdleSeconds);
			}
			else
			{
				ImGui::TextDisabled("-");
			}

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%d", Row.Channels);

			ImGui::TableSetColumnIndex(6);
			ImGui::Text("%d", Row.QueuedBits);

			ImGui::TableSetColumnIndex(7);
			ImGui::Text("%d", Row.MaxPacket);
		}

		ImGui::EndTable();
	}
}

GuiPanel* GuiDetail::CreateNetworkPanel()
{
	return new NetworkPanel();
}
