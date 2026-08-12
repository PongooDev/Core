#include "pch.h"

#include "Gui/Public/GuiInternal.h"

#include <vector>

namespace
{
	struct FMemoryCounters
	{
		SIZE_T WorkingSet;
		SIZE_T PeakWorkingSet;
		SIZE_T PagefileUsage;
		SIZE_T PeakPagefileUsage;
	};

	struct FProcessMemoryCounters
	{
		DWORD cb;
		DWORD PageFaultCount;
		SIZE_T PeakWorkingSetSize;
		SIZE_T WorkingSetSize;
		SIZE_T QuotaPeakPagedPoolUsage;
		SIZE_T QuotaPagedPoolUsage;
		SIZE_T QuotaPeakNonPagedPoolUsage;
		SIZE_T QuotaNonPagedPoolUsage;
		SIZE_T PagefileUsage;
		SIZE_T PeakPagefileUsage;
	};

	bool ReadMemory(FMemoryCounters& Out)
	{
		typedef BOOL(WINAPI* GetProcessMemoryInfoFn)(HANDLE, FProcessMemoryCounters*, DWORD);

		static GetProcessMemoryInfoFn Query = nullptr;
		static bool bResolved = false;

		if (!bResolved)
		{
			bResolved = true;
			if (HMODULE Kernel32 = GetModuleHandleW(L"kernel32.dll"))
				Query = (GetProcessMemoryInfoFn)GetProcAddress(Kernel32, "K32GetProcessMemoryInfo");
		}

		if (!Query)
			return false;

		FProcessMemoryCounters Counters = {};
		Counters.cb = sizeof(Counters);

		if (!Query(GetCurrentProcess(), &Counters, sizeof(Counters)))
			return false;

		Out.WorkingSet = Counters.WorkingSetSize;
		Out.PeakWorkingSet = Counters.PeakWorkingSetSize;
		Out.PagefileUsage = Counters.PagefileUsage;
		Out.PeakPagefileUsage = Counters.PeakPagefileUsage;
		return true;
	}

	std::string FormatBytes(SIZE_T Bytes)
	{
		const double Mb = (double)Bytes / (1024.0 * 1024.0);
		if (Mb >= 1024.0)
			return std::format("{:.2f} GB", Mb / 1024.0);

		return std::format("{:.1f} MB", Mb);
	}

	constexpr int HistorySize = 120;

	class PerformancePanel : public GuiPanel
	{
	public:
		PerformancePanel()
			: StartedAt(GetTickCount64())
		{
			MemoryHistory.resize(HistorySize, 0.0f);
			CpuHistory.resize(HistorySize, 0.0f);

			SYSTEM_INFO Info = {};
			GetSystemInfo(&Info);
			Cores = Info.dwNumberOfProcessors ? Info.dwNumberOfProcessors : 1;
		}

		const char* Name() const override { return "Performance"; }
		const char* Category() const override { return "DIAGNOSTICS"; }
		void Render() override;

	private:
		void Sample();

		ULONGLONG StartedAt = 0;
		ULONGLONG LastSample = 0;

		FMemoryCounters Memory = {};
		bool bHaveMemory = false;
		DWORD Handles = 0;
		DWORD Cores = 1;

		std::vector<float> MemoryHistory;
		float MemoryPeak = 1.0f;

		std::vector<float> CpuHistory;
		float CpuPercent = -1.0f;
		float CpuPeak = 10.0f;
		ULONGLONG PrevCpu100ns = 0;
		ULONGLONG PrevCpuWallMs = 0;
	};

	void PerformancePanel::Sample()
	{
		bHaveMemory = ReadMemory(Memory);

		if (!GetProcessHandleCount(GetCurrentProcess(), &Handles))
			Handles = 0;

		const float Mb = bHaveMemory ? (float)((double)Memory.WorkingSet / (1024.0 * 1024.0)) : 0.0f;

		MemoryHistory.erase(MemoryHistory.begin());
		MemoryHistory.push_back(Mb);

		MemoryPeak = 1.0f;
		for (float Value : MemoryHistory)
			if (Value > MemoryPeak)
				MemoryPeak = Value;

		FILETIME Creation, Exit, Kernel, User;
		if (GetProcessTimes(GetCurrentProcess(), &Creation, &Exit, &Kernel, &User))
		{
			ULARGE_INTEGER KernelTime, UserTime;
			KernelTime.LowPart = Kernel.dwLowDateTime;
			KernelTime.HighPart = Kernel.dwHighDateTime;
			UserTime.LowPart = User.dwLowDateTime;
			UserTime.HighPart = User.dwHighDateTime;

			const ULONGLONG Used = KernelTime.QuadPart + UserTime.QuadPart;
			const ULONGLONG NowMs = GetTickCount64();

			if (PrevCpuWallMs && NowMs > PrevCpuWallMs && Used >= PrevCpu100ns)
			{
				const double UsedMs = (double)(Used - PrevCpu100ns) / 10000.0;
				const double WallMs = (double)(NowMs - PrevCpuWallMs) * (double)Cores;

				float Percent = (float)(100.0 * UsedMs / WallMs);
				if (Percent > 100.0f)
					Percent = 100.0f;

				CpuPercent = Percent;
			}

			PrevCpu100ns = Used;
			PrevCpuWallMs = NowMs;
		}

		CpuHistory.erase(CpuHistory.begin());
		CpuHistory.push_back(CpuPercent > 0.0f ? CpuPercent : 0.0f);

		CpuPeak = 10.0f;
		for (float Value : CpuHistory)
			if (Value > CpuPeak)
				CpuPeak = Value;
	}

