#include "KG_ShapeObject.h"

HRESULT KG_ShapeObject::CreateVertexData()
{
	HRESULT hr = S_OK;

	m_obj.m_VertexSize = sizeof(PNCT_VERTEX);

	m_VerTex.resize(4);
	m_VerTex[0].p = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_VerTex[1].p = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_VerTex[2].p = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	m_VerTex[3].p = D3DXVECTOR3(1.0f, -1.0f, 0.0f);				//정점 좌표 입력

	m_VerTex[0].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
	m_VerTex[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VerTex[2].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VerTex[3].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);									//정점 컬러값 입력

	m_VerTex[0].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VerTex[1].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VerTex[2].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VerTex[3].t = D3DXVECTOR2(1.0f, 1.0f);												//정점 텍스쳐 좌표
	return hr;
}

HRESULT KG_ShapeObject::CreateIndexData()
{
	HRESULT hr = S_OK;

	m_IndexData.resize(6);
	m_IndexData[0] = 0;
	m_IndexData[1] = 1;
	m_IndexData[2] = 2;

	m_IndexData[3] = 2;
	m_IndexData[4] = 1;
	m_IndexData[5] = 3;

	return hr;
}

void KG_ShapeObject::UpdateVertexData(std::vector<PNCT_VERTEX>& list)
{
	int size = list.size();
	m_VerTex.clear();
	m_VerTex.resize(size);

	for (int i = 0; i < size; i++)
	{
		m_VerTex[i] = list[i];
	}
	if (m_obj.m_pVertexBuffer != nullptr)
	{
		m_obj.m_pContext->UpdateSubresource(m_obj.m_pVertexBuffer.Get(), 0, nullptr, &m_VerTex.at(0), 0, 0);
	}
}

KG_ShapeObject::KG_ShapeObject()
{
}


KG_ShapeObject::~KG_ShapeObject()
{
}

