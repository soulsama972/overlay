#pragma once
#include"model11.hpp"
#include"Hook.h"
#include"Utill.hpp"
#pragma warning (push)
#pragma warning (disable : 26495)
#pragma warning (disable : 4005)
#include<FW1FontWrapper.h>
#pragma warning(pop)

#pragma comment (lib,"FW1FontWrapper.lib")

typedef void(__thiscall* ShellClass)(void* Class, IDXGISwapChain* pSwapChain);
typedef HRESULT(__stdcall* _Present11)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);



class Overlay11
{
public:

	void Init3D(IDXGISwapChain* pSwapChain);
	static ShellClass shellClass;
	static void* pThis;
protected:
	void OverlayInit(void * pThis,ShellClass shellClass, bool createConsole = true);
	void OverlayCleanUp();

	void InsertLine(fVec2 p1, fVec2 p2,fVec4 color);
	void InsertRect(fVec2 pos, fVec2 size, fVec4 color);

	void Draw(bool cleanAfterDraw = true);

	void DrawString(float fontSize,fVec2 pos,DWORD color,wchar_t *arg,...);
	void DrawString(fVec2 pos, wchar_t* arg, ...);

	fVec2 screenSize;
private:
	void UpdateScreen();
	void InitShapes();

	ID3D11Buffer* screenBuffer = 0;
	IFW1FontWrapper* pFontWrapper = 0;
	ID3D11Device* dev = 0;
	ID3D11DeviceContext* devcon = 0;
	Model11<VertexInstance>rect;
	Model11<VertexInstance>line;
	bool firstTimeInitD3D = true;
	bool bCreateConosle = true;
	hook_t h;
	FILE* file;
};

