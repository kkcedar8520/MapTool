#pragma once
#include "KG_ShapeBox.h"

class KG_Frusutm
{
public:
	D3DXVECTOR3 m_vFrustum[8];
	KG_Plane m_Plane[6];					//프러스텀은 8개의 점과 6개의 평면을 가진다 (박스에서 투영을 반영해 변한것이니 박스로 생각하면 편하다)

public:
	void SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);

public:
	KG_Frusutm();
	virtual ~KG_Frusutm();
};
