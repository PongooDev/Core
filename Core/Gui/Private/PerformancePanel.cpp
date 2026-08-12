#include "pch.h"

#include "Gui/Public/GuiInternal.h"

#include "../../../includes/imgui/imgui.h"

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

	constexpr int HistorySize = 120;

	class PerformancePanel : public GuiPanel
	{
	public:
		PerformancePanel()
			: StartedAt(GetTickCount64())
		{
			History.resize(HistorySize, 0.0f);
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

		std::vector<float> History;
		float PeakSample = 1.0f;
	};

	void PerformancePanel::Sample()
	{
		bHaveMemory = ReadMemory(Memory);

		if (!GetProcessHandleCount(GetCurrentProcess(), &Handles))
			Handles = 0;

		const float Mb = bHaveMemory ? (float)((double)Memory.WorkingSet / (1024.0 * 1024.0)) : 0.0f;

		History.erase(History.begin());
		History.push_back(Mb);

		PeakSample = 1.0f;
		for (float Value : History)
			if (Value > PeakSample)
				PeakSample = Value;
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

		SectionLabel("MEMORY");

		if (bHaveMemory)
		{
			LabelledValue("Working set", FormatBytes(Memory.WorkingSet));
			LabelledValue("Peak working set", FormatBytes(Memory.PeakWorkingSet));
			LabelledValue("Commit", FormatBytes(Memory.PagefileUsage));
			LabelledValue("Peak commit", FormatBytes(Memory.PeakPagefileUsage));

			ImGui::Dummy(ImVec2(0, 6.0f));

			const std::string Overlay = std::format("working set - {:.1f} MB", History.back());
			ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.07f, 0.84f, 0.63f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.09f, 0.10f, 0.11f, 1.0f));

			ImGui::PlotLines("##Memory", History.data(), (int)History.size(), 0,
				Overlay.c_str(), 0.0f, PeakSample * 1.15f,
				ImVec2(-1.0f, 80.0f * GuiDetail::GetUiScale()));

			ImGui::PopStyleColor(2);
			ImGui::TextDisabled("last %d seconds", (int)History.size());
		}
		else
		{
			ImGui::TextDisabled("Memory counters are unavailable on this system.");
		}

		SectionLabel("PROCESS");

		LabelledValue("Handles", Handles ? std::to_string(Handles) : "-");
		LabelledValue("Process id", std::to_string(GetCurrentProcessId()));

		const ULONGLONG Seconds = (Now - StartedAt) / 1000;
		LabelledValue("GUI uptime", std::format("{:02}:{:02}:{:02}",
			Seconds / 3600, (Seconds / 60) % 60, Seconds % 60));

		SectionLabel("INTERFACE");

		LabelledValue("Frame time", std::format("{:.2f} ms", IO.DeltaTime * 1000.0f));
		LabelledValue("Frame rate", std::format("{:.0f} fps", IO.Framerate));
		LabelledValue("Draw calls", std::to_string(IO.MetricsRenderIndices / 3));
		LabelledValue("Vertices", std::to_string(IO.MetricsRenderVertices));
	}
}

GuiPanel* GuiDetail::CreatePerformancePanel()
{
	return new PerformancePanel();
}
