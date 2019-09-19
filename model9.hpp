#pragma once
#pragma warning (push)
#pragma warning (disable : 26495)
#pragma warning (disable : 4005)
#include<d3d9.h>
#include<d3dx9.h>
#include<windows.h>
#include"Math.hpp"
#include<iostream>
#include<vector>
#pragma warning(pop)

#define FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

#define FVFTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

struct defaultVertex
{
	FLOAT x, y, z, rhw;    // from the D3DFVF_XYZRHW flag
	DWORD color;    // from the D3DFVF_DIFFUSE flag
};

struct texVertex
{
	FLOAT x, y, z, rhw;    // from the D3DFVF_XYZRHW flag
	float u, v;    // from the D3DFVF_DIFFUSE flag
};

class model9
{
public:
	model9() = default;
	template<typename S>
	void InitBuffer(LPDIRECT3DDEVICE9 d3ddev, S* vertex, UINT vertexLen, unsigned int* index, UINT indexLen, UINT fvf, bool isTex = false, int idName = 0);
	void Draw(LPDIRECT3DDEVICE9 d3ddev);
private:
	LPDIRECT3DVERTEXBUFFER9 vBuffer = 0;
	LPDIRECT3DINDEXBUFFER9 iBuffer = 0;
	UINT vertexCount = 0;
	UINT indexCount = 0;
	UINT fvf = 0;
	UINT vertexSize = 0;
	LPDIRECT3DTEXTURE9 tex = 0;
	bool isTex = false;
};
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw;    // from the D3DFVF_XYZRHW flag
	DWORD color;    // from the D3DFVF_DIFFUSE flag
};


template<typename S>
inline void model9::InitBuffer(LPDIRECT3DDEVICE9 d3ddev, S* vertex, UINT vertexLen, unsigned int* index, UINT indexLen, UINT Fvf, bool istex, int idName)
{
	vertexCount = vertexLen;
	indexCount = indexLen;
	fvf = Fvf;
	isTex = istex;
	vertexSize = sizeof(vertex[0]);


	d3ddev->CreateVertexBuffer(vertexCount * sizeof(vertex[0]), D3DUSAGE_DYNAMIC, FVF, D3DPOOL_DEFAULT, &vBuffer, NULL);
	if (!vBuffer)
	{
		std::cout << "error vbuffer " << std::endl;
		return;
	}
	VOID* pVoid;    // the void pointer

	vBuffer->Lock(0, 0, (void**)& pVoid, D3DLOCK_DISCARD);    // lock the vertex buffer
	memcpy(pVoid, vertex, vertexCount * vertexSize);    // copy the vertices to the locked buffer
	vBuffer->Unlock();    // unlock the vertex buffer


	d3ddev->CreateIndexBuffer(indexCount * sizeof(index[0]), D3DUSAGE_DYNAMIC, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &iBuffer, NULL);
	if (!iBuffer)
	{
		std::cout << "error ibuffer " << std::endl;
		return;
	}


	iBuffer->Lock(0, 0, (void**)& pVoid, D3DLOCK_DISCARD);    // lock the index buffer
	memcpy(pVoid, index, indexCount * sizeof(unsigned int));    // copy the vertices to the locked buffer
	iBuffer->Unlock();    // unlock the index buffer

	if (isTex)
	{
		//D3DXCreateTextureFromFileInMemory()
		if (FAILED(D3DXCreateTextureFromResource(d3ddev, GetModuleHandleA("dllOverlay.dll"), MAKEINTRESOURCE(idName), &tex)))
			std::cout << "error" << std::endl;
	}

}


