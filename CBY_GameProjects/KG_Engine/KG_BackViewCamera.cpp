#include "KG_BackViewCamera.h"
void KG_BackViewCamera::Front(float Speed)
{
	m_At.z += Speed * g_SecondTime;
}
void KG_BackViewCamera::Side(float Speed) 
{
	m_At.x += Speed * g_SecondTime;
}
bool KG_BackViewCamera::Frame()
{
	m_fRadius += m_nMouseWhellDelta * m_fRadius * 0.1f / 120.0f;
	m_fRadius = min(m_fMaxRadius, m_fRadius);
	m_fRadius = max(m_fMinRadius, m_fRadius);
	m_nMouseWhellDelta = 0;

	if (I_Input.KeyCheck('W'))
	{
		Front(30.0f);
	}
	if (I_Input.KeyCheck('S'))
	{
		Front(-30.0f);
	}
	if (I_Input.KeyCheck('A'))
	{
		Side(-30.0f);
	}
	if (I_Input.KeyCheck('D'))
	{
		Side(30.0f);
	}

	D3DXVECTOR3 vPos = m_At + m_BackPos * m_fRadius;
	D3DXMatrixLookAtLH(&m_View, &vPos, &m_At, &m_Up);

	UpdateVector();
	return true;
}

KG_BackViewCamera::KG_BackViewCamera()
{
}


KG_BackViewCamera::~KG_BackViewCamera()
{
}
