#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "kiero/kiero.h"
#include "kiero/minhook/include/MinHook.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#include "GTLibc.hpp" 
#include "Utility.hpp"
#include "Logger.hpp"
#include "DbgHelper.hpp"
#include "ImGuiLogger.hpp"

using namespace TrackManiaTM;

//Include all static libraries for project. 
#if defined(_M_IX86) 
#if defined(DBG_x86)
//#pragma comment(lib,"hook/libMinHook-x86-Debug.lib")
#ifdef USE_GTLIBC_LIB 
#pragma comment(lib,"libs/GTLibc-x86-Debug.lib")
#pragma comment(lib,"libs/GTConsole-x86-Debug.lib")
#endif 

#elif defined(RLS_x86)
#pragma comment(lib,"hook/libMinHook-x86-Release.lib")
#ifdef USE_GTLIBC_LIB 
#pragma comment(lib,"libs/GTLibc-x86-Release.lib")
#pragma comment(lib,"libs/GTConsole-x86-Release.lib")
#endif 
#endif 
#else 
#error This project supports only x86 (32-Bit) builds. 
#endif 

#ifdef _WIN64
#define GWL_WNDPROC GWLP_WNDPROC
#endif

static HWND g_Hwnd = NULL;
namespace fs = std::filesystem;
using namespace std::chrono_literals;
inline HMODULE g_Hmodule{};
inline HANDLE g_Main_Thread{};
inline DWORD g_Main_Thread_Id{};

void ImGuiMainLoop();