#include "model9.hpp"


void model9::Draw(LPDIRECT3DDEVICE9 d3ddev)
{
	d3ddev->SetFVF(fvf);
	d3ddev->SetStreamSource(0, vBuffer, 0, vertexSize);

	d3ddev->SetIndices(iBuffer);

	d3ddev->SetPixelShader(0);
	d3ddev->SetTexture(0, NULL);

	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	d3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (isTex)
	{
		d3ddev->SetTexture(0, tex);


		d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		d3ddev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		d3ddev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		d3ddev->SetTextureStageState(0, D3DTSS_CONSTANT, 0xdcffffff);
		d3ddev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CONSTANT);

		//d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		//d3ddev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		//d3ddev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_BLENDDIFFUSEALPHA);
	}
	d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, indexCount / 3);
}
