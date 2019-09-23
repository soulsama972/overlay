#pragma once
#include"model11.hpp"
#include"Hook.h"
#include"Utill.hpp"
#include<mutex>
#pragma warning (push)
#pragma warning (disable : 26495)
#pragma warning (disable : 4005)
#include<FW1FontWrapper.h>
#pragma warning(pop)

typedef void(__thiscall* ShellClass)(void* Class, IDXGISwapChain* pSwapChain);
typedef HRESULT(__stdcall* _Present11)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

#pragma comment (lib,"FW1FontWrapper.lib")
class Overlay11
{
public:

	void Init3D(IDXGISwapChain* pSwapChain);
	static ShellClass shellClass;
	static void* pThis;
protected:
	void OverlayInit();
	void OverlayClean();
	void InitShapes();
	void UpdateScreen();
	void InsertLine(fVec2 p1, fVec2 p2,fVec4 color);
	void InsertRect(fVec2 pos, fVec2 size, fVec4 color);
	void DrawShapes(bool cleanAfterDraw = true);
	void DrawString(float fontSize,fVec2 pos,DWORD color,wchar_t *arg,...);
	void DrawString(fVec2 pos, wchar_t* arg, ...);

public: 
	fVec2 screenSize;
private:
	ID3D11Buffer* screenBuffer = 0;
	IFW1FontWrapper* pFontWrapper = 0;
	ID3D11Device* dev = 0;
	ID3D11DeviceContext* devcon = 0;
	Model11<VertexInstance>rect;
	Model11<VertexInstance>line;
	bool firstTimeInitD3D = true;
	hook_t h;
};

