#pragma once
#include "pch.h"

#include "Gui/Public/GuiPanel.h"

class Gui
{
public:
	static bool IsEnabled();
	static bool IsRunning();
	static void Start();
	static void Shutdown();

	static void RegisterPanel(GuiPanel* Panel);

	static void CaptureConsoleOutput();
	static bool IsCapturingConsole();

	static void PushLog(const std::string& Line);
	static void SetTitle(const std::string& Title);
};
