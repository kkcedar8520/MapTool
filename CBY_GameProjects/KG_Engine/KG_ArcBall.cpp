#include "KG_ArcBall.h"

D3DXVECTOR3 KG_ArcBall::ScreenToVector(float nX, float nY)
{
	float x = (nX - (Winrt.right / 2)) / (Winrt.right / 2);
	float y = ((Winrt.bottom / 2) - nY) / (Winrt.bottom / 2);
	float z = 0.0f;

	float mag = x * x + y * y;
	if (mag > 1.0f)
	{
		float scale = 1.0f / sqrtf(mag);
		x *= scale;
		y *= scale;
	}
	else
	{
		z = sqrtf(1.0f - mag);
	}
	return D3DXVECTOR3(x, y, z);
}
D3DXQUATERNION KG_ArcBall::QuatFromBallPoints(const D3DXVECTOR3& vFrom, const D3DXVECTOR3& vTo)
{
	float fDot = D3DXVec3Dot(&vFrom, &vTo);
	D3DXVECTOR3 vAxis;
	D3DXVec3Cross(&vAxis, &vFrom, &vTo);
	return D3DXQUATERNION(vAxis.x, vAxis.y, vAxis.z, fDot);

}

void  KG_ArcBall::OnBegin(int nX, int nY)
{
	m_bDrag = true;
	m_qDown = m_qNow;
	// 2D -> 3D
	m_vDownPt = ScreenToVector(nX, nY);
}
void  KG_ArcBall::OnMove(int nX, int nY)
{
	if (m_bDrag)
	{
		m_vCurrentPt = ScreenToVector(nX, nY);
		m_qNow = m_qDown * QuatFromBallPoints(m_vDownPt, m_vCurrentPt);
	}
}
void  KG_ArcBall::OnEnd()
{
	m_bDrag = false;
}

KG_ArcBall::KG_ArcBall()
{
	m_bDrag = false;
	D3DXQuaternionIdentity(&m_qDown);
	D3DXQuaternionIdentity(&m_qNow);
}


KG_ArcBall::~KG_ArcBall()
{
}
