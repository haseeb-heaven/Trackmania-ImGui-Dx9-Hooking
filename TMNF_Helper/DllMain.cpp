#include "DllMain.h"

BOOL WINAPI  DllMain(HMODULE hmod, DWORD reason, PVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hmod);
		g_Utility.SetModuleHandle(hmod);
		g_Hmodule = hmod;

		g_Main_Thread = CreateThread(nullptr, 0, [](PVOID)->DWORD
			{
				auto logger_instance = std::make_unique<ImGuiLogger>();
				try {

					LOG_IMGUI("Logger initialized.\n");

					//auto memory_instance = std::make_unique<Memory>(true);
					LOG_IMGUI("Memory initialized.\n");

#ifdef  USE_MINHOOK_LIB 
					// Initialize Hooking. 
					auto hook_instance = std::make_unique<Hook>(true);
					LOG_WARNING("Hook initialized.\n");
#endif //USE_MINHOOK_LIB 

#if  defined(USE_STACKTRACE_LIB) && defined(DBG_x86)
					auto dbg_instance = std::make_unique<DbgHelper>(true);
					LOG_IMGUI("DbgHelper initialized.\n");
#endif//USE_STACKTRACE_LIB 

					ImGuiMainLoop();


#ifdef  USE_MINHOOK_LIB
					hook_instance.reset();
					LOG_WARNING("Hook uninitialized.\n");
#endif
#if  defined(USE_STACKTRACE_LIB) && defined(DBG_x86)
					dbg_instance.reset();
					LOG_IMGUI("DbgHelper uninitialized\n.");
#endif

				}
				catch (std::exception const& ex) {
					GT_ShowError(ex.what());
#if  defined(USE_STACKTRACE_LIB) && defined(DBG_x86)
					g_DbgHelper->StackTrace(true);
#endif
				}

				LOG_IMGUI("Logger uninitialized.\n");
				logger_instance.reset();

				CloseHandle(g_Main_Thread);
				FreeLibraryAndExitThread(g_Hmodule, 0);
			}, nullptr, 0, &g_Main_Thread_Id);
	}
	return true;
}
