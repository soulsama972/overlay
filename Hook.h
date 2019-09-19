#pragma once
#include<Windows.h>
#include<TlHelp32.h>
#include<iostream>

#define  _CRT_SECURE_NO_WARNINGS



struct hook_t
{
	bool	isHooked = false;
	int     size = 0;
	LPVOID	addressToHook = NULL;
	LPVOID	newFunc = NULL;
	BYTE* oriByte = NULL;
	BYTE	jmp[5] = { 0 };
	uintptr_t jumpBack = 0;
};

struct Mod {
	uintptr_t  base = 0;
	uintptr_t end = 0;
};

namespace Hook {
	void apiHook(hook_t & hook, uintptr_t addrToHook, LPVOID funcAddrm,int size);
	void midHook(hook_t & hook, uintptr_t addrToHook, LPVOID funcAddr, int size);
	bool insertHook(hook_t& hook);
	bool unHooked(hook_t &hook);
	bool freeHook(hook_t& hook);
	uintptr_t Scan(uintptr_t base, uintptr_t end, const char* pattren, const char* mask);
}


