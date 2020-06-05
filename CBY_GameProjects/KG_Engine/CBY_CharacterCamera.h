#pragma once
#include "KG_Camera.h"

namespace CBY
{
	class CBY_CharacterCamera : public KG_Camera
	{
	public:
		D3DXMATRIX m_mModelLastRot;
		D3DXMATRIX m_mModelRot;
		bool m_bClip;
		POINT m_Center;
		D3DXVECTOR3 m_MousePos;

	public:
		virtual bool Frame();

	public:
		virtual void Update(D3DXVECTOR4 rotvalue);
		virtual void SetClientCursor(fPOINT pot);
		virtual bool SetViewProj();

	public:
		virtual void SetCameraSpeed(float fspeed);
		//virtual float GetCameraSpeed(float fspeed);


	public:
		CBY_CharacterCamera();
		virtual ~CBY_CharacterCamera();
	};
}

