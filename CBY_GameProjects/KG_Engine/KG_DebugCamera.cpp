#include "KG_DebugCamera.h"

void KG_DebugCamera::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	/*if (msg == WM_LBUTTONDOWN)
	{
		int iMouseX = LOWORD(lParam);
		int iMouseY = HIWORD(lParam);
		m_bDrag = true;
		m_LastPos.x = iMouseX;
		m_LastPos.y = iMouseY;
	}
	if (msg == WM_LBUTTONUP)
	{
		m_bDrag = false;
	}
	if (msg == WM_MOUSEMOVE)
	{
		int iMouseX = LOWORD(lParam);
		int iMouseY = HIWORD(lParam);
		if (m_bDrag)
		{
			m_fYaw -= (iMouseX - m_LastPos.x)*g_SecondTime * 2;
			m_fPitch -= (iMouseY - m_LastPos.y)*g_SecondTime * 2;
			m_LastPos.x = iMouseX;
			m_LastPos.y = iMouseY;
		}
	}

	if (msg == WM_MOUSEWHEEL)
	{
		SHORT sWHEEL = HIWORD(wParam);
		if (sWHEEL > 0)
		{
			m_Pos += m_Look *g_SecondTime * 1500;
		}
		else
		{
			m_Pos -= m_Look *g_SecondTime * 1500;
		}
	}*/
}

bool KG_DebugCamera::Frame()
{
	if (I_Input.GetKeyCheck('W'))					//카메라의 이동
	{
		m_Pos += m_Look * g_SecondTime * 30;
	}

	if (I_Input.GetKeyCheck('S'))
	{
		m_Pos -= m_Look * g_SecondTime * 30;
	}
	if (I_Input.GetKeyCheck(VK_UP))					//시점 변환
	{
		m_fPitch -= 2 * g_SecondTime;
	}

	if (I_Input.GetKeyCheck(VK_DOWN))
	{
		m_fPitch += 2 * g_SecondTime;
	}

	if (I_Input.GetKeyCheck(VK_LEFT))
	{
		m_fYaw -= 2 * g_SecondTime;
	}

	if (I_Input.GetKeyCheck(VK_RIGHT))
	{
		m_fYaw += 2 * g_SecondTime;
	}

	if (I_Input.GetKeyCheck(VK_SPACE))
	{
		m_fYaw = 0;
		m_fPitch = 0;
		m_fRoll = 0;
		m_Pos = D3DXVECTOR3(0, 3, 0);
	}
	Update(D3DXVECTOR4(m_fPitch, m_fYaw, m_fRoll, 0));
	UpdateVector();
	return true;
}

void KG_DebugCamera::Update(D3DXVECTOR4 rotvalue)
{
	D3DXQUATERNION qRot;
	D3DXMATRIX matRot;
	D3DXQuaternionRotationYawPitchRoll(&qRot, rotvalue.y, rotvalue.x, rotvalue.z);
	D3DXMatrixAffineTransformation(&matRot, 1.0f, NULL, &qRot, &m_Pos);
	D3DXMatrixInverse(&m_View, NULL, &matRot);
}

KG_DebugCamera::KG_DebugCamera()
{
	m_fYaw = 0;
	m_fPitch = 0;
	m_fRoll = 0;
	m_bDrag = false;
}


KG_DebugCamera::~KG_DebugCamera()
{
}
