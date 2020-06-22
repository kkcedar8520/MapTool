#pragma once
#include"LightObj.h"
#include"KG_Camera.h"
namespace JH 
{
	struct LightConstantBuffer
	{
		D3DXVECTOR4 vAmbinentLightColor[g_iLightNum];
		D3DXVECTOR4 vDiffuseLightColor[g_iLightNum];
		D3DXVECTOR4 vSpecularLightColor[g_iLightNum];
		D3DXVECTOR4 vSpotLightColor[g_iLightNum];
		D3DXVECTOR4 vLightDirection[g_iLightNum];//W :∞®ºË
		D3DXVECTOR4 vLightPos[g_iLightNum];//W:≈∏¿‘
		D3DXVECTOR4 vEyeDir[g_iLightNum];
		D3DXVECTOR4 vEyePos[g_iLightNum];// ≥ª∫Œ,ø‹∫Œ,»÷µµ theta, phi, Fallofff
		D3DXVECTOR4 vSpotLight[g_iLightNum];
		D3DXMATRIX  matLTM[g_iLightNum];
	};

	class LightMgr
	{
	public:
		const TCHAR*			m_ShaderFileName;
		std::vector<LightObj>	m_LightObjList;
		std::vector<LightInfo>	m_LightInfoList;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pLightConstantBuffer[CONSTANT_BUFFER_SIZE];
		LightConstantBuffer m_cbLight;
		int m_iLightNum;
		ID3D11Device*			m_pDevice;
		ID3D11DeviceContext*	m_pContext;
		KG_Camera*				m_pCamera;

	public:
		static LightMgr& Get()
		{
			static LightMgr mgr;
			return mgr;
		}
	public:
		int LightDataLoad(const TCHAR* pszLoad);
		void Create(const TCHAR* pszShaderFile, const TCHAR* pszLightInfoFile);
		void SetCamera(KG_Camera* pCamera);
		void GetDevice(ID3D11Device* pdevcie);
		void GetContext(ID3D11DeviceContext* pContext);
	public:
		virtual bool Init();
		virtual bool Frame();
		virtual bool Render();
		virtual bool Release();

		virtual ~LightMgr();
	private:
		LightMgr();
	};


#define I_LIGHT_MGR LightMgr::Get()
}
