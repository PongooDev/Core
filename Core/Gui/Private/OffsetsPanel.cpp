#include "pch.h"

#include "Gui/Public/GuiInternal.h"

#include "../../../includes/imgui/imgui.h"

#include <vector>

namespace
{
	struct FOffsetEntry
	{
		const char* Name;
		const uintptr_t* Value;
	};

#define CORE_OFFSET_ENTRY(Name) { #Name, &ServerOffsets::Name },
	const FOffsetEntry GOffsetTable[] = { CORE_OFFSET_LIST(CORE_OFFSET_ENTRY) };
#undef CORE_OFFSET_ENTRY

	constexpr int OffsetCount = (int)(sizeof(GOffsetTable) / sizeof(GOffsetTable[0]));

	class OffsetsPanel : public GuiPanel
	{
	public:
		const char* Name() const override { return "Offsets"; }
		const char* Category() const override { return "DIAGNOSTICS"; }
		void Render() override;

	private:
		void Rebuild();

		std::vector<int> Visible;
		std::string AppliedFilter;
		bool bVisibleDirty = true;
		bool bUnresolvedOnly = false;

		ImGuiTextFilter Filter;
	};

	void OffsetsPanel::Rebuild()
	{
		Visible.clear();
		Visible.reserve(OffsetCount);

		for (int i = 0; i < OffsetCount; i++)
		{
			if (bUnresolvedOnly && *GOffsetTable[i].Value != 0)
				continue;

			if (Filter.PassFilter(GOffsetTable[i].Name))
				Visible.push_back(i);
		}

		bVisibleDirty = false;
	}

	void OffsetsPanel::Render()
	{
		const float Scale = GuiDetail::GetUiScale();

		int Resolved = 0;
		for (int i = 0; i < OffsetCount; i++)
			if (*GOffsetTable[i].Value != 0)
				++Resolved;

		if (AppliedFilter != Filter.InputBuf)
		{
			AppliedFilter = Filter.InputBuf;
			bVisibleDirty = true;
		}

		if (bVisibleDirty || bUnresolvedOnly)
			Rebuild();

		if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_F))
			ImGui::SetKeyboardFocusHere();
		Filter.Draw("Filter", 240.0f * Scale);

		ImGui::SameLine();
		if (ImGui::Checkbox("Unresolved only", &bUnresolvedOnly))
			bVisibleDirty = true;

		ImGui::SameLine();
		if (ImGui::Button("Copy"))
		{
			std::string Out;
			for (int Index : Visible)
				Out += std::format("{} = 0x{:X}\n", GOffsetTable[Index].Name, *GOffsetTable[Index].Value);

			ImGui::SetClipboardText(Out.c_str());
		}

		ImGui::SameLine();
		ImGui::TextDisabled("%d resolved, %d unresolved", Resolved, OffsetCount - Resolved);

		ImGui::Separator();

		constexpr ImGuiTableFlags Flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH |
			ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp;

		if (!ImGui::BeginTable("Offsets", 2, Flags, ImVec2(0, 0)))
			return;

		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, 130.0f * Scale);
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();

		ImGuiListClipper Clipper;
		Clipper.Begin((int)Visible.size());
		while (Clipper.Step())
		{
			for (int Row = Clipper.DisplayStart; Row < Clipper.DisplayEnd; Row++)
			{
				const FOffsetEntry& Entry = GOffsetTable[Visible[Row]];
				const uintptr_t Value = *Entry.Value;

				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted(Entry.Name);

				ImGui::TableSetColumnIndex(1);
				ImGui::PushFont(GuiDetail::GetMonoFont(), 0.0f);

				if (Value != 0)
					ImGui::TextColored(ImVec4(0.56f, 0.78f, 1.00f, 1.0f), "0x%llX", (unsigned long long)Value);
				else
					ImGui::TextColored(ImVec4(0.94f, 0.42f, 0.42f, 1.0f), "unresolved");

				ImGui::PopFont();
			}
		}
		Clipper.End();

		ImGui::EndTable();
	}
}

GuiPanel* GuiDetail::CreateOffsetsPanel()
{
	return new OffsetsPanel();
}
