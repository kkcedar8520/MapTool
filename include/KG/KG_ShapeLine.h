#pragma once
#include "KG_Model.h"

class KG_ShapeLine :public KG_Model
{
	std::vector<PC_VERTEX>	m_VertexLineData;
public:
	HRESULT CreateVertexData();
	HRESULT CreateIndexData();
	HRESULT CreateInputLayout();
	HRESULT CreateVertexBuffer();
	bool    PostRender();
	bool	Draw(D3DXVECTOR3 v0,
		D3DXVECTOR3 v1,
		D3DXVECTOR4 color);
public:
	KG_ShapeLine();
	virtual ~KG_ShapeLine();
};

