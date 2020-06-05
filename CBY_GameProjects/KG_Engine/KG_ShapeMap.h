#pragma once
#include "KG_Model.h"


class KG_ShapeMap : public KG_Model
{

public:
	KG_Model m_Model;

public:
	virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	HRESULT CreateVertexData();
	HRESULT CreateIndexData();
	virtual void UpdateVertexData(std::vector<PNCT_VERTEX>& list);
public:
	KG_ShapeMap();
	~KG_ShapeMap();
};


