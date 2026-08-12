#include "pch.h"

#include "Gui/Public/GuiInternal.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameStateBase.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerState.h"
#include "FortniteGame/Public/Player/FortPlayerController.h"
#include "FortniteGame/Public/Player/FortRegisteredPlayerInfo.h"

void GuiDetail::SectionLabel(const char* Text)
{
	ImGui::Spacing();
	ImGui::PushStyleColor(ImGuiCol_Text, Theme::AccentLabel);
	ImGui::TextUnformatted(Text);
	ImGui::PopStyleColor();

	const ImVec2 Pos = ImGui::GetCursorScreenPos();
	ImGui::GetWindowDrawList()->AddLine(
		ImVec2(Pos.x, Pos.y + 1.0f),
		ImVec2(Pos.x + ImGui::GetContentRegionAvail().x, Pos.y + 1.0f),
		ImGui::GetColorU32(Theme::Line));

	ImGui::Dummy(ImVec2(0, 6.0f));
}

void GuiDetail::LabelledValue(const char* Label, const std::string& Value, float Column)
{
	ImGui::TextDisabled("%s", Label);
	ImGui::SameLine(Column * GetUiScale());
	ImGui::TextUnformatted(Value.c_str());
}

void GuiDetail::StatCard(const char* Label, const std::string& Value, const ImVec4& ValueColor, float Width)
{
	const float Scale = GetUiScale();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(13.0f * Scale, 10.0f * Scale));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 7.0f * Scale);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, Theme::Surface);
	ImGui::PushStyleColor(ImGuiCol_Border, Theme::Line);

	ImGui::BeginChild(Label, ImVec2(Width, 64.0f * Scale),
		ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysUseWindowPadding,
		ImGuiWindowFlags_NoScrollbar);

	ImGui::TextDisabled("%s", Label);

	ImGui::PushFont(GetHeadingFont(), 0.0f);
	ImGui::PushStyleColor(ImGuiCol_Text, ValueColor);
	ImGui::TextUnformatted(Value.c_str());
	ImGui::PopStyleColor();
	ImGui::PopFont();

	ImGui::EndChild();

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar(2);
}

ImVec4 GuiDetail::PingColor(int PingMs)
{
	if (PingMs <= 60)
		return Theme::Good;
	if (PingMs <= 140)
		return Theme::Warn;

	return Theme::Bad;
}

std::string GuiDetail::ReadNameBounded(const FString& Name)
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

void GuiDetail::PrimeEngineClasses()
{
	UWorld::StaticClass();
	AActor::StaticClass();
	AController::StaticClass();
	APlayerController::StaticClass();
	AGameStateBase::StaticClass();
	APlayerState::StaticClass();
	UNetDriver::StaticClass();
	UNetConnection::StaticClass();
	AFortPlayerController::StaticClass();
	UFortRegisteredPlayerInfo::StaticClass();
}

GuiDetail::FPlayerNameInfo GuiDetail::ReadPlayerName(APlayerState* PlayerState)
{
	FPlayerNameInfo Out;

	if (!PlayerState)
		return Out;

	if (PlayerState->_HasPlayerNamePrivate())
		Out.NetName = ReadNameBounded(PlayerState->PlayerNamePrivate);

	if (Out.NetName.empty() && PlayerState->_HasPlayerName())
		Out.NetName = ReadNameBounded(PlayerState->PlayerName);

	if (PlayerState->_HasOwner())
	{
		AActor* Owner = PlayerState->Owner;
		AFortPlayerController* Controller = Owner ? Owner->Cast<AFortPlayerController>() : nullptr;

		if (Controller && Controller->_HasMyPlayerInfo())
		{
			UFortRegisteredPlayerInfo* Info = Controller->MyPlayerInfo;
			if (Info && Info->_HasPlayerName())
				Out.AccountName = ReadNameBounded(Info->PlayerName);
		}
	}

	if (!Out.AccountName.empty())
	{
		Out.Name = Out.AccountName;
		Out.Source = "account";
	}
	else if (!Out.NetName.empty())
	{
		Out.Name = Out.NetName;
		Out.Source = "connection";
	}

	return Out;
}

std::string GuiDetail::FormatDuration(ULONGLONG TotalSeconds)
{
	return std::format("{:02}:{:02}:{:02}", TotalSeconds / 3600, (TotalSeconds / 60) % 60, TotalSeconds % 60);
}
