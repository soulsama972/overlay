#include "Overlay11.hpp"

ShellClass Overlay11::shellClass = 0;
void * Overlay11::pThis = 0;
_Present11 Present11;

HRESULT __stdcall Present11CallBack(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (Overlay11::shellClass && Overlay11::pThis)
		Overlay11::shellClass(Overlay11::pThis, pSwapChain);
	return Present11(pSwapChain, SyncInterval, Flags);
}

void Overlay11::OverlayCleanUp()
{

	if(orignalWinProc)
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG)orignalWinProc);

	Hook::unHooked(h);
	Sleep(1000);
	Hook::freeHook(h);
	if (dev)
		dev->Release();

	if (devcon)
		devcon->Release();

	if (screenBuffer)
		screenBuffer->Release();

	if (pFontWrapper)
		pFontWrapper->Release();

	rect.CleanUp();
	line.CleanUp();
	if (bCreateConosle)
	{
		fclose(file);
		FreeConsole();
	}
}

void Overlay11::OverlayInit(void* pThis, void* shellClass,bool createConsole)
{
	Overlay11::pThis = pThis;
	Overlay11::shellClass = (ShellClass)shellClass;
	bCreateConosle = createConsole;
	UINT_PTR p;
	do
	{
		 p = Utill::GetD3D11Present();
	} while (p == 0);
	Hook::apiHook(h, p, Present11CallBack, 5);
	Present11 = (_Present11)h.jumpBack;
	Hook::insertHook(h);
\
	if (bCreateConosle)
	{
		AllocConsole();
		freopen_s(&file, "CONOUT$", "w", stdout);
	}
		
}

void Overlay11::Init3D(IDXGISwapChain* pSwapChain)
{
	if (!firstTimeInitD3D)
		return;
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	pSwapChain->GetDesc(&sd);

	screenSize = fVec2(static_cast<float>(sd.BufferDesc.Width), static_cast<float>(sd.BufferDesc.Height));
	hwnd = sd.OutputWindow;
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
		UpdateScreen();
		firstTimeInitD3D = false;
	}
}

void Overlay11::InitShapes()
{
	//rect
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
	//line
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
	//circle
	const int numPoint = 36;
	const float PI = 3.14159f;
	float WedgeAngle = (float)( (2 * PI) / numPoint);
	VertexType VertexCircle[numPoint];
	unsigned int indircle[numPoint+1];
	for (int i = 0; i < numPoint; i++)
	{
		//Calculate theta for this vertex
		float Theta = i * WedgeAngle;
		//Compute X and Y locations
		VertexCircle[i].position = fVec3(cosf(Theta),sinf(Theta),1.0f);
		indircle[i] = i;
	}
	indircle[numPoint] = 0;
	/*for (int i = 0; i < numPoint; i++)
	{
		indircle[i] = i;

	}*/
	circle.InitBuffer(dev, devcon, VertexCircle, numPoint, indircle, numPoint+1, 5000, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
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

void Overlay11::InsertCircle(fVec2 pos, float rad, fVec4 color)
{
	VertexInstance in;
	in.color = color;
	in.size = fVec2(rad,rad);
	in.position = pos;
	circle.AddInstance(in);
		

}//Draw2DCircle

void Overlay11::Draw(bool cleanAfterDraw)
{
	UpdateScreen();
	rect.Draw();
	line.Draw();
	circle.Draw();
	if (cleanAfterDraw)
	{
		rect.ClearInstance();
		line.ClearInstance();	
		circle.ClearInstance();
	}
}
