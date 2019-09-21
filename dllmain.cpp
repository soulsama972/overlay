// dllmain.cpp : Defines the entry point for the DLL application.
#define WIN32_LEAN_AND_MEAN    
#define  _CRT_NON_CONFORMING_SWPRINTFS
#include<Windows.h>
#include<iostream>
#include"Overlay11.hpp"
#include"Overlay9.hpp"
#include"BlackList.hpp"
using namespace std;



extern "C" uintptr_t Jump = NULL;
FILE *f;

void startLoop(HMODULE hModule)
{
	bList.Init();
	while (1)
	{
		if (GetAsyncKeyState(VK_END))
			break;
		Sleep(100);
	}
	bList.Clean();
	FreeLibraryAndExitThread(hModule, 1);
}


BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		//AllocConsole();
		//freopen_s(&f, "CONOUT$", "w", stdout);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)startLoop, hModule, NULL, NULL);
	}
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
	{
	//	fclose(f);
	//	FreeConsole();
	}
        break;
    }
    return TRUE;
}

