#ifndef __LOGGER_H__ 
#define __LOGGER_H__ 

#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 

#include "Common.hpp" 
#define LOG_IMGUI g_ImGuiLogger.AddLog
#define LOG_INFO( fmt, ...) g_Log->WriteA(true,true, ELOG_TYPE::TYPE_PRINT, fmt, ##__VA_ARGS__ )
#define LOG_DEBUG( fmt, ...) g_Log->WriteA(true,true,  ELOG_TYPE::TYPE_DEBUG, fmt, ##__VA_ARGS__ )
#define LOG_WARNING( fmt, ...) g_Log->WriteA(true,true,  ELOG_TYPE::TYPE_WARNING, fmt, ##__VA_ARGS__ )
#define LOG_ERROR( fmt, ...) g_Log->WriteA(true,true,  ELOG_TYPE::TYPE_ERROR, fmt, ##__VA_ARGS__ )
#define LOG_CONSOLE( fmt, ...) g_Log->WriteA(true,false,  ELOG_TYPE::TYPE_PRINT, fmt, ##__VA_ARGS__ )
#define LOG_FILE( fmt, ...) g_Log->WriteA(false,true,  ELOG_TYPE::TYPE_PRINT, fmt, ##__VA_ARGS__ )
#define LOG_RAW(raw_str) g_Log->WriteW(false,true,ELOG_TYPE::TYPE_PRINT,raw_str)
#define LOG_RAW_CONSOLE(raw_str) g_Log->WriteW(true,false,ELOG_TYPE::TYPE_PRINT,raw_str)

namespace TrackManiaTM {

	enum ELOG_TYPE {
		TYPE_PRINT,
		TYPE_DEBUG,
		TYPE_WARNING,
		TYPE_ERROR,
	};

	class Log {
	public:

		Log();
		~Log();

		void    WriteA(bool, bool, ELOG_TYPE log_type, const char* fmt, ...);
		void    WriteW(bool, bool, ELOG_TYPE log_type, const wchar_t* buff);

	private:

		void    LogFileA(const char* buff);
		void	LogFileW(const wchar_t* buff);
		const string GetTimeFormatted();


		std::map <int32_t, int32_t> g_Log_Color;
		std::map <int32_t, string>  g_Log_Format;
		bool    first_entry = true;
	};

	inline Log* g_Log{};
}

#endif // __LOGGER_H__