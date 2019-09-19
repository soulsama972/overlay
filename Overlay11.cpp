#include "Overlay11.hpp"
#include "BlackList.hpp"

Overlay11 Overlay;

typedef HRESULT(__stdcall* _Present11)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
_Present11 Present11;

HRESULT __stdcall Present11CallBack(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	std::lock_guard<std::mutex> lck(Overlay.mu);
	if (Overlay.firstTimeInitD3D)
		Overlay.Init3D(pSwapChain);
	if (Overlay.viewMatrix != nullptr)
	{
		Overlay.UpdateScreen();
		bList.DrawESPBone();
		bList.AimBot();
	}
	return Present11(pSwapChain, SyncInterval, Flags);
}

void Overlay11::Clean()
{
	std::lock_guard<std::mutex> lck(Overlay.mu);

	Hook::unHooked(h);
	Hook::freeHook(h);
	Sleep(100);
	if (dev)
	{
		dev->Release();
		dev = 0;
	}
	if (devcon)
	{
		devcon->Release();
		devcon = 0;
	}
	if (screenBuffer)
	{
		screenBuffer->Release();
		screenBuffer = 0;
	}
	if (pFontWrapper)
	{
		pFontWrapper->Release();
		pFontWrapper = 0;
	}
	rect.CleanUp();
	Line.CleanUp();
}

void Overlay11::Init()
{
	UINT_PTR p;
	do
	{
		 p = Utill::GetD3D11Present();
	} while (p == 0);
	Hook::apiHook(h, p, Present11CallBack, 5);
	Present11 = (_Present11)h.jumpBack;
	Hook::insertHook(h);

}

void Overlay11::Init3D(IDXGISwapChain* pSwapChain)
{
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	pSwapChain->GetDesc(&sd);
	hwnd = sd.OutputWindow;
	screenSize = fVec2(static_cast<float>(sd.BufferDesc.Width), static_cast<float>(sd.BufferDesc.Height));
	HRESULT res = pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& dev);
	if (SUCCEEDED(res))
	{

		
		IFW1Factory* pFW1Factory;
		FW1CreateFactory(FW1_VERSION, &pFW1Factory);

		pFW1Factory->CreateFontWrapper(dev, L"Arial", &pFontWrapper);
		pFW1Factory->Release();


		dev->GetImmediateContext(&Overlay.devcon);
		InitShapes();
		D3D11_BUFFER_DESC matrixBufferDesc = { 0 };
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(float) * 16;
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		HRESULT result = dev->CreateBuffer(&matrixBufferDesc, NULL, &screenBuffer);
		if (FAILED(result))
		{
			MessageBox(0, L"", L"CreateBuffer  ", MB_OK);
			exit(-1);
		}
		Overlay.firstTimeInitD3D = false;
	}
}

void Overlay11::InitShapes()
{
	VertexType vertex[] =
	{
		{{-1.0f,-1.0f,1.0f}},
		{{-1.0f,1.0f,1.0f}},
		{{1.0f,1.0f,1.0f}},
		{{1.0f,-1.0f,1.0f}},
	};

	unsigned int ind[] =
	{
		0,1,2,
		0,2,3
	};
	rect.InitBuffer(dev,devcon, vertex, 4, ind, 6, 5000, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	VertexType VertexLine[] =
	{
		{fVec3(-1.0f,1.0f,1.0f)},
		{fVec3(1.0f,-1.0f,1.0f)},
	};
	unsigned int indLine[] =
	{
		0,1
	};
	Line.InitBuffer(dev, devcon, VertexLine, 2, ind, 2, 5000, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

}

bool Overlay11::WorldToScreen(fVec3 enemyPos, fVec2 & newEnemyPos, bool Transpose)
{

	auto TransfromCoord = [](float x, float f)
	{
		return f * (x + 1) * 0.5f;
	};
	fVec4 vec4;
	Matrix4x4 view = viewMatrix;

	if (Transpose)
		view.Transpose(view);

	vec4.Transform(enemyPos, view);

	if (vec4.w < 0.5f)
		return false;

	vec4.x /= vec4.w;
	vec4.y /= vec4.w;

	newEnemyPos.x = TransfromCoord(vec4.x, screenSize.x);
	newEnemyPos.y = TransfromCoord(-vec4.y, screenSize.y);

	return true;
}

void Overlay11::UpdateScreen()
{

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ScreenShader* dataPtr;
	unsigned int bufferNumber = 0;
	D3D11_VIEWPORT view;
	UINT num = 1;

	HRESULT result = devcon->Map(screenBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	devcon->RSGetViewports(&num, &view);
	dataPtr = (ScreenShader*)mappedResource.pData;
	if (view.Width == 0)
		dataPtr->screenPos = screenSize;
	else
	{
		dataPtr->screenPos = fVec2(view.Width, view.Height);
		screenSize = fVec2(view.Width, view.Height);
	}


	devcon->Unmap(screenBuffer, 0);

	devcon->VSSetConstantBuffers(bufferNumber, 1, &screenBuffer);
}

void Overlay11::InsertLine(float x, float y, float x2, float y2, fVec4 color)
{
	VertexInstance in;
	in.color = color;
	in.position = fVec2(x, y);
	in.size = fVec2(x2 - x, y2 - y);
	Line.AddInstance(in);
}

void Overlay11::InsertRect(fVec2 pos, fVec2 size, fVec4 color)
{
	VertexInstance in;
	in.color = color;
	in.size = size;
	in.position = pos;
	rect.AddInstance(in);
}

void Overlay11::DrawShapes()
{
	rect.Draw();
	Line.Draw();
}
