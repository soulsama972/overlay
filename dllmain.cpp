// dllmain.cpp : Defines the entry point for the DLL application.
#define WIN32_LEAN_AND_MEAN    
#define  _CRT_NON_CONFORMING_SWPRINTFS
#include"BlackList.hpp"
using namespace std;



#ifdef _WIN64
extern "C" uintptr_t Jump = NULL;
#endif

void startLoop(HMODULE hModule)
{
	bList.Init();
	while (!GetAsyncKeyState(VK_END))
		Sleep(100);
	bList.CleanUp();
	FreeLibraryAndExitThread(hModule, 1);
}


BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:	
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)startLoop, hModule, NULL, NULL);break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

