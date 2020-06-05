#include "KG_DxRT.h"

HRESULT KG_DxRT::Create(ID3D11Device* pd3dDevice, float fWidth, float fHeight)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC td;
	td.Width = (UINT)fWidth;
	td.Height = (UINT)fHeight;
	td.MipLevels = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;											//¹Ýµå½Ã µðÆúÆ®¿©¾ß ·£´õÅ¸ÄÏºä¿ëÀÓ
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	//
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	td.ArraySize = 1;
	if (FAILED(hr = pd3dDevice->CreateTexture2D(&td, NULL, m_pTexture.GetAddressOf())))
	{
		return hr;
	}

	if (FAILED(hr = pd3dDevice->CreateRenderTargetView(
		m_pTexture.Get(), NULL,
		m_pRenderTargetView.GetAddressOf())))
	{
		return hr;
	}
	if (FAILED(hr = pd3dDevice->CreateShaderResourceView(
		m_pTexture.Get(), NULL,
		m_pSRV.GetAddressOf())))
	{
		return hr;
	}

	m_vp.Width = fWidth;
	m_vp.Height = fHeight;
	m_vp.TopLeftX = 0;
	m_vp.TopLeftY = 0;
	m_vp.MinDepth = 0;
	m_vp.MaxDepth = 1.0f;

	UpdateDepthStencilView(pd3dDevice, fWidth, fHeight);
	return hr;
}

HRESULT KG_DxRT::UpdateDepthStencilView(ID3D11Device* pDevice, UINT Width, UINT Height)
{
	HRESULT hr;
	if (m_pDepthStencilView != nullptr)
	{
		m_pDepthStencilView.ReleaseAndGetAddressOf();
	}

	m_vp.Width = Width;
	m_vp.Height = Height;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
	D3D11_TEXTURE2D_DESC DescDepth;
	DescDepth.Width = Width;
	DescDepth.Height = Height;
	DescDepth.MipLevels = 1;
	DescDepth.ArraySize = 1;
	DescDepth.Format = m_DSFormat;
	DescDepth.SampleDesc.Count = 1;
	DescDepth.SampleDesc.Quality = 0;
	DescDepth.Usage = D3D11_USAGE_DEFAULT;

	// ¹é ¹öÆÛ ±íÀÌ ¹× ½ºÅÙ½Ç ¹öÆÛ »ý¼º
	if (DescDepth.Format == DXGI_FORMAT_D24_UNORM_S8_UINT)
		DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	else // ±íÀÌ¸Ê Àü¿ë ±íÀÌ¸Ê »ý¼º
		DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	DescDepth.CPUAccessFlags = 0;
	DescDepth.MiscFlags = 0;
	if (FAILED(hr = pDevice->CreateTexture2D(&DescDepth, NULL, &pDSTexture)))
	{
		return hr;
	}

	///// ½¦ÀÌ´õ ¸®¼Ò½º »ý¼º : ±íÀÌ ¸Ê ½¦µµ¿ì¿¡¼­ »ç¿ëÇÑ´Ù. ///
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	switch (DescDepth.Format)
	{
	case DXGI_FORMAT_R32_TYPELESS:
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	}
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	if (FAILED(hr = pDevice->CreateDepthStencilView(pDSTexture.Get(), &dsvDesc, &m_pDepthStencilView)))
	{
		return hr;
	}

	// ±íÀÌ¸Ê ÀÏ °æ¿ì¸¸ »ý¼ºÇÑ´Ù.
	if (DescDepth.Format == DXGI_FORMAT_R32_TYPELESS)
	{
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		pDevice->CreateShaderResourceView(pDSTexture.Get(), &srvDesc, &m_pDsvSRV);
	}
	m_pDepthStencilView->GetDesc(&m_DepthStencilDesc);
	return hr;
}

bool KG_DxRT::Begin(ID3D11DeviceContext* pContext,
	D3DXVECTOR4 vColor)
{
	m_iViewPorts = 1;
	pContext->RSGetViewports(&m_iViewPorts, m_vpOld);
	pContext->OMGetRenderTargets(1, &m_pOldRTV, &m_pOldDSV);

	ID3D11RenderTargetView* pNullRTV = NULL;
	pContext->OMSetRenderTargets(1, &pNullRTV, NULL);
	pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(),
		m_pDepthStencilView.Get());

	pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), vColor);
	pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);
	pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_STENCIL, 1.0, 0);

	pContext->RSSetViewports(1, &m_vp);

	return true;
}
bool KG_DxRT::End(ID3D11DeviceContext* pContext)
{
	pContext->RSSetViewports(m_iViewPorts, m_vpOld);
	pContext->OMSetRenderTargets(1, &m_pOldRTV, m_pOldDSV);
	SAFE_RELEASE(m_pOldRTV);
	SAFE_RELEASE(m_pOldDSV);
	//SaveFile(pContext, L"save.png");
	return true;
}

bool KG_DxRT::Release()
{
	/*if (m_pRenderTargetView)
	{
		m_pRenderTargetView->Release();
	}
	if (m_pDepthStencilView)
	{
		m_pDepthStencilView->Release();
	}*/
	/*if (m_pTexture)
	{
		m_pTexture->Release();
	}*/
	/*if (m_pSRV)
	{
		m_pSRV->Release();
	}*/

	//SAFE_RELEASE(m_pOldRTV);
	//SAFE_RELEASE(m_pOldDSV);
	return true;
}

HRESULT KG_DxRT::SaveFile(ID3D11DeviceContext* pContext, T_STR name)
{
	HRESULT hr;
	if (m_pTexture != nullptr)
	{
		hr = D3DX11SaveTextureToFile(pContext,
			m_pTexture.Get(),
			D3DX11_IFF_PNG, name.c_str());
	}
	return hr;
}

KG_DxRT::KG_DxRT()
{
}


KG_DxRT::~KG_DxRT()
{
}

