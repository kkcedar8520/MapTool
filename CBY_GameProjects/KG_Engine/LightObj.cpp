#pragma once

#include "LightObj.h"


namespace JH {



	void LightObj::SetMatrix(D3DXMATRIX* matWorld,
		D3DXMATRIX* matView,
		D3DXMATRIX* matProj)
	{
		m_LightObj.SetMatrix(matWorld, matView, matProj);
	}


	void LightObj::CreateLightObj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LightInfo light)
	{

		m_obj.m_pd3dDevice = pDevice;
		m_obj.m_pContext = pContext;



		m_LightObj.Create(pDevice, pContext, L"../../data/Shader/LightShader.txt", L"../../data/Resource/powerbattles_title.gif","VS","PS");


		m_Info = light;


		m_Info.LightDir = light.At - light.LightPos;



		D3DXVec3Normalize(&m_Info.LightDir, &m_Info.LightDir);









		LightObj::Init();
	}


	bool LightObj::Init()
	{
		m_Shadow.m_DSFormat = DXGI_FORMAT_R32_TYPELESS;
		m_Shadow.Create(m_obj.m_pd3dDevice);
		m_LightObj.m_matWorld._41 = m_Info.LightPos.x;
		m_LightObj.m_matWorld._42 = m_Info.LightPos.y;
		m_LightObj.m_matWorld._43 = m_Info.LightPos.z;

		D3DXMatrixScaling(&m_MatScale, 3, 3, 3);


		m_LightObj.m_matWorld = m_LightObj.m_matWorld*m_MatScale;
		return true;
	}

	void LightObj::Update(D3DXVECTOR3 m_EyePos, D3DXVECTOR3 m_EyeDirection)
	{


		//D3DXMatrixLookAtLH(,)
		//m_obj.m_matWorld


	}
	bool LightObj::Frame()
	{
		float Angle = g_SecondTime * D3DX_PI / 2000.0f;
		D3DXMATRIX matRot;
		D3DXMatrixRotationY(&matRot, Angle);

		D3DXVECTOR3 Lightpos;
		Lightpos.x = m_Info.LightPos.x;
		Lightpos.y = m_Info.LightPos.y;
		Lightpos.z = m_Info.LightPos.z;


		D3DXVec3TransformCoord(&Lightpos, &Lightpos, &matRot);
		m_Info.LightPos.x = Lightpos.x;
		m_Info.LightPos.y = Lightpos.y;
		m_Info.LightPos.z = Lightpos.z;

		m_Info.LightDir.x = m_Info.At.x - m_Info.LightPos.x;
		m_Info.LightDir.y = m_Info.At.y - m_Info.LightPos.y;
		m_Info.LightDir.z = m_Info.At.z - m_Info.LightPos.z;

		D3DXVec3Normalize(&m_Info.LightDir, &m_Info.LightDir);
		//m_cbLight.vLightPos.x = m_cbLight.vLightPos.x*cosf(g_fProgramTime);
		//m_cbLight.vLightPos.z = m_cbLight.vLightPos.z*sinf(-g_fProgramTime);

		m_LightObj.m_vPos.x = Lightpos.x;
		m_LightObj.m_vPos.z = Lightpos.z;

		m_LightObj.m_matWorld._41 = Lightpos.x;
		m_LightObj.m_matWorld._42 = Lightpos.y;
		m_LightObj.m_matWorld._43 = Lightpos.z;


		m_LightObj.Frame();

		float fDistance = D3DXVec3Length(&(m_Info.At - m_Info.LightPos));

		m_Shadow.SetLTM(&m_LightObj.m_matWorld, m_LightObj.m_vPos, m_Info.At, fDistance);

		D3DXMATRIX matworld;
		m_Shadow.GetLTM(matworld, matworld, matworld, m_Info.matLTM);
		return true;
	}
	bool LightObj::Render()
	{
		m_LightObj.Render();

		return true;
	}
	bool LightObj::Release()
	{
		return true;
	}

	LightObj::LightObj()
	{
	}
	LightObj::~LightObj()
	{
	}
}