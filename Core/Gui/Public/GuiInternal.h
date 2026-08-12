#pragma once
#include "pch.h"

#include "Gui/Public/Gui.h"

#include <deque>

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

	void StopConsoleMirror();

	GuiPanel* CreateConsolePanel();
	GuiPanel* CreateStatusPanel();
}
