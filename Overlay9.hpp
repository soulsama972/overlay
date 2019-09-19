#pragma once
#pragma warning (push)
#pragma warning (disable : 26495)
#pragma warning (disable : 4005)
#include<d3d9.h>
#include<d3dx9.h>
#include<windows.h>

#pragma warning(pop)

#include"model9.hpp"
#include"Utill.hpp"
#include<iostream>
#include<string>
#include<vector>
#include<mutex>
#include"Hook.h"
#include<commctrl.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib,"Comctl32.lib")



class Overlay9
{
public:
	std::mutex mu;
	bool overlay = false;
	bool firstTimeInitD3D = true;
	WNDPROC orignal = NULL;
	void Init();
	void Clean();
	void Update(LPDIRECT3DDEVICE9 d3ddev);
	void InitD3D(LPDIRECT3DDEVICE9 d3ddev);
	void CreateRect9(LPDIRECT3DDEVICE9 d3ddev, fVec2 pos, fVec2 size, DWORD color);
	void CreateTex9(LPDIRECT3DDEVICE9 d3ddev, fVec2 pos, fVec2 size, int id);
	void Draw(LPDIRECT3DDEVICE9 d3ddev);
private:
	hook_t h;
	std::vector<model9> models;
	HWND hwnd = NULL;

	D3DVIEWPORT9 view = { 0 };
	LPD3DXFONT font = NULL;
	struct rect9
	{
		DWORD left, top, right, bottom;
	};
};

extern Overlay9 overlay9;


HRESULT __stdcall D3D9PresentCallBack(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);