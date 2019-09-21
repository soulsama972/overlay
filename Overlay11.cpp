#include "Overlay11.hpp"
#include "BlackList.hpp"

typedef HRESULT(__stdcall* _Present11)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
_Present11 Present11;

HRESULT __stdcall Present11CallBack(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	std::lock_guard<std::mutex> lck(bList.mu);
	bList.Init3D(pSwapChain);
	bList.DrawESPBone();
	return Present11(pSwapChain, SyncInterval, Flags);
}

void Overlay11::OverlayClean()
{
	std::lock_guard<std::mutex> lck(mu);

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
	line.CleanUp();
}

void Overlay11::OverlayInit()
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
	if (!firstTimeInitD3D)
		return;
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	pSwapChain->GetDesc(&sd);
	screenSize = fVec2(static_cast<float>(sd.BufferDesc.Width), static_cast<float>(sd.BufferDesc.Height));
	HRESULT res = pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& dev);
	if (SUCCEEDED(res))
	{

		
		IFW1Factory* pFW1Factory;
		FW1CreateFactory(FW1_VERSION, &pFW1Factory);

		pFW1Factory->CreateFontWrapper(dev, L"Arial", &pFontWrapper);
		pFW1Factory->Release();


		dev->GetImmediateContext(&devcon);


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
		InitShapes();

		firstTimeInitD3D = false;
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
	line.InitBuffer(dev, devcon, VertexLine, 2, ind, 2, 5000, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

}



void Overlay11::DrawString( float fontSize, fVec2 pos, DWORD color, wchar_t* fmt, ...)
{

	va_list args;
	va_start(args, fmt);
	wchar_t buffer[1024];
	ZeroMemory(buffer, 1024*2);
	vswprintf(buffer, 1024, fmt,args);
	pFontWrapper->DrawString(devcon, buffer, fontSize, pos.x,pos.y, color, FW1_TEXT_FLAG::FW1_RESTORESTATE);
}

void Overlay11::DrawString(fVec2 pos, wchar_t* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	wchar_t buffer[1024];
	ZeroMemory(buffer, 1024 * 2);
	vswprintf(buffer, 1024, fmt, args);
	pFontWrapper->DrawString(devcon, buffer, 15, pos.x, pos.y, 0xffffff00, FW1_TEXT_FLAG::FW1_RESTORESTATE);
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

void Overlay11::InsertLine(fVec2 p1,fVec2 p2, fVec4 color)
{
	VertexInstance in;
	in.color = color;
	in.position = p1;
	in.size = p2 - p1;
	line.AddInstance(in);
}

void Overlay11::InsertRect(fVec2 pos, fVec2 size, fVec4 color)
{
	VertexInstance in;
	in.color = color;
	in.size = size;
	in.position = pos;
	rect.AddInstance(in);
}

void Overlay11::DrawShapes(bool cleanAfterDraw)
{

	rect.Draw();
	line.Draw();

	if (cleanAfterDraw)
	{
		rect.ClearInstance();
		line.ClearInstance();		
	}
}
