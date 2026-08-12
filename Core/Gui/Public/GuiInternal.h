#pragma once
#include "pch.h"

#include "Gui/Public/Gui.h"

#include "../../../includes/imgui/imgui.h"

#include <deque>

class FString;

struct FGuiScopedLock
{
	SRWLOCK* Lock;

	explicit FGuiScopedLock(SRWLOCK& InLock) : Lock(&InLock) { AcquireSRWLockExclusive(Lock); }
	~FGuiScopedLock() { ReleaseSRWLockExclusive(Lock); }

	FGuiScopedLock(const FGuiScopedLock&) = delete;
	FGuiScopedLock& operator=(const FGuiScopedLock&) = delete;
};

namespace Theme
{
	inline constexpr ImVec4 Accent = ImVec4(0.07f, 0.84f, 0.63f, 1.00f);
	inline constexpr ImVec4 AccentSoft = ImVec4(0.07f, 0.84f, 0.63f, 0.45f);
	inline constexpr ImVec4 AccentBright = ImVec4(0.20f, 0.93f, 0.73f, 1.00f);
	inline constexpr ImVec4 AccentLabel = ImVec4(0.07f, 0.84f, 0.63f, 0.85f);

	inline constexpr ImVec4 Background = ImVec4(0.07f, 0.08f, 0.09f, 1.00f);
	inline constexpr ImVec4 Sidebar = ImVec4(0.055f, 0.06f, 0.07f, 1.00f);
	inline constexpr ImVec4 Surface = ImVec4(0.11f, 0.12f, 0.14f, 1.00f);
	inline constexpr ImVec4 SurfaceHover = ImVec4(0.16f, 0.18f, 0.21f, 1.00f);
	inline constexpr ImVec4 Recessed = ImVec4(0.09f, 0.10f, 0.11f, 1.00f);
	inline constexpr ImVec4 Line = ImVec4(0.19f, 0.21f, 0.24f, 1.00f);

	inline constexpr ImVec4 Text = ImVec4(0.90f, 0.92f, 0.94f, 1.00f);
	inline constexpr ImVec4 TextDim = ImVec4(0.48f, 0.52f, 0.57f, 1.00f);

	inline constexpr ImVec4 Good = ImVec4(0.35f, 0.85f, 0.45f, 1.00f);
	inline constexpr ImVec4 Warn = ImVec4(0.95f, 0.78f, 0.30f, 1.00f);
	inline constexpr ImVec4 Bad = ImVec4(0.94f, 0.42f, 0.42f, 1.00f);
	inline constexpr ImVec4 Info = ImVec4(0.56f, 0.78f, 1.00f, 1.00f);
}

namespace GuiDetail
{
	constexpr size_t MaxLogLines = 5000;

	bool TakePendingLog(std::deque<std::string>& Out);

	ImFont* GetMonoFont();
	ImFont* GetHeadingFont();
	float GetUiScale();

	void StopConsoleMirror();

	void SectionLabel(const char* Text);
	void LabelledValue(const char* Label, const std::string& Value, float Column = 200.0f);
	void StatCard(const char* Label, const std::string& Value, const ImVec4& ValueColor, float Width);
	ImVec4 PingColor(int PingMs);
	std::string ReadNameBounded(const FString& Name);
	std::string FormatDuration(ULONGLONG TotalSeconds);

	GuiPanel* CreateConsolePanel();
	GuiPanel* CreateStatusPanel();
	GuiPanel* CreatePlayersPanel();
	GuiPanel* CreateNetworkPanel();
	GuiPanel* CreatePerformancePanel();
	GuiPanel* CreateOffsetsPanel();
}
