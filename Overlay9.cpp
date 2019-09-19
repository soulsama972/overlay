#include "Overlay9.hpp"

Overlay9 overlay9;

typedef HRESULT(WINAPI* _D3D9Present)(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion);
_D3D9Present D3D9Present;

void Overlay9::CreateRect9(LPDIRECT3DDEVICE9 d3ddev, fVec2 pos, fVec2 size, DWORD color)
{
	model9 model;
	defaultVertex vertex[] =
	{
		{pos.x,pos.y + size.y		  ,1.0f,1.0f,color},
		{pos.x,pos.y				  ,1.0f,1.0f,color},
		{pos.x + size.x,pos.y		  ,1.0f,1.0f,color},
		{pos.x + size.x,pos.y + size.y,1.0f,1.0f,color},
	};
	unsigned int index[] =
	{
		0,1,2,
		0,2,3
	};
	model.InitBuffer<defaultVertex>(d3ddev, vertex, 4, index, 6, FVF);
	models.push_back(model);
}

void Overlay9::CreateTex9(LPDIRECT3DDEVICE9 d3ddev, fVec2 pos, fVec2 size, int id)
{
	model9 model;
	texVertex vertex[] =
	{
		{pos.x,pos.y + size.y		  ,1.0f,1.0f,0,1},
		{pos.x,pos.y				  ,1.0f,1.0f,0,0},
		{pos.x + size.x,pos.y		  ,1.0f,1.0f,1,0},
		{pos.x + size.x,pos.y + size.y,1.0f,1.0f,1,1},
	};
	unsigned int index[] =
	{
		0,1,2,
		0,2,3
	};
	model.InitBuffer<texVertex>(d3ddev, vertex, 4, index, 6, FVFTEX, true, id);
	models.push_back(model);
}

void Overlay9::Draw(LPDIRECT3DDEVICE9 d3ddev)
{
	mu.lock();
	Update(d3ddev);
	for (auto r : models)
		r.Draw(d3ddev);
	mu.unlock();
}


void Overlay9::Init()
{
	Hook::apiHook(h, Utill::GetD3D9Present(), D3D9PresentCallBack, 5);
	D3D9Present = (_D3D9Present)h.jumpBack;
	Hook::insertHook(h);
}

void Overlay9::InitD3D(LPDIRECT3DDEVICE9 d3ddev)
{
	Update(d3ddev);
	//CreateTex9(d3ddev, Vec2(view.Width - view.Width / 2.0f, 0), Vec2(view.Width / 2.0f, view.Height / 3.0f), IDB_PNG2);
	D3DXCreateFontA(d3ddev, 40, 20, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Calibri", &font);
	D3DDEVICE_CREATION_PARAMETERS param;
	d3ddev->GetCreationParameters(&param);
	hwnd = param.hFocusWindow;
	orignal = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC,(LONG_PTR)WndProc);

}

void Overlay9::Clean()
{
	mu.lock();
	Hook::unHooked(h);
	Hook::freeHook(h);
	if (orignal)
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)orignal);
	mu.unlock();
}

void Overlay9::Update(LPDIRECT3DDEVICE9 d3ddev)
{
	d3ddev->GetViewport(&view);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_INSERT)
			overlay9.overlay = !overlay9.overlay;
		break;
	}
	default:
		break;
	}
	return CallWindowProc(overlay9.orignal, hWnd, uMsg, wParam, lParam);
}


HRESULT __stdcall D3D9PresentCallBack(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion)
{
	if (overlay9.firstTimeInitD3D)
	{
		overlay9.InitD3D(pDevice);
		overlay9.firstTimeInitD3D = false;
		overlay9.CreateRect9(pDevice, fVec2(0, 0), fVec2(200, 200), 0xffff00ff);
	}
	if (overlay9.overlay)
	{
		pDevice->BeginScene();
		overlay9.Draw(pDevice);
		pDevice->EndScene();
	}
	return D3D9Present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
