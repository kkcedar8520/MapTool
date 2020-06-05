#include "KG_Shadow.h"



HRESULT KG_Shadow::Create(ID3D11Device* pd3dDevice, float fWidth, float fHeight, const TCHAR* ShaderFileName, const CHAR* VSName, const CHAR* PSName)
{
	HRESULT hr = S_OK;

	if (m_Rt)
	{
		m_Rt->Release();
		m_Rt = NULL;
	}
	m_Rt = std::make_shared<KG_DxRT>();
	m_Rt->m_DSFormat = m_DSFormat;
	hr = m_Rt->Create(pd3dDevice, fWidth, fHeight);

	hr = LoadShader(pd3dDevice, ShaderFileName, VSName, PSName);

	return hr;
}
bool KG_Shadow::Begin(ID3D11DeviceContext* pContext, D3DXVECTOR4 vColor)
{
	CDX::ApplyDSS(pContext, CDX::KG_DxState::g_pDSS);
	CDX::ApplyBS(pContext, CDX::KG_DxState::g_pAlpahBlend);
	CDX::ApplyRS(pContext, CDX::KG_DxState::g_pRSDepth);

	m_Rt->Begin(pContext, vColor);
	return true;
}

void KG_Shadow::ShaderSet(ID3D11DeviceContext* pContext)
{
	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->PSSetShader(m_pPS, NULL, 0);


}

bool KG_Shadow::End(ID3D11DeviceContext* pContext)
{
	m_Rt->End(pContext);

	CDX::ApplyBS(pContext, CDX::KG_DxState::g_pAlpahBlend);
	CDX::ApplySS(pContext, CDX::KG_DxState::g_pSampler);
	CDX::ApplyRS(pContext, CDX::KG_DxState::g_pRSSold);
	return true;
}

bool KG_Shadow::Release()
{
	return true;
}

HRESULT KG_Shadow::LoadShader(ID3D11Device* pd3dDevice, const TCHAR* ShaderFileName, const CHAR* VSName, const CHAR* PSName)			//½¦ÀÌ´õ ·Îµå
{
	HRESULT hr = S_OK;
	ID3DBlob* pErrorBlob;

	//if (m_pVS)m_pVS->Release();
	//if (m_pPS)m_pPS->Release();
	//if (m_pVertexCode)m_pVertexCode->Release();
	//if (m_pPixelCode)m_pPixelCode->Release();

	hr = D3DX11CompileFromFile(ShaderFileName, NULL, NULL, VSName, "vs_5_0", 0, 0, NULL, &m_pVertexCode, &pErrorBlob, NULL);		//Á¤Á¡ ½¦ÀÌ´õ ÄÄÆÄÀÏ
	if (FAILED(hr))
	{
		if (pErrorBlob == nullptr)
			return hr;
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		return hr;
	}
	hr = pd3dDevice->CreateVertexShader(m_pVertexCode->GetBufferPointer(), m_pVertexCode->GetBufferSize(), NULL, &m_pVS);	//Á¤Á¡ ½¦ÀÌ´õ »ý¼º
	if (FAILED(hr))
	{
		return hr;
	}


	hr = D3DX11CompileFromFile(ShaderFileName, NULL, NULL, PSName, "ps_5_0", 0, 0, NULL, &m_pPixelCode, &pErrorBlob, NULL);			//ÇÈ¼¿ ½¦ÀÌ´õ ÄÄÆÄÀÏ
	if (FAILED(hr))
	{
		if (pErrorBlob == nullptr)
			return hr;
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		return hr;
	}
	hr = pd3dDevice->CreatePixelShader(m_pPixelCode->GetBufferPointer(), m_pPixelCode->GetBufferSize(), NULL, &m_pPS);		//ÇÈ¼¿ ½¦ÀÌ´õ »ý¼º
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

void KG_Shadow::SetLTM(D3DXMATRIX* world, D3DXVECTOR3 vPos, D3DXVECTOR3 vAt, float fDistance, float fFar)
{
	m_matWorld = *world;

	D3DXVECTOR3 vEye = D3DXVECTOR3(vPos.x, vPos.y, vPos.z);
	D3DXVECTOR3 vLookat = vAt;
	D3DXVECTOR3 vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_matView, &vEye, &vLookat, &vUp);

	D3DXMatrixOrthoOffCenterLH(&m_matProj, -fDistance / 2, fDistance / 2, -fDistance / 2, fDistance / 2, 0.0f, fFar);

	m_matLTM = m_matWorld * m_matView * m_matProj * m_matTexture;
	D3DXMatrixTranspose(&m_matLTM, &m_matLTM);
}

void KG_Shadow::GetLTM(D3DXMATRIX& world, D3DXMATRIX& View, D3DXMATRIX& Proj, D3DXMATRIX& LTM)
{
	world = m_matWorld;
	View = m_matView;
	Proj = m_matProj;
	LTM = m_matLTM;
}

//ID3D11ShaderResourceView KG_Shadow::GetDSV()
//{
//	return m_Rt->m_pDsvSRV.GetAddressOf();
//}


KG_Shadow::KG_Shadow()
{
	m_matTexture = D3DXMATRIX(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
}


KG_Shadow::~KG_Shadow()
{
}

