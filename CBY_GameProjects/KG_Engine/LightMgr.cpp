#pragma once
#include "LightMgr.h"
namespace JH {
	void LightMgr::SetCamera(KG_Camera* pCamera)
	{
		m_pCamera = pCamera;
	}
	void LightMgr::GetDevice(ID3D11Device* pdevcie)
	{
		m_pDevice = pdevcie;
	}
	void LightMgr::GetContext(ID3D11DeviceContext* pContext)
	{
		m_pContext = pContext;
	}
	int  LightMgr::LightDataLoad(const TCHAR* pszLoad)
	{
		TCHAR pBuffer[256] = { 0 };
		TCHAR pTemp[64] = { 0 };

		int iNumSprite = 0;
		FILE* fp_src;
		_wfopen_s(&fp_src, pszLoad, _T("rt"));
		if (fp_src == NULL) return false;

		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s%d%s"), pTemp, _countof(pTemp), &iNumSprite);
		m_LightInfoList.resize(iNumSprite);

		for (int iCnt = 0; iCnt < iNumSprite; iCnt++)
		{
			int iNumFrame = 0;
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%s %d"), pTemp, _countof(pTemp), &iNumFrame);

			std::vector<D3DXVECTOR4> LightData;
			LightData.resize(iNumFrame);

			for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
			{
				_fgetts(pBuffer, _countof(pBuffer), fp_src);
				_stscanf_s(pBuffer, _T("%s %f %f %f %f"), pTemp, _countof(pTemp),
					&LightData[iFrame].x, &LightData[iFrame].y,
					&LightData[iFrame].z, &LightData[iFrame].w);

			}

			m_LightInfoList[iCnt].vAmbinentLightColor = LightData[0];
			m_LightInfoList[iCnt].vDiffuseLightColor = LightData[1];
			m_LightInfoList[iCnt].vSpecularLightColor = LightData[2];
			m_LightInfoList[iCnt].vSpotLightColor = LightData[3];

			m_LightInfoList[iCnt].LightDir.x = LightData[4].x;
			m_LightInfoList[iCnt].LightDir.y = LightData[4].y;
			m_LightInfoList[iCnt].LightDir.z = LightData[4].z;

			m_LightInfoList[iCnt].At.x = LightData[5].x;
			m_LightInfoList[iCnt].At.y = LightData[5].y;
			m_LightInfoList[iCnt].At.z = LightData[5].z;

			m_LightInfoList[iCnt].LightPos.x = LightData[6].x;
			m_LightInfoList[iCnt].LightPos.y = LightData[6].y;
			m_LightInfoList[iCnt].LightPos.z = LightData[6].z;

			m_LightInfoList[iCnt].vSpotLight.x = cosf(D3DXToRadian(LightData[7].x));
			m_LightInfoList[iCnt].vSpotLight.y = cosf(D3DXToRadian(LightData[7].y));
			m_LightInfoList[iCnt].vSpotLight.z = LightData[7].z;
			m_LightInfoList[iCnt].vSpotLight.w = LightData[7].w;

			m_LightInfoList[iCnt].Fallof = LightData[8].x;
			m_LightInfoList[iCnt].Radius = LightData[8].y;
			m_LightInfoList[iCnt].Attenuation = LightData[8].z;
			m_LightInfoList[iCnt].iType = LightData[8].w;



		}
		fclose(fp_src);
		return iNumSprite;
	}

	void LightMgr::Create(const TCHAR* pszShaderFile, const TCHAR* pszLightInfoFile)
	{
		m_ShaderFileName = pszShaderFile;
		m_iLightNum = LightDataLoad(pszLightInfoFile);
		Init();
	}

	bool LightMgr::Init()
	{
		m_pLightConstantBuffer[0].Attach(CDXH::CreateConstantBuffer(m_pDevice, &m_cbLight, sizeof(LightConstantBuffer), 1));
		m_LightObjList.resize(m_iLightNum);

		for (int i = 0; i < m_iLightNum; i++)
		{
			m_LightObjList[i].CreateLightObj(m_pDevice, m_pContext, m_LightInfoList[i]);
			m_LightObjList[i].Create(m_pDevice, m_pContext, m_ShaderFileName, nullptr);
			m_cbLight.vAmbinentLightColor[i] = m_LightObjList[i].m_Info.vAmbinentLightColor;
			m_cbLight.vDiffuseLightColor[i] = m_LightObjList[i].m_Info.vDiffuseLightColor;
			m_cbLight.vSpecularLightColor[i] = m_LightObjList[i].m_Info.vSpecularLightColor;
			m_cbLight.vSpotLightColor[i] = m_LightObjList[i].m_Info.vSpotLightColor;
			m_cbLight.vLightDirection[i].x = m_LightObjList[i].m_Info.LightDir.x;
			m_cbLight.vLightDirection[i].y = m_LightObjList[i].m_Info.LightDir.y;
			m_cbLight.vLightDirection[i].z = m_LightObjList[i].m_Info.LightDir.z;
			m_cbLight.vLightDirection[i].w = m_LightObjList[i].m_Info.Attenuation;
			m_cbLight.vLightPos[i].x = m_LightObjList[i].m_Info.LightPos.x;//W:타입
			m_cbLight.vLightPos[i].y = m_LightObjList[i].m_Info.LightPos.y;
			m_cbLight.vLightPos[i].z = m_LightObjList[i].m_Info.LightPos.z;
			m_cbLight.vLightPos[i].w = m_LightObjList[i].m_Info.iType;
			m_cbLight.vSpotLight[i] = m_LightObjList[i].m_Info.vSpotLight;// 내부,외


		}
		return true;

	}

	bool LightMgr::Frame()
	{
		for (int i = 0; i < g_iLightNum; i++)
		{

			m_LightObjList[i].Frame();



			m_cbLight.vLightDirection[i] = { m_LightObjList[i].m_Info.LightDir, m_LightObjList[i].m_Info.Attenuation };

			m_cbLight.vLightPos[i] = { m_LightObjList[i].m_Info.LightPos ,(float)m_LightObjList[i].m_Info.iType };;

		}

		return true;
	}
	bool LightMgr::Render()
	{
		for (int i = 0; i < g_iLightNum; i++)
		{
			m_LightObjList[i].SetMatrix(nullptr, &m_pCamera->m_View,&m_pCamera->m_Proj);
			m_LightObjList[i].Render();
		}

		return true;
	}
	bool LightMgr::Release()
	{
		for (int i = 0; i < g_iLightNum; i++)
		{

			I_LIGHT_MGR.m_LightObjList[i].Release();
		}

		return true;
	}
	LightMgr::LightMgr()
	{
	}


	LightMgr::~LightMgr()
	{
	}
}