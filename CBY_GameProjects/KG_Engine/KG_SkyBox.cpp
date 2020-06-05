#include "KG_SkyBox.h"

namespace JH {
	bool	KG_SkyBox::CreateSkyBox(ID3D11Device* pDevice,
		ID3D11DeviceContext*pContext,
		const TCHAR* pLoadShaderFile,
		const TCHAR* pLoadTextureString)
	{
		Create(pDevice, pContext, pLoadShaderFile, pLoadTextureString, "VS", "PS_Cube");
		D3DXMatrixScaling(&m_matWorld, 100000.0f, 100000.0f, 100000.0f);
		return true;
	}
	bool	KG_SkyBox::Render()
	{



		D3DXMATRIX matSkyBoxView = m_matView;
		matSkyBoxView._41 = 0;
		matSkyBoxView._42 = 0;
		matSkyBoxView._43 = 0;

		CDX::ApplyDSS(m_obj.m_pContext, CDX::KG_DxState::g_pDSSDisable);

		ID3D11SamplerState* ppSamplerStates[2] = { CDX::KG_DxState::g_pSSClampPoint,CDX::KG_DxState::g_pSSClampLinear };
		m_obj.m_pContext->PSSetSamplers(0, 2, ppSamplerStates);

		SetMatrix(&m_matWorld, &matSkyBoxView, &m_matProj);

		PreRender();


		m_obj.m_pContext->PSSetShaderResources(3, 1, m_pCubeTexSRV.GetAddressOf());
		m_obj.m_pContext->DrawIndexed(36, 0, 0);

		CDX::ApplyDSS(m_obj.m_pContext, CDX::KG_DxState::g_pDSS);
		CDX::ApplyRS(m_obj.m_pContext, CDX::KG_DxState::g_pRSSold);
		CDX::ApplySS(m_obj.m_pContext, CDX::KG_DxState::g_pSampler);
		return true;
	}
	//HRESULT KG_SkyBox::LoadTextures(ID3D11Device* pDevice, const TCHAR* pLoadTextureString)
	//{
	//
	//}
	HRESULT  KG_SkyBox::CreateTexutureArray(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		const TCHAR** TextureArray)
	{
		HRESULT hr = S_OK;
		const TCHAR* g_szSkyTexture[] =
		{
			L"..\\..\\data\\sky\\st00_cm_front.bmp",
			L"..\\..\\data\\sky\\st00_cm_back.bmp",
			L"..\\..\\data\\sky\\st00_cm_right.bmp",
			L"..\\..\\data\\sky\\st00_cm_left.bmp",
			L"..\\..\\data\\sky\\st00_cm_up.bmp",
			L"..\\..\\data\\sky\\st00_cm_down.bmp"
		};

		int iNumTexture = sizeof(g_szSkyTexture) / sizeof(g_szSkyTexture[0]);

		for (int iTex = 0; iTex < iNumTexture; iTex++)
		{
			D3DX11CreateShaderResourceViewFromFile(m_obj.m_pd3dDevice,
				g_szSkyTexture[iTex],
				NULL, NULL,
				&m_pTexSRV[iTex],
				NULL);
		}
		return hr;
	}

	HRESULT  KG_SkyBox::CreateTexuture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const TCHAR* Texture)
	{

		HRESULT hr = S_OK;
		if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(m_obj.m_pd3dDevice,
			Texture,
			NULL, NULL,
			m_pCubeTexSRV.GetAddressOf(),
			NULL)));
		return hr;
	}
	bool KG_SkyBox::Frame()
	{
		float Angle = g_SecondTime * D3DX_PI / 100.0f;
		D3DXMATRIX matRot;
		D3DXMatrixRotationY(&matRot, Angle);

		m_matWorld *= matRot;

		return true;
	}
	KG_SkyBox::KG_SkyBox()
	{
	}


	KG_SkyBox::~KG_SkyBox()
	{
	}
}