	void PerformancePanel::Render()
	{
		const ULONGLONG Now = GetTickCount64();
		if (Now - LastSample >= 1000)
		{
			LastSample = Now;
			Sample();
		}

		const ImGuiIO& IO = ImGui::GetIO();

		GuiDetail::SectionLabel("CPU");

		GuiDetail::LabelledValue("Usage", CpuPercent >= 0.0f ? std::format("{:.1f} %", CpuPercent) : std::string("-"));
		GuiDetail::LabelledValue("Logical cores", std::to_string(Cores));

		ImGui::Dummy(ImVec2(0, 6.0f));

		const std::string CpuOverlay = CpuPercent >= 0.0f
			? std::format("cpu - {:.1f} %", CpuPercent)
			: std::string("cpu - sampling...");

		ImGui::PushStyleColor(ImGuiCol_PlotLines, Theme::Accent);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, Theme::Recessed);

		ImGui::PlotLines("##Cpu", CpuHistory.data(), (int)CpuHistory.size(), 0,
			CpuOverlay.c_str(), 0.0f, CpuPeak * 1.15f,
			ImVec2(-1.0f, 80.0f * GuiDetail::GetUiScale()));

		ImGui::PopStyleColor(2);
		ImGui::TextDisabled("last %d seconds", (int)CpuHistory.size());

		GuiDetail::SectionLabel("MEMORY");

		if (bHaveMemory)
		{
			GuiDetail::LabelledValue("Working set", FormatBytes(Memory.WorkingSet));
			GuiDetail::LabelledValue("Peak working set", FormatBytes(Memory.PeakWorkingSet));
			GuiDetail::LabelledValue("Commit", FormatBytes(Memory.PagefileUsage));
			GuiDetail::LabelledValue("Peak commit", FormatBytes(Memory.PeakPagefileUsage));

			ImGui::Dummy(ImVec2(0, 6.0f));

			const std::string Overlay = std::format("working set - {:.1f} MB", MemoryHistory.back());
			ImGui::PushStyleColor(ImGuiCol_PlotLines, Theme::Accent);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Theme::Recessed);

			ImGui::PlotLines("##Memory", MemoryHistory.data(), (int)MemoryHistory.size(), 0,
				Overlay.c_str(), 0.0f, MemoryPeak * 1.15f,
				ImVec2(-1.0f, 80.0f * GuiDetail::GetUiScale()));

			ImGui::PopStyleColor(2);
			ImGui::TextDisabled("last %d seconds", (int)MemoryHistory.size());
		}
		else
		{
			ImGui::TextDisabled("Memory counters are unavailable on this system.");
		}

		GuiDetail::SectionLabel("PROCESS");

		GuiDetail::LabelledValue("Handles", Handles ? std::to_string(Handles) : "-");
		GuiDetail::LabelledValue("Process id", std::to_string(GetCurrentProcessId()));
		GuiDetail::LabelledValue("GUI uptime", GuiDetail::FormatDuration((Now - StartedAt) / 1000));

		GuiDetail::SectionLabel("INTERFACE");

		GuiDetail::LabelledValue("Frame time", std::format("{:.2f} ms", IO.DeltaTime * 1000.0f));
		GuiDetail::LabelledValue("Frame rate", std::format("{:.0f} fps", IO.Framerate));
		GuiDetail::LabelledValue("Draw calls", std::to_string(IO.MetricsRenderIndices / 3));
		GuiDetail::LabelledValue("Vertices", std::to_string(IO.MetricsRenderVertices));
	}
}

GuiPanel* GuiDetail::CreatePerformancePanel()
{
	return new PerformancePanel();
}
