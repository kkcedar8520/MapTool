#pragma once
#include "KG_ShapeBox.h"

namespace CBY
{
	class CBY_BoneBox : public KG_ShapeBox
	{
	public:
		D3DXVECTOR3 m_vMax;
		D3DXVECTOR3 m_vMin;

	public:
		HRESULT CreateVertexData();
		HRESULT CreateIndexData();
		void SetBox(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax);

	public:
		CBY_BoneBox();
		virtual ~CBY_BoneBox();
	};
}

