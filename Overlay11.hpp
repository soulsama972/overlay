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

#pragma comment (lib,"FW1FontWrapper.lib")
class Overlay11
{
public:
	bool overlay = false;
	bool firstTimeInitD3D = true;
	void Init();
	void Init3D(IDXGISwapChain* pSwapChain);
	void Clean();
	void InitShapes();
	void UpdateScreen();
	void InsertLine(float x,float y,float x2,float y2,fVec4 color);
	void InsertRect(fVec2 pos, fVec2 size, fVec4 color);
	void DrawShapes();
	bool WorldToScreen(fVec3 enemyPos, fVec2 & newEnemyPos, bool Transpose);
	void DrawString(float fontSize,fVec2 pos,DWORD color,wchar_t *arg,...);
	void DrawString(fVec2 pos, wchar_t* arg, ...);
	float* viewMatrix = nullptr;
	ID3D11Device* dev = 0;                     
	ID3D11DeviceContext* devcon = 0;           
	IFW1FontWrapper* pFontWrapper = 0;
	fVec2 screenSize;
	HWND hwnd = NULL;
	std::mutex mu;
	hook_t h;
	Model11<VertexInstance>rect;
	Model11<VertexInstance>Line;
private:
	ID3D11Buffer* screenBuffer = 0;
};

extern Overlay11 Overlay;