#pragma once
#include "KG_Model.h"

class KG_ShapeBox : public KG_Model
{
public:
	KG_Model m_Model;
	HRESULT CreateVertexData();
	HRESULT CreateIndexData();

public:
	KG_ShapeBox();
	~KG_ShapeBox();
};

