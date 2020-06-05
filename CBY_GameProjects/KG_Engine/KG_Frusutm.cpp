#include "KG_Frusutm.h"

void KG_Frusutm::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	D3DXMATRIX matFrustum = *pView * *pProj;
	D3DXMATRIX matInvFrustum;
	D3DXMatrixInverse(&matInvFrustum, NULL, &matFrustum);				//투영행렬까지 반영된 상태에선 박스의 형태이고 거기서 행렬을 빼주면 피라미드 프러스텀 박스가 되기에 박스 형태로 정점 선언
	m_vFrustum[0] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	m_vFrustum[1] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_vFrustum[2] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_vFrustum[3] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);

	m_vFrustum[4] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_vFrustum[5] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_vFrustum[6] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_vFrustum[7] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&m_vFrustum[i], &m_vFrustum[i], &matInvFrustum);		//박스로된 프러스텀에 행렬을 빼주면 우리가 원하는 시야각이 나타난다.
	}

	// 노말 방향 안쪽으로
	m_Plane[0].CreatePlane(m_vFrustum[5], m_vFrustum[0], m_vFrustum[1]);	// 좌 평면(left)
	m_Plane[1].CreatePlane(m_vFrustum[3], m_vFrustum[6], m_vFrustum[2]);	// 우 평면(right)
	m_Plane[2].CreatePlane(m_vFrustum[5], m_vFrustum[2], m_vFrustum[6]);	// 상 평면(top)
	m_Plane[3].CreatePlane(m_vFrustum[0], m_vFrustum[7], m_vFrustum[3]);	// 하 평면(bottom)
	m_Plane[4].CreatePlane(m_vFrustum[0], m_vFrustum[2], m_vFrustum[1]);	// 근 평면(near)
	m_Plane[5].CreatePlane(m_vFrustum[6], m_vFrustum[4], m_vFrustum[5]);	// 원 평면(far)
}

KG_Frusutm::KG_Frusutm()
{
}


KG_Frusutm::~KG_Frusutm()
{
}
