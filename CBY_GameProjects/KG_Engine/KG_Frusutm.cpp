#include "KG_Frusutm.h"

void KG_Frusutm::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	D3DXMATRIX matFrustum = *pView * *pProj;
	D3DXMATRIX matInvFrustum;
	D3DXMatrixInverse(&matInvFrustum, NULL, &matFrustum);				//������ı��� �ݿ��� ���¿��� �ڽ��� �����̰� �ű⼭ ����� ���ָ� �Ƕ�̵� �������� �ڽ��� �Ǳ⿡ �ڽ� ���·� ���� ����
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
		D3DXVec3TransformCoord(&m_vFrustum[i], &m_vFrustum[i], &matInvFrustum);		//�ڽ��ε� �������ҿ� ����� ���ָ� �츮�� ���ϴ� �þ߰��� ��Ÿ����.
	}

	// �븻 ���� ��������
	m_Plane[0].CreatePlane(m_vFrustum[5], m_vFrustum[0], m_vFrustum[1]);	// �� ���(left)
	m_Plane[1].CreatePlane(m_vFrustum[3], m_vFrustum[6], m_vFrustum[2]);	// �� ���(right)
	m_Plane[2].CreatePlane(m_vFrustum[5], m_vFrustum[2], m_vFrustum[6]);	// �� ���(top)
	m_Plane[3].CreatePlane(m_vFrustum[0], m_vFrustum[7], m_vFrustum[3]);	// �� ���(bottom)
	m_Plane[4].CreatePlane(m_vFrustum[0], m_vFrustum[2], m_vFrustum[1]);	// �� ���(near)
	m_Plane[5].CreatePlane(m_vFrustum[6], m_vFrustum[4], m_vFrustum[5]);	// �� ���(far)
}

KG_Frusutm::KG_Frusutm()
{
}


KG_Frusutm::~KG_Frusutm()
{
}
