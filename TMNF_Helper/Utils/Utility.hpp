#pragma once 
#ifndef __UTILS_H__ 
#define __UTILS_H__ 

#include "Common.hpp" 
using std::string;
enum { BINARY_FILE, ASCII_FILE, HEX_FILE };
#define ReadFileType(file,FILE_TYPE) std::get<FILE_TYPE>(g_Utility.ReadFile(file, FILE_TYPE));
#define WriteFileType(file,data,FILE_TYPE) g_Utility.WriteFile(file,data,FILE_TYPE);

namespace TrackManiaTM {
	class Utility {
	public:
		Utility() = default;
		~Utility() = default;

		const string GetExecutableFolder();
		const string GetModuleFolder();
		void    SetModuleHandle(const HMODULE);
		const HMODULE  GetModuleHandle();
		void    SetHandle(const HANDLE);
		const HANDLE  GetHandle();
		string   GetBaseFileName(string);
		string   GetLastErrorString();
		const string GetModuleName(const HMODULE);
		const string GetModuleNameExtension(const HMODULE);
		std::tuple<std::vector<uint8_t>, string, string>  ReadFile(string, int);
		bool WriteFile(string, binary_t,int);
		bool RemoveFile(const string&);
		bool WriteMemory(LPVOID, binary_t&);
		bool StrCaseCompare(const string&, const string&);
		size_t Find(string& data,string& search, size_t pos=0);
		bool IsAsciiStr(const string&);
		void Tokenize(string&,char,vector<string>&);
		void Replace(string& str, const string& from, const string& to);
		int ShellExec(string shell_cmd);
		string Trim(const string& str, const string& whitespace = " ");
		string GetAppdataPath();
		bool IsStringEmpty(const string& str);

	};
	inline Utility g_Utility;
}
#endif // __UTILITY_H__