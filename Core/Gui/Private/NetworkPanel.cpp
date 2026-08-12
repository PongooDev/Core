#include "pch.h"

#include "Gui/Public/GuiInternal.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"

#include "../../../includes/imgui/imgui.h"

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
		std::string State;
		int32 Channels = 0;
		int32 QueuedBits = 0;
		int32 MaxPacket = 0;
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
		uint32 ReplicationFrame = 0;
	};

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
		ImGui::SameLine(200.0f);
		ImGui::TextUnformatted(Value.c_str());
	}

	class NetworkPanel : public GuiPanel
	{
	public:
		const char* Name() const override { return "Network"; }
		const char* Category() const override { return "DIAGNOSTICS"; }
		void Render() override;

	private:
		void Refresh();

		FDriverInfo Driver;
		std::vector<FConnectionRow> Connections;
		ULONGLONG LastRefresh = 0;
	};

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
		}
	}

	void NetworkPanel::Render()
	{
		const ULONGLONG Now = GetTickCount64();
		if (Now - LastRefresh >= 250)
		{
			LastRefresh = Now;
			Refresh();
		}

		if (!Driver.bValid)
		{
			ImGui::TextDisabled("No net driver yet - the server is not listening.");
			return;
		}

		SectionLabel("DRIVER");

		LabelledValue("Name", Driver.DriverName.empty() ? "-" : Driver.DriverName);
		LabelledValue("Max tick rate", std::to_string(Driver.MaxTickRate));
		LabelledValue("Max client rate", std::to_string(Driver.MaxClientRate));
		LabelledValue("Max internet rate", std::to_string(Driver.MaxInternetClientRate));
		LabelledValue("Connection timeout", std::format("{:.0f}s", Driver.ConnectionTimeout));
		LabelledValue("Keep alive", std::format("{:.1f}s", Driver.KeepAliveTime));
		LabelledValue("Relevant timeout", std::format("{:.0f}s", Driver.RelevantTimeout));
		LabelledValue("Replication frame", std::to_string(Driver.ReplicationFrame));

		SectionLabel("CONNECTIONS");

		if (Connections.empty())
		{
			ImGui::TextDisabled("No client connections.");
			return;
		}

		const float Scale = GuiDetail::GetUiScale();

		constexpr ImGuiTableFlags Flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH |
			ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PadOuterX;

		if (!ImGui::BeginTable("Connections", 5, Flags))
			return;

		ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 26.0f * Scale);
		ImGui::TableSetupColumn("State");
		ImGui::TableSetupColumn("Channels", ImGuiTableColumnFlags_WidthFixed, 80.0f * Scale);
		ImGui::TableSetupColumn("Queued bits", ImGuiTableColumnFlags_WidthFixed, 100.0f * Scale);
		ImGui::TableSetupColumn("MTU", ImGuiTableColumnFlags_WidthFixed, 70.0f * Scale);
		ImGui::TableHeadersRow();

		for (int i = 0; i < (int)Connections.size(); i++)
		{
			const FConnectionRow& Row = Connections[i];

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::TextDisabled("%d", i + 1);

			ImGui::TableSetColumnIndex(1);
			if (Row.bOpen)
				ImGui::TextColored(ImVec4(0.35f, 0.85f, 0.45f, 1.0f), "%s", Row.State.c_str());
			else
				ImGui::TextColored(ImVec4(0.95f, 0.78f, 0.30f, 1.0f), "%s", Row.State.c_str());

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%d", Row.Channels);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%d", Row.QueuedBits);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%d", Row.MaxPacket);
		}

		ImGui::EndTable();
	}
}

GuiPanel* GuiDetail::CreateNetworkPanel()
{
	return new NetworkPanel();
}
