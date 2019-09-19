#pragma once


#pragma warning (push)
#pragma warning (disable : 26495)
#pragma warning (disable : 4005)
#include<d3d11.h> 
#include<d3d9.h>
#include<Windows.h>
#include<TlHelp32.h>
#pragma warning(pop)

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3d11.lib")



namespace Utill
{

	inline UINT_PTR  GetD3D11Present()
	{
		ID3D11Device* pDevice = NULL;
		ID3D11DeviceContext* pContext = NULL;

		UINT_PTR* pSwapChainVtable = NULL;
		UINT_PTR* pDeviceContextVTable = NULL;
		UINT_PTR  PresentAddr = 0;

		HWND hWnd = GetForegroundWindow();
		IDXGISwapChain* pSwapChain;

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = TRUE;//((GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
			NULL, NULL, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, NULL, &pContext)))
		{
			MessageBoxA(hWnd, "Failed to create directX device and swapchain!", "uBoos?", MB_ICONERROR);

		}
		if (pContext && pSwapChain)
		{
			pSwapChainVtable = (UINT_PTR*)pSwapChain;
			pSwapChainVtable = (UINT_PTR*)pSwapChainVtable[0];

			pDeviceContextVTable = (UINT_PTR*)pContext;
			pDeviceContextVTable = (UINT_PTR*)pDeviceContextVTable[0];
			PresentAddr = *(UINT_PTR*)& pSwapChainVtable[8];
		}
		if(pDevice)
			pDevice->Release();
		if (pContext)
		pContext->Release();
		if (pSwapChain)
		pSwapChain->Release();
		return PresentAddr;
	}

	inline UINT_PTR  GetEndScene()
	{
		LPDIRECT3D9 d3d = 0;
		LPDIRECT3DDEVICE9 d3ddev = 0;
		DWORD* pdivce = 0;
		DWORD addr = 0;
		D3DPRESENT_PARAMETERS d3dpp = { 0 };

		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = GetForegroundWindow();
		d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface
		if (d3d)
		{
			if (SUCCEEDED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev)))
			{
				pdivce = *(DWORD **)d3ddev;
				addr = pdivce[42];
				d3ddev->Release();
				d3d->Release();
				d3d = 0;
				d3ddev = 0;
				return addr;
			}
		}
		return 0;
	}

	inline UINT_PTR GetD3D9Present()
	{
		LPDIRECT3D9 d3d = 0;
		LPDIRECT3DDEVICE9 d3ddev = 0;
		DWORD* pdivce = 0;
		DWORD addr = 0;
		D3DPRESENT_PARAMETERS d3dpp = { 0 };

		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = GetForegroundWindow();
		d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface
		if (d3d)
		{
			if (SUCCEEDED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev)))
			{
				pdivce = *(DWORD * *)d3ddev;
				addr = pdivce[17];
				d3ddev->Release();
				d3d->Release();
				d3d = 0;
				d3ddev = 0;
				return addr;
			}
		}
		return addr;
	}

	inline UINT_PTR getBaseAddr(const wchar_t* moduleName)
	{
		MODULEENTRY32 pe32 = { 0 };
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId());
		if (hSnap == INVALID_HANDLE_VALUE)
			return 0;
		pe32.dwSize = sizeof(pe32);
		if (!Module32First(hSnap, &pe32))
			return 0;
		do
		{
			if (!wcscmp(moduleName, pe32.szModule)) 
			{
				CloseHandle(hSnap);
				return (UINT_PTR)pe32.modBaseAddr;
			}

		} while (Module32Next(hSnap, &pe32));
		CloseHandle(hSnap);
		return 0;
	}


	inline UINT_PTR GetMainThread()
	{
		DWORD currentPID = GetCurrentProcessId();
		THREADENTRY32 th32 = { 0 };
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, currentPID);
		if (hSnap == INVALID_HANDLE_VALUE)
			return 0;
		th32.dwSize = sizeof(th32);
		if (!Thread32First(hSnap, &th32))
			return 0;
		do
		{
			if (currentPID == th32.th32OwnerProcessID) 
			{
				CloseHandle(hSnap);
				return (UINT_PTR)th32.th32ThreadID;
			}

		} while (Thread32Next(hSnap, &th32));
		CloseHandle(hSnap);
		return 0;
	}
}