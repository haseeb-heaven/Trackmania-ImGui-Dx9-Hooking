#include "Features.hpp"

void ImGuiMainLoop() {
	bool attached = false;
	if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
	{
		kiero::bind(42, (void**)&oEndScene, hkEndScene);
		g_Hwnd = GetForegroundWindow();
		oWndProc = (WNDPROC)SetWindowLongPtr(g_Hwnd, GWL_WNDPROC, (LONG_PTR)WndProc);
		attached = true;
	}

	//Main loop of DLL. 
	while (!GT_IsKeyPressed(VK_END)) {
		std::this_thread::sleep_for(10ms);
	}

	kiero::shutdown();
	SetWindowLongPtr(g_Hwnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
}

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	IMGUI_CHECKVERSION();
	auto pCtx = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange | ImGuiConfigFlags_NavEnableKeyboard;
	ImGui_ImplWin32_Init(g_Hwnd);
	ImGui_ImplDX9_Init(pDevice);
	//GT_ShowInfo("Init done....");
	//LOG_INFO("%s %s", __FUNCTION__, "Init done...");
}

bool init = false;
float test = 0.f;
long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!init)
	{
		InitImGui(pDevice);
		init = true;
	}
	ImGui::StyleColorsDark();

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//LOG_INFO("%s %s", __FUNCTION__, "Init NewFrame...");

	static bool no_titlebar = false;
	static bool no_border = true;
	static bool no_resize = false;
	static bool auto_resize = false;
	static bool no_move = false;
	static bool no_scrollbar = false;
	static bool no_collapse = false;
	static bool no_menu = true;
	static bool start_pos_set = false;
	ImVec4  clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGuiWindowFlags	window_flags = 0;
	if (no_titlebar)	window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_resize)		window_flags |= ImGuiWindowFlags_NoResize;
	if (auto_resize)	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	if (no_move)		window_flags |= ImGuiWindowFlags_NoMove;
	if (no_scrollbar)	window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (no_collapse)	window_flags |= ImGuiWindowFlags_NoCollapse;
	if (!no_menu)		window_flags |= ImGuiWindowFlags_MenuBar;
	ImGui::SetNextWindowSize(ImVec2(450, 600));
	if (!start_pos_set) { ImGui::SetNextWindowPos(ImVec2(25, 25)); start_pos_set = true; }
	ImGui::GetIO().MouseDrawCursor = init;

	if (init)
	{
		ImGui::SetNextWindowFocus();

		static float f = 0.0f;
		static int track_addr = 0;
		static bool track_info = false, show_ghost = false, show_names = false, game_data_info = false, logs_enabled = false;
		char buf[20] = { NULL };
		void* game_data_addr = (void*)0x00D71898;
		string log_file = g_Utility.GetModuleFolder() + "\\" + LOGGER_FILE;
		ImGui::Begin("TMNF - Helper @HM", &init, window_flags);

		//Show Logger.
		if (logs_enabled)
			g_ImGuiLogger.Draw("Trackmania Logs", &init);

		//ImGui::LogToFile(10, log_file.c_str());

		ImGui::InputText("Track Name", buf, 20);
		if (ImGui::Button("Show Track Info")) {
			track_info = !track_info;
			LOG_IMGUI("%s Track info selected %d\n", __FUNCTION__, track_info);
		}

		if (ImGui::Button("Show Game Data")) {
			game_data_info = !game_data_info;
			LOG_IMGUI("%s Game info selected %d\n", __FUNCTION__, game_data_info);
		}

		if (game_data_info) {
			try {
				LOG_IMGUI("%s Game data address 0x%X\n", __FUNCTION__, game_data_addr);
				string tm_data = GT_ReadString(game_data_addr, 150);
				////LOG_INFO("%s Game data %s", __FUNCTION__, tm_data.c_str());


				size_t login_info_pos = tm_data.find("login=");
				LOG_IMGUI("%s Login Info Position %d\n", __FUNCTION__, login_info_pos);

				size_t key_info_pos = tm_data.find("key=");
				LOG_IMGUI("%s Key Info Position %d\n", __FUNCTION__, key_info_pos);

				string login_info, key_info;

				if (login_info_pos != std::string::npos) {
					login_info = tm_data.substr(login_info_pos, 10);
					ImGui::Text("Key = %s", key_info);
				}

				if (login_info_pos != std::string::npos) {
					key_info = tm_data.substr(key_info_pos, 10);
					ImGui::Text("Login = %s", login_info);
				}

				if (logs_enabled) {
					LOG_IMGUI("%s key_info %s\n", __FUNCTION__, key_info.c_str());
					LOG_IMGUI("%s login_info %s\n", __FUNCTION__, login_info.c_str());
				}

			}
			catch (...) {

			}
		}

		if (track_info) {
			try {
				track_addr = std::stoi(buf, 0, 16);

				ImGui::Text("Addres Hex = %X", track_addr);
				auto track_name = (char*)track_addr;
				ImGui::Text("Name = %s", track_name);

				int track_day_addr = track_addr + 0x0019;
				int track_type_addr = track_addr + 0x00E0;
				int track_auth_addr = track_addr + 0x00EF;

				auto track_day = (char*)track_day_addr;
				ImGui::Text("Day = %s", track_day);

				auto track_type = (char*)track_type_addr;
				ImGui::Text("Type = %s", track_type);

				auto track_auth = (char*)track_auth_addr;
				ImGui::Text("Author = %s", track_auth);

				int track_coppers_addr = READ_STATIC_PTR_OFF3(0x00D6E978, 0x19C, 0x198, 0x104);
				ImGui::Text("Coppers = %d", *(int*)track_coppers_addr);
			}
			catch (...) {

			}
		}

		ImGui::Checkbox("Show Ghost", &show_ghost);
		ImGui::Checkbox("Show Names", &show_names);
		ImGui::Checkbox("Enable Logs", &logs_enabled);

		if (show_ghost) GT_WriteByte((LPVOID)0x00D560B0, show_ghost);
		if (show_names) GT_WriteByte((LPVOID)0x00D560DC, show_names);

		ImGui::Text("Game Frames (%.1f FPS)", ImGui::GetIO().Framerate);
		ImGui::End();
	}

	//LOG_INFO("%s %s", __FUNCTION__, "Init Rendering...");
	// Rendering
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	//LOG_INFO("%s %s", __FUNCTION__, "Init Rendering Done...");

	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	if (pDevice->BeginScene() >= 0)
	{
		pDevice->EndScene();
	}
	HRESULT result = pDevice->Present(NULL, NULL, NULL, NULL);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		GT_ShowInfo("Reset device");

	//ImGui_ImplDX9_Shutdown();
	//ImGui_ImplWin32_Shutdown();
	//ImGui::DestroyContext();
	//LOG_INFO("%s %s", __FUNCTION__, "Exiting method...");
	return oEndScene(pDevice);
}


LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}