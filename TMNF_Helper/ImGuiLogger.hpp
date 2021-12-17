#pragma once
#include <imgui/imgui.h>

namespace TrackManiaTM {
	class ImGuiLogger
	{
		ImGuiTextBuffer  Buf;
		ImGuiTextFilter  Filter;
		ImVector<int>    LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
		bool AutoScroll;  // Keep scrolling if already at the bottom.
	public:
		ImGuiLogger();
		~ImGuiLogger() = default;

		void Clear();
		void AddLog(const char* fmt, ...) IM_FMTARGS(2);
		void Draw(const char* title, bool* p_open = NULL);
	};
	inline ImGuiLogger g_ImGuiLogger;
}