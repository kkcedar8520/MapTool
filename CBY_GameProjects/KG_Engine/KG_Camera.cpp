#include "KG_Camera.h"

void KG_Camera::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_LBUTTONDOWN)
	{
		int iMouseX = LOWORD(lParam);
		int iMouseY = HIWORD(lParam);
		m_ViewArcBall.OnBegin(iMouseX, iMouseY);
	}
	if (msg == WM_RBUTTONDOWN)
	{
		int iMouseX = LOWORD(lParam);
		int iMouseY = HIWORD(lParam);
		m_WorldArcBall.OnBegin(iMouseX, iMouseY);
	}
	if (msg == WM_LBUTTONUP)
	{
		m_ViewArcBall.OnEnd();
	}
	if (msg == WM_RBUTTONUP)
	{
		m_WorldArcBall.OnEnd();
	}
	if (msg == WM_MOUSEMOVE)
	{
		int iMouseX = LOWORD(lParam);
		int iMouseY = HIWORD(lParam);
		m_WorldArcBall.OnMove(iMouseX, iMouseY);
		m_ViewArcBall.OnMove(iMouseX, iMouseY);
	}
	if (msg == WM_MOUSEWHEEL)
	{
		m_nMouseWhellDelta = -(short)HIWORD(wParam);
	}
}

bool KG_Camera::Init()
{
	return true;
}

bool KG_Camera::SetViewProj()
{
	CreateViewMatrix(D3DXVECTOR3(0, 0.0f, -1.0f), D3DXVECTOR3(0, 0.0f, 0));
	float fAspect = (float)Winrt.right / (float)Winrt.bottom;
	CreateProjMatrix(1.0f, 1000.0f, D3DX_PI * 0.5f, fAspect);
	return true;
}

void KG_Camera::CreateViewMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 target, D3DXVECTOR3 up)
{
	m_Pos = pos;
	m_At = target;
	m_DefaultUp = up;
	m_fRadius = D3DXVec3Length(&(m_Pos - m_At));
	D3DXVec3Normalize(&m_BackPos, &(m_Pos - m_At));
	D3DXMatrixLookAtLH(&m_View, &pos, &target, &up);
}

void KG_Camera::CreateProjMatrix(float fNear, float fFar, float fFovY, float fAspect)
{
	m_fNear = fNear;
	m_fFar = fFar;
	m_fFovY = fFovY;
	m_fAspect = fAspect;
	D3DXMatrixPerspectiveFovLH(&m_Proj, fFovY, fAspect, fNear, fFar);

	D3DXMatrixOrthoLH(&m_OrthoProj,
		Winrt.right,
		Winrt.bottom,
		fNear, fFar);
}

bool KG_Camera::Frame()
{
	D3DXMatrixLookAtLH(&m_View, &m_Pos, &m_At, &m_DefaultUp);

	UpdateVector();
	return true;
}

void KG_Camera::UpdateVector()
{
	m_Look.x = m_View._13;
	m_Look.y = m_View._23;
	m_Look.z = m_View._33;
	m_Side.x = m_View._11;
	m_Side.y = m_View._21;
	m_Side.z = m_View._31;
	m_Up.x = m_View._12;
	m_Up.y = m_View._22;
	m_Up.z = m_View._32;

	D3DXVec3Normalize(&m_Look, &m_Look);
	D3DXVec3Normalize(&m_Side, &m_Side);
	D3DXVec3Normalize(&m_Up, &m_Up);

	m_Frustum.SetMatrix(NULL, &m_View, &m_Proj);
}

int	KG_Camera::CheckOBBInPlane(KG_Box& box)
{
	D3DXVECTOR3 vDir;
	float  fDistance = 0.0f;
	float  fPlaneToCenterDistance = 0.0f;
	int t_Pos = P_FRONT;

	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = box.vAxis[0] * box.fExtent[0];
		fDistance = fabs(m_Frustum.m_Plane[iPlane].fA *
			vDir.x +
			m_Frustum.m_Plane[iPlane].fB *
			vDir.y +
			m_Frustum.m_Plane[iPlane].fC *
			vDir.z);

		vDir = box.vAxis[1] * box.fExtent[1];
		fDistance += fabs(m_Frustum.m_Plane[iPlane].fA *
			vDir.x +
			m_Frustum.m_Plane[iPlane].fB *
			vDir.y +
			m_Frustum.m_Plane[iPlane].fC *
			vDir.z);

		vDir = box.vAxis[2] * box.fExtent[2];
		fDistance += fabs(m_Frustum.m_Plane[iPlane].fA *
			vDir.x +
			m_Frustum.m_Plane[iPlane].fB *
			vDir.y +
			m_Frustum.m_Plane[iPlane].fC *
			vDir.z);

		fPlaneToCenterDistance =
			m_Frustum.m_Plane[iPlane].fA *
			box.vCenter.x +
			m_Frustum.m_Plane[iPlane].fB *
			box.vCenter.y +
			m_Frustum.m_Plane[iPlane].fC *
			box.vCenter.z +
			m_Frustum.m_Plane[iPlane].fD;

		if (fPlaneToCenterDistance <= fDistance)
		{
			t_Pos = P_SPANNING;
		}

		if (fPlaneToCenterDistance < -fDistance)
		{
			return  P_BACK;
		}
	}
	return t_Pos;
}
bool KG_Camera::Render()
{
	return true;
}

bool KG_Camera::Release()
{
	return true;
}



KG_Camera::KG_Camera()
{
	D3DXMatrixIdentity(&m_World);
	m_nMouseWhellDelta = 0;
	//m_Pos = D3DXVECTOR3(0, 1.0f, 0);
	m_fRadius = 1.0f;
	m_fMinRadius = 1.0f;
	m_fSpeed = 10;
	m_fMaxRadius = FLT_MAX;
	m_fYaw = 0;
	m_fPitch = 0;
	m_fRoll = 0;
	m_At = D3DXVECTOR3(0, 0, 0);
	m_LookDir = D3DXVECTOR3(0, 0, 0);
	m_SideDir = D3DXVECTOR3(0, 0, 0);
	m_ObjDir = D3DXVECTOR3(0, 0, 0);
}


KG_Camera::~KG_Camera()
{
}
