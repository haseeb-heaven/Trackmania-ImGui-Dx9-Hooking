#pragma once
#include <imgui/imgui.h>

namespace TrackManiaTM {
	class ImGuiLogger
	{
		ImGuiTextBuffer  im_buf;
		ImGuiTextFilter  im_filter;
		ImVector<int>    im_lineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
		bool im_auto_scroll;  // Keep scrolling if already at the bottom.
	public:
		ImGuiLogger();
		~ImGuiLogger() = default;

		void Clear();
		void AddLog(const char* fmt, ...) IM_FMTARGS(2);
		void Draw(const char* title, bool* p_open = NULL);
	};
	inline ImGuiLogger g_ImGuiLogger;
}