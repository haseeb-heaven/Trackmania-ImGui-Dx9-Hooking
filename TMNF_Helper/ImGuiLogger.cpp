#include "ImGuiLogger.hpp"
using namespace TrackManiaTM;

ImGuiLogger::ImGuiLogger()
{
	im_auto_scroll = true;
	Clear();
}

void ImGuiLogger::Clear()
{
	im_buf.clear();
	im_lineOffsets.clear();
	im_lineOffsets.push_back(0);
}

void ImGuiLogger::AddLog(const char* fmt, ...) IM_FMTARGS(2)
{
	int old_size = im_buf.size();
	va_list args;
	va_start(args, fmt);
	im_buf.appendfv(fmt, args);
	va_end(args);
	for (int new_size = im_buf.size(); old_size < new_size; old_size++)
		if (im_buf[old_size] == '\n')
			im_lineOffsets.push_back(old_size + 1);
}

void ImGuiLogger::Draw(const char* title, bool* p_open)
{
	if (!ImGui::Begin(title, p_open))
	{
		ImGui::End();
		return;
	}

	// Options menu
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Auto-scroll", &im_auto_scroll);
		ImGui::EndPopup();
	}

	// Main window
	if (ImGui::Button("Options"))
		ImGui::OpenPopup("Options");
	ImGui::SameLine();
	bool clear = ImGui::Button("Clear");
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	im_filter.Draw("Filter", -100.0f);

	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	if (clear)
		Clear();
	if (copy)
		ImGui::LogToClipboard();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	const char* buf = im_buf.begin();
	const char* buf_end = im_buf.end();
	if (im_filter.IsActive())
	{
		for (int line_no = 0; line_no < im_lineOffsets.Size; line_no++)
		{
			const char* line_start = buf + im_lineOffsets[line_no];
			const char* line_end = (line_no + 1 < im_lineOffsets.Size) ? (buf + im_lineOffsets[line_no + 1] - 1) : buf_end;
			if (im_filter.PassFilter(line_start, line_end))
				ImGui::TextUnformatted(line_start, line_end);
		}
	}
	else
	{
		ImGuiListClipper clipper;
		clipper.Begin(im_lineOffsets.Size);
		while (clipper.Step())
		{
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
			{
				const char* line_start = buf + im_lineOffsets[line_no];
				const char* line_end = (line_no + 1 < im_lineOffsets.Size) ? (buf + im_lineOffsets[line_no + 1] - 1) : buf_end;
				ImGui::TextUnformatted(line_start, line_end);
			}
		}
		clipper.End();
	}
	ImGui::PopStyleVar();

	if (im_auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();
	ImGui::End();
}
