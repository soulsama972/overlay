#pragma once
#include<windows.h>
#include<iostream>
#include<string>
#include<fstream>

#pragma warning (push)
#pragma warning (disable : 26495)
#pragma warning (disable : 4005)
#include<d3d11.h>
#include<D3DX11.h>
#pragma warning(pop)

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#include"Math.hpp"



extern std::string stringPixelShader;


extern std::string stringVertexShader;


struct VertexType
{
	fVec3 position;
};

struct ScreenShader
{
	fVec2 screenPos;
};

struct VertexInstance
{
	fVec2 position;
	fVec2 size;
	fVec4 color;
};

void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd);


template<typename T>
class Model11
{
public:
	~Model11();

	void InitBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		VertexType* vertex, UINT vertexLen, unsigned int* index, UINT indexLen, UINT instanceCount, D3D11_PRIMITIVE_TOPOLOGY topology);

	void CleanUp();

	void AddInstance(T in);
	void ChangeInstance(UINT id, T in);
	void UpdateModel();
	void ClearInstance();
	void Draw();

	T* vInstance = 0;
private:

	void InitializeShaders();


	ID3D11Buffer* vertexBuffer = 0;
	ID3D11Buffer* indexBuffer = 0;
	ID3D11Buffer* instanceBuffer = 0;
	
	ID3D11VertexShader* vertexShader = 0;
	ID3D11PixelShader* pixelShader = 0;
	ID3D11InputLayout* layout = 0;
	ID3D11Device* device = 0;
	ID3D11DeviceContext* deviceContext = 0;

	UINT  vertexCount = 0, indexCount = 0;
	UINT vInstanceCount = 0;
	UINT vInstanceMax = 0;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
};

template<typename T>
Model11<T>::~Model11()
{
	CleanUp();
}

template<typename T>
void Model11<T>::InitBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, VertexType* vertex, UINT vertexLen, unsigned int* index, UINT indexLen, UINT instanceCount, D3D11_PRIMITIVE_TOPOLOGY Topology)
{
	this->device = device;
	this->deviceContext = deviceContext;

	InitializeShaders();

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	topology = Topology;
	vertexCount = vertexLen;
	indexCount = indexLen;
	vInstanceMax = instanceCount;
	vInstance = new T[instanceCount];

	if (!vInstance)
	{
		MessageBoxA(NULL, "failed to alloc", "", MB_OK);
		exit(-1);
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertex;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		MessageBoxA(NULL, "error create buffer line 79 model11.hpp", "", MB_OK);
		exit(-1);
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = index;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		MessageBoxA(NULL, "error create buffer line 100 model11.hpp", "", MB_OK);
		return;
	}

	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = sizeof(VertexInstance) * instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Create the instance buffer.
	result = device->CreateBuffer(&instanceBufferDesc, NULL, &instanceBuffer);
	if (FAILED(result))
	{
		MessageBoxA(NULL, "error create buffer line 101 model11.hpp", "", MB_OK);
		return;
	}
}

template<typename T>
void Model11<T>::InitializeShaders()
{
	HRESULT result = 0;
	ID3D10Blob* vertexShaderBuffer = 0;
	ID3D10Blob* pixelShaderBuffer = 0;
	ID3D10Blob* errorMessage;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	unsigned int numElements = 0;
	

	result = result = D3DX11CompileFromMemory(stringVertexShader.c_str(), stringVertexShader.length(),
		NULL, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, NULL, 0, &vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, 0);
		else
			MessageBox(0, L"", L"Missing Shader vertex", MB_OK);
		exit(-1);
	}

	result = D3DX11CompileFromMemory(stringPixelShader.c_str(), stringPixelShader.length(), NULL, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		NULL, NULL, &pixelShaderBuffer, NULL, NULL);
	if (FAILED(result))
	{

		MessageBox(0, L"", L"error Shader pixel ", MB_OK);
		exit(-1);
	}
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	if (FAILED(result))
	{
		MessageBox(0, L"", L"create Shader vertex ", MB_OK);
		exit(-1);
	}
	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
	if (FAILED(result))
	{
		vertexShader->Release();
		vertexShader = 0;
		MessageBox(0, L"", L"create Shader pixel ", MB_OK);
		exit(-1);
	}


	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 1;
	polygonLayout[1].AlignedByteOffset = 0;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[1].InstanceDataStepRate = 1;

	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 1;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 1;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[2].InstanceDataStepRate = 1;

	polygonLayout[3].SemanticName = "COLOR";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[3].InputSlot = 1;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[3].InstanceDataStepRate = 1;


	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &layout);
	if (FAILED(result))
	{
		vertexShaderBuffer->Release();
		vertexShaderBuffer = 0;

		pixelShaderBuffer->Release();
		pixelShaderBuffer = 0;
		MessageBox(0, L"", L"create Inputlayout  ", MB_OK);
		exit(-1);
	}
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

}

template<typename T>
void Model11<T>::Model11::CleanUp()
{
	// Release the index buffer.
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}
	if (instanceBuffer)
	{
		instanceBuffer->Release();
		instanceBuffer = 0;
	}
	if (vertexShader)
	{
		vertexShader->Release();
		vertexShader = 0;
	}
	if (pixelShader)
	{
		pixelShader->Release();
		pixelShader = 0;
	}
	if (layout)
	{
		layout->Release();
		layout = 0;
	}
	if (vInstance)
	{
		delete vInstance;
		vInstance = 0;
	}

}


template<typename T>
void Model11<T>::AddInstance(T in)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexInstance* dataPtr;
	unsigned int bufferNumber = 0;
	if (vInstanceCount < vInstanceMax)
	{
		vInstance[vInstanceCount++] = in;

		deviceContext->Map(instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		dataPtr = (VertexInstance*)mappedResource.pData;

		memcpy(dataPtr, vInstance, sizeof(VertexInstance) * vInstanceCount);

		deviceContext->Unmap(instanceBuffer, 0);
	}
}

template<typename T>
void Model11<T>::ChangeInstance(UINT id, T in)
{
	vInstance[id] = in;
}

template<typename T>
void Model11<T>::Model11::UpdateModel()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexInstance* dataPtr;
	unsigned int bufferNumber = 0;
	deviceContext->Map(instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (VertexInstance*)mappedResource.pData;

	memcpy(dataPtr, vInstance, sizeof(VertexInstance) * vInstanceCount);

	deviceContext->Unmap(instanceBuffer, 0);
}

template<typename T>
void Model11<T>::ClearInstance()
{
	vInstanceCount = 0;
}


template<typename T>
void Model11<T>::Model11::Draw()
{
	if (vInstanceCount == 0)
		return;

	unsigned int stride[] = { sizeof(VertexType),sizeof(VertexInstance) };
	unsigned int offset[] = { 0,0 };
	ID3D11Buffer* bufferPointers[] = { vertexBuffer ,instanceBuffer };


	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, stride, offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(topology);

	// Set the vertex input layout.
	deviceContext->IASetInputLayout(layout);


	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(vertexShader, NULL, 0);
	deviceContext->PSSetShader(pixelShader, NULL, 0);

	// Render the triangle.
	deviceContext->DrawIndexedInstanced(indexCount, vInstanceCount, 0, 0, 0);
}



