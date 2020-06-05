#include "CBY_CharacterCamera.h"
#include "KG_Input.h"

namespace CBY
{
	bool CBY_CharacterCamera::Frame()
	{
		D3DXMATRIX matView, matLook;

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

		if (I_Input.GetKeyCheck(VK_F1))
		{
			if (m_bClip)
			{
				m_bClip = false;
			}
			else
			{
				m_bClip = true;
			}
		}

		float x = ((float)I_Input.m_Mouse.x - ((float)Winrt.right / 2.0f)) / ((float)Winrt.right / 2.0f);
		float y = (((float)Winrt.bottom / 2.0f) - (float)I_Input.m_Mouse.y) / ((float)Winrt.bottom / 2.0f);
		/*float z = 0;
		float mag = x * x + y * y;
		if (mag > 1.0f)
		{
			float scale = 1.0f / sqrtf(mag);
			x *= scale * 10;
			y *= scale * 10;
		}
		else
		{
			z = sqrtf(1.0f - mag);
		}*/
		m_fPitch += y * g_SecondTime * m_fSpeed;
		if (m_fPitch > 1.0f)
		{
			m_fPitch = 1.0f;
		}

		if (m_fPitch < -1.0f)
		{
			m_fPitch = -1.0f;
		}
		m_fYaw += x * g_SecondTime * m_fSpeed;


		D3DXMatrixRotationYawPitchRoll(&matLook, m_fYaw, m_fPitch, m_fRoll);

		D3DXMATRIX Charmat, pitchmat;
		D3DXMatrixRotationYawPitchRoll(&Charmat, m_fYaw, 0, 0);		//캐릭터에 적용할 월드행렬용
		//D3DXMatrixRotationYawPitchRoll(&pitchmat, 0, m_fPitch, 0);
		
		//Charmat *= pitchmat;

		D3DXVECTOR3 lookdir = D3DXVECTOR3(0, 0, -1);
		D3DXVECTOR3 sidedir = D3DXVECTOR3(1, 0, 0);
		D3DXVECTOR3 objdir = D3DXVECTOR3(1, 0, -1);
		D3DXVec3TransformCoord(&lookdir, &lookdir, &matLook);
		D3DXVec3TransformCoord(&sidedir, &sidedir, &matLook);
		D3DXVec3TransformCoord(&objdir, &sidedir, &matLook);
		m_LookDir = lookdir;
		m_SideDir = sidedir;
		m_ObjDir = objdir;

		D3DXVECTOR3 At;
		m_At = m_Pos + lookdir;
		D3DXMatrixLookAtLH(&m_View, &m_Pos, &m_At, &m_DefaultUp);
		m_World = Charmat;
		m_MousePos.x = matLook._41;
		m_MousePos.y = matLook._42;
		m_MousePos.z = matLook._43;
		m_World._41 = 0;
		m_World._42 = 0;
		m_World._43 = 0;

		if (m_bClip)
		{
			m_Center.x = Winrt.right / 2;
			m_Center.y = Winrt.bottom / 2;
			ClientToScreen(g_hwnd, &m_Center);
			SetCursorPos(m_Center.x, m_Center.y);
		}
		UpdateVector();
		return true;
	}



	void CBY_CharacterCamera::Update(D3DXVECTOR4 rotvalue)
	{
		D3DXQUATERNION qRot;
		D3DXMATRIX matRot;
		D3DXQuaternionRotationYawPitchRoll(&qRot, rotvalue.y, rotvalue.x, rotvalue.z);
		D3DXMatrixAffineTransformation(&matRot, 1.0f, NULL, &qRot, &m_Pos);
		D3DXMatrixInverse(&m_View, NULL, &matRot);
	}

	void CBY_CharacterCamera::SetClientCursor(fPOINT pot)
	{
		m_Center.x = pot.x;
		m_Center.y = pot.y;
	}

	void CBY_CharacterCamera::SetCameraSpeed(float fspeed)
	{
		m_fSpeed = fspeed;
	}

	CBY_CharacterCamera::CBY_CharacterCamera()
	{
		D3DXMatrixIsIdentity(&m_mModelLastRot);
		m_At = D3DXVECTOR3(0, 0, 0);
		m_LookDir = D3DXVECTOR3(0, 0, 0);
		m_bClip = true;
	}

	bool CBY_CharacterCamera::SetViewProj()
	{
		CreateViewMatrix(m_Pos, m_At);
		float fAspect = (float)Winrt.right / (float)Winrt.bottom;
		CreateProjMatrix(1.0f, 1000.0f, D3DX_PI * 0.5f, fAspect);
		return true;
	}


	CBY_CharacterCamera::~CBY_CharacterCamera()
	{
	}
}

