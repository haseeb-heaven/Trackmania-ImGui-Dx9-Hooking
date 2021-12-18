#pragma once
#include "DllMain.h"
typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern long __stdcall hkEndScene(LPDIRECT3DDEVICE9);
extern LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ImGuiShowGameData(string game_data, string start_tag, string end_tag = "&", string data_text = "Game Data");
void ImGuiShowTimeData(string game_data, string start_tag, string end_tag = " ", string data_text = "Game Data");
void ImGuiFeaturesWindow();
EndScene oEndScene = NULL;
WNDPROC oWndProc;
bool initImGui = false;