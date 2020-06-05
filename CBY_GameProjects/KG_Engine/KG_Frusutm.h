#pragma once
#include "KG_ShapeBox.h"

class KG_Frusutm
{
public:
	D3DXVECTOR3 m_vFrustum[8];
	KG_Plane m_Plane[6];					//���������� 8���� ���� 6���� ����� ������ (�ڽ����� ������ �ݿ��� ���Ѱ��̴� �ڽ��� �����ϸ� ���ϴ�)

public:
	void SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);

public:
	KG_Frusutm();
	virtual ~KG_Frusutm();
};
