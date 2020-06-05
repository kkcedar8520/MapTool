#pragma once
#include "KG_Model.h"

class KG_ShapeObject : public KG_Model
{
public:
	KG_Model m_Model;

public:
	virtual HRESULT CreateVertexData();
	virtual HRESULT CreateIndexData();
	virtual void UpdateVertexData(std::vector<PNCT_VERTEX>& list);					//정점 버퍼 데이터 업데이트

public:
	KG_ShapeObject();
	~KG_ShapeObject();
};


