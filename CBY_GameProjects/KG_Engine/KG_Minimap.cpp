#include "KG_Minimap.h"

HRESULT KG_Minimap::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	m_pd3dDevice = pd3dDevice;
	m_Context = Context;
	m_pMiniMap = std::make_shared<KG_ShapeObject>();
	m_RT = std::make_shared<KG_DxRT>();


	m_pMiniMap->Create(m_pd3dDevice, m_Context, L"../../data/shader/LightBlend.txt", nullptr);

	D3DXVECTOR3 pos = D3DXVECTOR3(0, 1000, 0);
	D3DXVECTOR3 at = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&m_matTopView, &pos, &at, &up);

	D3DXMatrixPerspectiveFovLH(&m_matTopProj, D3DX_PI * 0.4f, 1, 1, 20000);
	return S_OK;
}

void KG_Minimap::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	m_pMiniMap->SetMatrix(world, &m_matTopView, &m_matTopProj);
}

bool KG_Minimap::Render()
{
	m_pMiniMap->PreRender();
	m_Context->PSSetShaderResources(0, 1, m_RT->m_pSRV.GetAddressOf());
	m_pMiniMap->PostRender();
	return true;
}

bool KG_Minimap::Release()
{
	if (m_pMiniMap)
	{
		m_pMiniMap->Release();
		m_pMiniMap = 0;
	}
	if (m_RT)
	{
		m_RT->Release();
		m_RT = 0;
	}
	return true;
}

KG_Minimap::KG_Minimap()
{
}


KG_Minimap::~KG_Minimap()
{
}
