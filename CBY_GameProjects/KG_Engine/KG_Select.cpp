#include "KG_Select.h"

D3DXVECTOR3 g_fPickpos;

bool KG_Select::PickCheck(float* pft, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2)
{
	float t, u, v;

	D3DXVECTOR3 vT = m_Ray.m_Orig - v0;
	D3DXVECTOR3 vE0 = v1 - v0;
	D3DXVECTOR3 vE1 = v2 - v0;

	D3DXVECTOR3 vP, vQ;
	D3DXVec3Cross(&vP, &m_Ray.m_Dir, &vE1);
	D3DXVec3Cross(&vQ, &vT, &vE0);

	float dot = D3DXVec3Dot(&vP, &vE0);

	t = D3DXVec3Dot(&vQ, &vE1) / dot;
	u = D3DXVec3Dot(&vP, &vT) / dot;
	v = D3DXVec3Dot(&vQ, &m_Ray.m_Dir) / dot;

	if (u<0 || u>1.0f || v<0 || v>1.0f || (u + v) > 1.0f)
	{
		return false;
	}


	if (*pft > t)
	{
		*pft = t;
	}

	return true;
}


bool KG_Select::ChkOBBToRay(KG_Box* pbox)
{
	float t_min = -999999.0f;
	float t_max = 999999.0f;
	float  f[3], fa[3], s[3], sa[3];

	D3DXVECTOR3 vR = m_Ray.m_Orig - pbox->vCenter;

	for (int v = 0; v < 3; v++)
	{
		f[v] = D3DXVec3Dot(&pbox->vAxis[v], &m_Ray.m_Dir);
		s[v] = D3DXVec3Dot(&pbox->vAxis[v], &vR);
		fa[v] = fabs(f[v]);
		sa[v] = fabs(s[v]);

		if (sa[v] > pbox->fExtent[v] && s[v] * f[v] >= 0.0f)
			return false;

		float t1 = (-s[v] - pbox->fExtent[v]) / f[v];
		float t2 = (-s[v] + pbox->fExtent[v]) / f[v];
		if (t1 > t2)
		{
			std::swap(t1, t2);
		}
		t_min = max(t_min, t1);
		t_max = min(t_max, t2);
		if (t_min > t_max)
			return false;
	}

	return true;
}
bool KG_Select::AABBtoRay(KG_Box* pbox)
{
	

	float t_min = 0;
	float t_max = 999999.0f;
	for (int i = 0; i < 3; i++)
	{
		// 축에 평행할 경우
		
		if (abs(m_Ray.m_Dir[i]) < 0.0001f)
		{
			if (m_Ray.m_Orig[i] < pbox->vMin[i] || m_Ray.m_Orig[i] > pbox->vMax[i])
				return false;
		}
		else
		{
			float denom = 1.0f / m_Ray.m_Dir[i];
			float t1 = (-m_Ray.m_Orig[i] - pbox->vMin[i]) * denom;
			float t2 = (-m_Ray.m_Orig[i] - pbox->vMax[i]) * denom;
			if (t1 > t2)
			{
				std::swap(t1, t2);
			}
			t_min = max(t_min, t1);
			t_max = min(t_max, t2);
			if (t_min > t_max)
				return false;
		}
	}
	m_vIntersection = m_Ray.m_Orig + m_Ray.m_Dir* t_min;
	return true;
}
bool KG_Select::OBBToRay(KG_Box* pBox)
{
	float t_min = -999999.0f;
	float t_max = 999999.0f;
	float  f[3], fa[3], s[3], sa[3];

	D3DXVECTOR3 vR = m_Ray.m_Orig - pBox->vCenter;

	for (int v = 0; v < 3; v++)
	{
		f[v] = D3DXVec3Dot(&pBox->vAxis[v], &m_Ray.m_Dir);
		s[v] = D3DXVec3Dot(&pBox->vAxis[v], &vR);
		fa[v] = fabs(f[v]);
		sa[v] = fabs(s[v]);

		if (sa[v] > pBox->fExtent[v] && s[v] * f[v] >= 0.0f)
			return false;

		float t1 = (-s[v] - pBox->fExtent[v]) / f[v];
		float t2 = (-s[v] + pBox->fExtent[v]) / f[v];
		if (t1 > t2)
		{
			std::swap(t1, t2);
		}
		t_min = max(t_min, t1);
		t_max = min(t_max, t2);
		if (t_min > t_max)
			return false;
	}

	float  fCross[3], fRhs;
	D3DXVECTOR3 vDxR;
	D3DXVec3Cross(&vDxR, &m_Ray.m_Dir, &vR);
	// D X pBox->vAxis[0]  분리축
	fCross[0] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[0]));
	float fAxis2 = pBox->fExtent[1] * fa[2];
	float fAxis1 = pBox->fExtent[2] * fa[1];
	fRhs = fAxis2 + fAxis1;
	if (fCross[0] > fRhs)
	{
		return false;
	}
	// D x pBox->vAxis[1]  분리축
	fCross[1] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[1]));
	fRhs = pBox->fExtent[0] * fa[2] + pBox->fExtent[2] * fa[0];
	if (fCross[1] > fRhs)
	{
		return false;
	}
	// D x pBox->vAxis[2]  분리축
	fCross[2] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[2]));
	fRhs = pBox->fExtent[0] * fa[1] + pBox->fExtent[1] * fa[0];
	if (fCross[2] > fRhs)
	{
		return false;
	}

	m_vIntersection = m_Ray.m_Orig + m_Ray.m_Dir* t_min;
	return true;
}

void KG_Select::Update()
{
	m_Ray.m_Dir.x = (((2.0f * I_Input.m_Mouse.x) / Winrt.right) - 1.0f) / m_Proj._11;
	m_Ray.m_Dir.y = (((-2.0f * I_Input.m_Mouse.y) / Winrt.bottom) + 1.0f) / m_Proj._22;
	m_Ray.m_Dir.z = 1.0f;

	D3DXMATRIX invview;
	D3DXMatrixInverse(&invview, NULL, &m_View);
	m_Ray.m_Orig = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&m_Ray.m_Orig, &m_Ray.m_Orig, &invview);
	D3DXVec3TransformNormal(&m_Ray.m_Dir, &m_Ray.m_Dir, &invview);
	D3DXVec3Normalize(&m_Ray.m_Dir, &m_Ray.m_Dir);
}

void KG_Select::SetMarix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != nullptr)
	{
		m_World = *pWorld;
	}

	if (pView != nullptr)
	{
		m_View = *pView;
	}

	if (pProj != nullptr)
	{
		m_Proj = *pProj;
	}

	Update();
}

D3DXVECTOR3 KG_Select::GetIntersection(float t)
{
	D3DXVECTOR3 inter;
	inter = m_Ray.m_Orig + (m_Ray.m_Dir*t);
	return inter;
}

KG_Select::KG_Select()
{
	D3DXMatrixIdentity(&m_World);
	D3DXMatrixIdentity(&m_View);
	D3DXMatrixIdentity(&m_Proj);
}


KG_Select::~KG_Select()
{
}

