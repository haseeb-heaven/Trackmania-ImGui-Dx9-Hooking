#include "Features.hpp"

void ImGuiMainLoop() {

	//Init Window and API.
	bool attached = false;
	if (kiero::initImGui(kiero::RenderType::D3D9) == kiero::Status::Success)
	{
		kiero::bind(42, (void**)&oEndScene, hkEndScene);//42 - EndScene DirectX -Api method.
		g_Hwnd = GetForegroundWindow();
		oWndProc = (WNDPROC)SetWindowLongPtr(g_Hwnd, GWL_WNDPROC, (LONG_PTR)WndProc);
		attached = true;
	}

	//Main loop of DLL. 
	while (!GT_IsKeyPressed(VK_END)) {
		std::this_thread::sleep_for(10ms);
	}

	//Shutting down Window and API.
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
	LOG_IMGUI("%s %s\n", __FUNCTION__, "Init done...");
}

long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!initImGui)
	{
		InitImGui(pDevice);
		initImGui = true;
	}
	ImGui::StyleColorsDark();

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (initImGui) {
		ImGuiFeaturesWindow();
	}

	// Rendering
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

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

	return oEndScene(pDevice);
}

void ImGuiFeaturesWindow() {
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
	ImGui::GetIO().MouseDrawCursor = initImGui;

	ImGui::SetNextWindowFocus();

	static float f = 0.0f;
	static int track_addr = 0;
	static bool track_info = false, show_ghost = false, show_names = false, game_data_info = false, logs_enabled = false;
	static bool login_info_cb = false, track_editor_cb = false;
	char buf[20] = { NULL };
	void* game_data_addr = (void*)0x00D71898;
	string log_file = g_Utility.GetModuleFolder() + "\\" + LOGGER_FILE;
	ImGui::Begin("TMNF - Helper @HM", &initImGui, window_flags);

	//Show Logger.
	if (logs_enabled)
		g_ImGuiLogger.Draw("Trackmania Logs", &initImGui);

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
	ImGui::SameLine();
	ImGui::Checkbox("Login Info", &login_info_cb);
	ImGui::SameLine();
	ImGui::Checkbox("Editor Info", &track_editor_cb);

	if (game_data_info) {
		try {
			string login_tag = "login=", key_tag = "key=", version_tag = "exever=", track_uuid_tag = "uid=", time_best_tag = "best=", respawns_tag = "respawns=",
				score_tag = "stuntscore=", validable_tag = "validable=", coppers_tag = "price=",
				time_bronze_tag = "bronze=", time_silver_tag = "silver=", time_gold_tag = "gold=", time_author_tag = "authortime=",
				track_name_tag = "name=", track_author_tag = "author=", track_envir_tag = "envir=", track_mood_tag = "mood=", track_type_tag = "type=", laps_tag = "nblaps=",
				end_tag = "&", sep_tag = " ", xml_end_tag = "/>";

			binary_t tm_data_vec(400, '\0');
			std::memcpy(tm_data_vec.data(), game_data_addr, tm_data_vec.capacity());
			std::string tm_data(tm_data_vec.begin(), tm_data_vec.end());

			if (login_info_cb) {
				ImGuiShowGameData(tm_data, key_tag, end_tag, "Key");
				ImGuiShowGameData(tm_data, version_tag, end_tag, "Login");
				ImGuiShowGameData(tm_data, version_tag, xml_end_tag, "Version");
			}

			if (track_editor_cb) {

				string track_editor_data;

				//Show General game data.
				ImGuiShowGameData(tm_data, track_name_tag, sep_tag, "Name");
				ImGuiShowGameData(tm_data, track_author_tag, xml_end_tag, "Author");
				ImGuiShowGameData(tm_data, track_type_tag, sep_tag, "Type");
				ImGuiShowGameData(tm_data, track_mood_tag, sep_tag, "Mood");
				ImGuiShowGameData(tm_data, track_envir_tag, sep_tag, "Environment");
				ImGuiShowGameData(tm_data, track_uuid_tag, sep_tag, "ID");
				ImGuiShowGameData(tm_data, coppers_tag, sep_tag, "Coppers");

				//Show Time data.
				ImGuiShowTimeData(tm_data, time_author_tag, sep_tag, "Author Time");
				ImGuiShowTimeData(tm_data, time_gold_tag, sep_tag, "Gold Time");
				ImGuiShowTimeData(tm_data, time_silver_tag, sep_tag, "Silver Time");
				ImGuiShowTimeData(tm_data, time_bronze_tag, sep_tag, "Bronze Time");
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
	ImGui::SameLine();
	ImGui::Checkbox("Show Names", &show_names);

	ImGui::Checkbox("Enable Logs", &logs_enabled);
	//if (ImGui::Button("Exit Application")) { GT_DoKeyPress(VK_END); }

	if (show_ghost) GT_WriteByte((LPVOID)0x00D560B0, show_ghost);
	if (show_names) GT_WriteByte((LPVOID)0x00D560DC, show_names);

	ImGui::Text("Game Frames (%.1f FPS)", ImGui::GetIO().Framerate);
	ImGui::End();
}


void ImGuiShowGameData(string game_data, string start_tag, string end_tag, string data_text) {
	string game_data_info = g_Utility.Slice(game_data, start_tag, end_tag); data_text += " : %s";
	if (!g_Utility.IsStringEmpty(game_data_info)) ImGui::Text(data_text.c_str(), game_data_info.c_str());
}

void ImGuiShowTimeData(string game_data, string start_tag, string end_tag, string data_text) {
	float track_time = 0.0f;
	string game_data_info = g_Utility.Slice(game_data, start_tag, end_tag); data_text += " : %0.3f";
	g_Utility.Replace(game_data_info, "\"", "");
	track_time = std::stoi(game_data_info, 0, 10) / 1000.0f;

	if (!g_Utility.IsStringEmpty(game_data_info)) ImGui::Text(data_text.c_str(), track_time);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}