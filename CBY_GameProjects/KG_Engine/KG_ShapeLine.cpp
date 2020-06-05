#include "KG_ShapeLine.h"

HRESULT KG_ShapeLine::CreateInputLayout()
{
	HRESULT hr = S_OK;
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iElementCount = sizeof(layout) / sizeof(layout[0]);
	hr = m_obj.m_pd3dDevice->CreateInputLayout(layout, iElementCount, m_obj.m_pVertexCode->GetBufferPointer(), m_obj.m_pVertexCode->GetBufferSize(), &m_obj.m_pVertexLayout);
	return hr;
}
HRESULT KG_ShapeLine::CreateVertexData()
{
	HRESULT hr = S_OK;
	m_obj.m_VertexSize = sizeof(PC_VERTEX);
	m_VertexLineData.resize(2);
	m_VertexLineData[0].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_VertexLineData[1].p = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
	m_VertexLineData[0].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexLineData[1].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	//Convert(m_VerTex);	
	return hr;
}
HRESULT KG_ShapeLine::CreateIndexData()
{
	HRESULT hr = S_OK;
	m_IndexData.resize(2);
	m_IndexData[0] = 0;
	m_IndexData[1] = 1;
	return hr;
}
HRESULT KG_ShapeLine::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	if (m_VertexLineData.size() <= 0) return E_FAIL;
	m_obj.m_Vertexnum = m_VertexLineData.size();
	D3D11_BUFFER_DESC pDesc;
	ZeroMemory(&pDesc, sizeof(D3D11_BUFFER_DESC));
	pDesc.Usage = D3D11_USAGE_DEFAULT;
	pDesc.ByteWidth = m_obj.m_VertexSize * m_VertexLineData.size();
	pDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA pInitialData;
	ZeroMemory(&pInitialData,
		sizeof(D3D11_SUBRESOURCE_DATA));
	pInitialData.pSysMem = &m_VertexLineData.at(0);

	hr = m_obj.m_pd3dDevice->CreateBuffer(&pDesc,
		&pInitialData,
		&m_obj.m_pVertexBuffer);
	return hr;
}
bool KG_ShapeLine::PostRender()
{
	m_obj.m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//m_obj.m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_obj.PostPender();
	return true;
}
bool	KG_ShapeLine::Draw(D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR4 color)
{
	m_VertexLineData[0].p = v0;
	m_VertexLineData[0].c = color;
	m_VertexLineData[1].p = v1;
	m_VertexLineData[1].c = color;
	m_obj.m_pContext->UpdateSubresource(
		m_obj.m_pVertexBuffer.Get(),
		0, NULL,
		&m_VertexLineData.at(0), 0, 0);
	return Render();
}
KG_ShapeLine::KG_ShapeLine()
{
}


KG_ShapeLine::~KG_ShapeLine()
{
}
