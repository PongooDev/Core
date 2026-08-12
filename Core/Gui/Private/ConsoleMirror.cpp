#include "pch.h"

#include "Gui/Public/GuiInternal.h"

#include <vector>

namespace
{
	std::atomic<bool> GCaptureActive = false;
	std::atomic<bool> GScrapeRun = false;
	HANDLE GScrapeThread = nullptr;
	HANDLE GConsoleOut = nullptr;

	void PushRow(const wchar_t* Row, int Length)
	{
		while (Length > 0 && (Row[Length - 1] == L' ' || Row[Length - 1] == 0))
			--Length;

		if (Length <= 0)
			return;

		const int Size = WideCharToMultiByte(CP_UTF8, 0, Row, Length, nullptr, 0, nullptr, nullptr);
		if (Size <= 0)
			return;

		std::string Utf8(Size, 0);
		WideCharToMultiByte(CP_UTF8, 0, Row, Length, Utf8.data(), Size, nullptr, nullptr);
		Gui::PushLog(Utf8);
	}

	DWORD WINAPI ScrapeProc(LPVOID)
	{
		constexpr SHORT DesiredHeight = 30000;

		SHORT NextRow = 0;
		std::vector<wchar_t> Row;

		while (GScrapeRun)
		{
			CONSOLE_SCREEN_BUFFER_INFO Info;
			if (GConsoleOut && GetConsoleScreenBufferInfo(GConsoleOut, &Info))
			{
				if (Info.dwSize.Y < DesiredHeight)
				{
					COORD Grow = { Info.dwSize.X, DesiredHeight };
					SetConsoleScreenBufferSize(GConsoleOut, Grow);
					GetConsoleScreenBufferInfo(GConsoleOut, &Info);
				}

				const SHORT Width = Info.dwSize.X;
				const SHORT Cursor = Info.dwCursorPosition.Y;

				if (Cursor < NextRow)
					NextRow = 0;

				if (Width > 0)
				{
					Row.resize(Width);

					for (SHORT y = NextRow; y < Cursor; ++y)
					{
						DWORD Read = 0;
						COORD At = { 0, y };
						if (ReadConsoleOutputCharacterW(GConsoleOut, Row.data(), Width, At, &Read) && Read)
							PushRow(Row.data(), (int)Read);
					}

					NextRow = Cursor;
				}
			}

			Sleep(100);
		}

		return 0;
	}
}

bool Gui::IsCapturingConsole()
{
	return GCaptureActive;
}

void Gui::CaptureConsoleOutput()
{
	if (!IsRunning() || GCaptureActive)
		return;

	GConsoleOut = CreateFileW(L"CONOUT$", GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);

	CONSOLE_SCREEN_BUFFER_INFO Probe;
	if (GConsoleOut == INVALID_HANDLE_VALUE || !GetConsoleScreenBufferInfo(GConsoleOut, &Probe))
	{
		Log("Gui: could not open CONOUT$ (error " + std::to_string(GetLastError()) +
			") - engine logs will not appear in the console tab.");

		if (GConsoleOut != INVALID_HANDLE_VALUE)
			CloseHandle(GConsoleOut);
		GConsoleOut = nullptr;
		return;
	}

	GCaptureActive = true;
	GScrapeRun = true;
	GScrapeThread = CreateThread(nullptr, 0, ScrapeProc, nullptr, 0, nullptr);

	if (!GScrapeThread)
	{
		GScrapeRun = false;
		GCaptureActive = false;
		Log("Gui: failed to start the console mirror thread.");
	}
}

void GuiDetail::StopConsoleMirror()
{
	GScrapeRun = false;
	GCaptureActive = false;

	if (GScrapeThread)
	{
		CloseHandle(GScrapeThread);
		GScrapeThread = nullptr;
	}

	if (GConsoleOut && GConsoleOut != INVALID_HANDLE_VALUE)
	{
		CloseHandle(GConsoleOut);
		GConsoleOut = nullptr;
	}
}
