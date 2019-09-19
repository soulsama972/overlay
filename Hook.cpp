#include "Hook.h"


void Hook::apiHook(hook_t & hook, uintptr_t addrToHook, LPVOID funcAddr,int size)
{
#ifdef _WIN64
	for (uintptr_t i = 0; i < 65535; i++)
	{
		hook.newFunc = VirtualAlloc((LPVOID)((uintptr_t)addrToHook - (i * 16384)), 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (hook.newFunc != NULL)
			break;
	}

	if (hook.newFunc)
	{
		bool Hooked = *(BYTE*)addrToHook == 0xE9;
		hook.oriByte = new BYTE[size];
		hook.addressToHook = (BYTE*)addrToHook;
		hook.size = size;
		hook.jmp[0] = 0xE9;
		*(uintptr_t*)&hook.jmp[1] = (uintptr_t)(hook.newFunc) - (uintptr_t)(addrToHook)-5;
		memcpy(hook.oriByte, (BYTE*)addrToHook, size);

		if (uintptr_t(hook.newFunc) - uintptr_t(funcAddr) > 0xFFFFFFFF) 
		{
			memset(hook.newFunc, 0x0, 14);
			*(WORD*)((BYTE*)hook.newFunc) = 0x25FF;
			*(uintptr_t*)((BYTE*)hook.newFunc + 6) = (uintptr_t)funcAddr;

			if (Hooked)
			{
				*(BYTE*)((BYTE*)hook.newFunc+14) = 0xE9;
				*(uintptr_t*)((uintptr_t)hook.newFunc + 15) = *(uintptr_t*)& hook.oriByte[1] + (uintptr_t)hook.addressToHook - (uintptr_t)hook.newFunc -14;
			}
			else
			{
				memcpy(LPVOID((BYTE*)hook.newFunc + 14), hook.oriByte, size);
				*(BYTE*)((BYTE*)hook.newFunc + 14 + size) = 0xE9;
				*(uintptr_t*)((BYTE*)hook.newFunc + size + 15) = (uintptr_t)hook.addressToHook - (uintptr_t)hook.newFunc - size - 14;
			}		
			hook.jumpBack = (uintptr_t)hook.newFunc + 14;
		}
		else 
		{
			if (Hooked)
			{
				*(BYTE*)hook.newFunc = 0xE9;
				*(uintptr_t*)((uintptr_t)hook.newFunc + 1) = *(uintptr_t*)& hook.oriByte[1] + (uintptr_t)hook.addressToHook - (uintptr_t)hook.newFunc;
			}
			else
			{
				memcpy(hook.newFunc, hook.oriByte, size);
				*(BYTE*)((uintptr_t)hook.newFunc + size) = 0xE9;
				*(uintptr_t*)((uintptr_t)hook.newFunc + size + 1) = (uintptr_t)hook.addressToHook - (uintptr_t)hook.newFunc - 5;
			}
			hook.jumpBack = (uintptr_t)hook.newFunc;


			//*(BYTE*)((BYTE*)hook.newFunc) = 0xE9;
			//*(uintptr_t*)((BYTE*)hook.newFunc + 1) = (uintptr_t)funcAddr - (uintptr_t)hook.newFunc - 5;

			//memcpy(LPVOID((BYTE*)hook.newFunc + 5), hook.oriByte, size);

			//*(BYTE*)((BYTE*)hook.newFunc + size + 5) = 0xE9;
			//*(uintptr_t*)((BYTE*)hook.newFunc + size + 6) = (uintptr_t)hook.addressToHook  - (uintptr_t)hook.newFunc + size - 5;

			//hook.jumpBack = (uintptr_t)hook.newFunc + 5;
		}
	}
	else
		std::cout << "cannot find mem to alloc" << std::endl;



#else
	hook.newFunc = VirtualAlloc(NULL, 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (hook.newFunc == NULL)
	{
		MessageBoxA(NULL, "bad alloc for incj", "title", MB_OK);
		exit(-1);
	}
	bool Hooked = *(BYTE*)addrToHook == 0xE9;

	hook.oriByte = new BYTE[size];
	memcpy(hook.oriByte, (BYTE*)addrToHook, size);
	hook.addressToHook = (BYTE*)addrToHook;
	hook.size = size;
	hook.jmp[0] = 0xE9;
	*(uintptr_t*)& hook.jmp[1] = (uintptr_t)(funcAddr)-(uintptr_t)(addrToHook)-5;
	if (Hooked)
	{
		*(BYTE*)hook.newFunc = 0xE9;
		*(uintptr_t*)((uintptr_t)hook.newFunc + 1) = *(uintptr_t*)&hook.oriByte[1] + (uintptr_t)hook.addressToHook - (uintptr_t)hook.newFunc;		
	}
	else
	{
		memcpy(hook.newFunc, hook.oriByte, size);
		*(BYTE*)((uintptr_t)hook.newFunc + size) = 0xE9;
		*(uintptr_t*)((uintptr_t)hook.newFunc + size + 1) = (uintptr_t)hook.addressToHook - (uintptr_t)hook.newFunc - 5;
	}
	hook.jumpBack = (uintptr_t)hook.newFunc;
#endif 
}

void Hook::midHook(hook_t & hook, uintptr_t addrToHook, LPVOID funcAddr, int size)
{
#ifdef _WIN64 
	for (uintptr_t i = 0; i < 65535; i++) 
	{
		hook.newFunc = VirtualAlloc((LPVOID)((uintptr_t)addrToHook - (i * 16384)), 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (hook.newFunc != NULL)
			break;
	}
	if (hook.newFunc)
	{
		hook.oriByte = new BYTE[size];
		memcpy(hook.oriByte, (LPVOID)addrToHook, size);
		hook.addressToHook = (LPVOID)addrToHook;
		hook.size = size;
		hook.jmp[0] = 0xE9;
		*(uintptr_t*)&hook.jmp[1] = (uintptr_t)(hook.newFunc) - (uintptr_t)(addrToHook)-5;

		if (uintptr_t(hook.newFunc) - uintptr_t(funcAddr) > 0xFFFFFFFF)
		{
			memset(hook.newFunc, 0x0, 14);
			*(WORD*)((BYTE*)hook.newFunc) = 0x25FF;
			*(uintptr_t*)((BYTE*)hook.newFunc + 6) = (uintptr_t)funcAddr;

			*(BYTE*)((BYTE*)hook.newFunc + 14) = 0xE9;
			*(uintptr_t*)((BYTE*)hook.newFunc + 15) = addrToHook - (uintptr_t)hook.newFunc - 14;

			hook.jumpBack = (uintptr_t)hook.newFunc + 14;
		}
		else
		{
			*(BYTE*)((BYTE*)hook.newFunc) = 0xE9;
			*(uintptr_t*)((BYTE*)hook.newFunc + 1) = (uintptr_t)funcAddr - (uintptr_t)hook.newFunc - 5;

			*(BYTE*)((BYTE*)hook.newFunc + 5) = 0xE9;
			*(uintptr_t*)((BYTE*)hook.newFunc + 6) = addrToHook - (uintptr_t)hook.newFunc + size - 5;

			hook.jumpBack = (uintptr_t)hook.newFunc + 5;
		}
	}



#else
	hook.oriByte = new BYTE[size];
	memcpy(hook.oriByte, (LPVOID)addrToHook, size);
	hook.addressToHook = (LPVOID)addrToHook;
	hook.size = size;
	hook.jmp[0] = 0xE9;
	*(uintptr_t*)&hook.jmp[1] = (uintptr_t)(funcAddr)-(uintptr_t)(addrToHook)-5;
	hook.jumpBack = addrToHook + 5;

#endif 
}




bool Hook::unHooked(hook_t& hook)
{
	if (!hook.isHooked)
		return false;
	DWORD old = NULL;
	DWORD temp = NULL;

	VirtualProtect(hook.addressToHook, hook.size, PAGE_EXECUTE_READWRITE, reinterpret_cast<DWORD*>(&old));
	memcpy(hook.addressToHook, hook.oriByte, hook.size);
	VirtualProtect(hook.addressToHook, hook.size, old, &temp);

	return true;
}

bool Hook::freeHook(hook_t &hook)
{
	if(hook.newFunc)
		VirtualFree(hook.newFunc,0 , MEM_RELEASE);
	delete hook.oriByte;
	if (hook.newFunc == NULL && hook.oriByte == NULL)
		return true;
	return false;
}

bool Hook::insertHook(hook_t& hook)
{
	DWORD old = NULL;
	DWORD temp = NULL;
	VirtualProtect(hook.addressToHook, hook.size, PAGE_EXECUTE_READWRITE, reinterpret_cast<DWORD*>(&old));
	memset(hook.addressToHook, 0x90, hook.size);
	memcpy(hook.addressToHook, hook.jmp, 5);
	VirtualProtect(hook.addressToHook, hook.size, old, &temp);
	hook.isHooked = true;
	return true;
}




uintptr_t Hook::Scan(uintptr_t base, uintptr_t end, const char * pattren, const char * mask)
{
	size_t len = strlen(mask);
	for (uintptr_t i = 0; i < end; i++) {
		bool found = true;
		for (size_t j = 0; j < len - 1; j++) {
			if (mask[j] != '?' && *(char*)(base + i + j) != pattren[j]) {
				found = false;
				break;
			}
		}
		if (found) {
			return base + i;
		}
	}
	return 0;
}

