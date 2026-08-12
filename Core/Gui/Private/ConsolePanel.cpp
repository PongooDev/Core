#include "pch.h"

#include "Gui/Public/GuiInternal.h"

#include "../../../includes/imgui/imgui.h"

#include <deque>
#include <vector>

namespace
{
	enum class ELogSeverity : uint8_t
	{
		Normal,
		Core,
		Warning,
		Error,
	};

	ELogSeverity Classify(const std::string& Line)
	{
		if (Line.find("Error:") != std::string::npos || Line.find("Fatal:") != std::string::npos)
			return ELogSeverity::Error;

		if (Line.find("Warning:") != std::string::npos)
			return ELogSeverity::Warning;

		if (Line.rfind("LogServer:", 0) == 0 || Line.rfind("LogClient:", 0) == 0)
			return ELogSeverity::Core;

		return ELogSeverity::Normal;
	}

	bool SeverityColor(ELogSeverity Severity, ImVec4& OutColor)
	{
		switch (Severity)
		{
		case ELogSeverity::Error:   OutColor = ImVec4(1.00f, 0.42f, 0.42f, 1.0f); return true;
		case ELogSeverity::Warning: OutColor = ImVec4(1.00f, 0.84f, 0.40f, 1.0f); return true;
		case ELogSeverity::Core:    OutColor = ImVec4(0.56f, 0.78f, 1.00f, 1.0f); return true;
		default:                    return false;
		}
	}

	struct FLogLine
	{
		std::string Text;
		ELogSeverity Severity = ELogSeverity::Normal;
	};

	class ConsolePanel : public GuiPanel
	{
	public:
		const char* Name() const override { return "Console"; }
		void Render() override;

	private:
		void Drain();
		void RebuildVisible();
		void CopyVisibleToClipboard() const;

		std::deque<FLogLine> Lines;

		std::vector<int> Visible;
		std::string AppliedFilter;
		bool bVisibleDirty = true;

		ImGuiTextFilter Filter;
		bool bAutoScroll = true;
	};

	void ConsolePanel::Drain()
	{
		std::deque<std::string> Incoming;
		if (!GuiDetail::TakePendingLog(Incoming))
			return;

		for (std::string& Text : Incoming)
		{
			FLogLine Line;
			Line.Severity = Classify(Text);
			Line.Text = std::move(Text);
			Lines.push_back(std::move(Line));
		}

		while (Lines.size() > GuiDetail::MaxLogLines)
			Lines.pop_front();

		bVisibleDirty = true;
	}

	void ConsolePanel::RebuildVisible()
	{
		Visible.clear();
		Visible.reserve(Lines.size());

		for (int i = 0; i < (int)Lines.size(); i++)
			if (Filter.PassFilter(Lines[i].Text.c_str()))
				Visible.push_back(i);

		bVisibleDirty = false;
	}

	void ConsolePanel::CopyVisibleToClipboard() const
	{
		std::string Out;
		for (int Index : Visible)
		{
			Out += Lines[Index].Text;
			Out += '\n';
		}

		ImGui::SetClipboardText(Out.c_str());
	}

	void ConsolePanel::Render()
	{
		Drain();

		if (AppliedFilter != Filter.InputBuf)
		{
			AppliedFilter = Filter.InputBuf;
			bVisibleDirty = true;
		}

		if (bVisibleDirty)
			RebuildVisible();

		const bool bNewLines = !Visible.empty() && bAutoScroll;

		if (ImGui::Button("Clear"))
		{
			Lines.clear();
			bVisibleDirty = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Copy"))
			CopyVisibleToClipboard();
		ImGui::SameLine();
		ImGui::Checkbox("Auto-scroll", &bAutoScroll);
		ImGui::SameLine();
		Filter.Draw("Filter", 200.0f);

		ImGui::SameLine();
		if (Filter.IsActive())
			ImGui::TextDisabled("%d / %d lines", (int)Visible.size(), (int)Lines.size());
		else
			ImGui::TextDisabled("%d lines", (int)Lines.size());

		ImGui::Separator();

		if (ImGui::BeginChild("Scroll", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

			ImGuiListClipper Clipper;
			Clipper.Begin((int)Visible.size());
			while (Clipper.Step())
			{
				for (int Row = Clipper.DisplayStart; Row < Clipper.DisplayEnd; Row++)
				{
					const FLogLine& Line = Lines[Visible[Row]];

					ImVec4 Color;
					const bool bColored = SeverityColor(Line.Severity, Color);

					if (bColored)
						ImGui::PushStyleColor(ImGuiCol_Text, Color);

					ImGui::TextUnformatted(Line.Text.c_str());

					if (bColored)
						ImGui::PopStyleColor();
				}
			}
			Clipper.End();

			ImGui::PopStyleVar();

			if (bNewLines && ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 5.0f)
				ImGui::SetScrollHereY(1.0f);
		}
		ImGui::EndChild();
	}
}

GuiPanel* GuiDetail::CreateConsolePanel()
{
	return new ConsolePanel();
}
