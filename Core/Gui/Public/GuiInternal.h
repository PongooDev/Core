#pragma once
#include "pch.h"

#include "Gui/Public/Gui.h"

#include <deque>

struct ImFont;

struct FGuiScopedLock
{
	SRWLOCK* Lock;

	explicit FGuiScopedLock(SRWLOCK& InLock) : Lock(&InLock) { AcquireSRWLockExclusive(Lock); }
	~FGuiScopedLock() { ReleaseSRWLockExclusive(Lock); }

	FGuiScopedLock(const FGuiScopedLock&) = delete;
	FGuiScopedLock& operator=(const FGuiScopedLock&) = delete;
};

namespace GuiDetail
{
	constexpr size_t MaxLogLines = 5000;

	bool TakePendingLog(std::deque<std::string>& Out);

	ImFont* GetMonoFont();
	ImFont* GetHeadingFont();
	float GetUiScale();

	void StopConsoleMirror();

	GuiPanel* CreateConsolePanel();
	GuiPanel* CreateStatusPanel();
	GuiPanel* CreateNetworkPanel();
	GuiPanel* CreatePerformancePanel();
	GuiPanel* CreateOffsetsPanel();
}
