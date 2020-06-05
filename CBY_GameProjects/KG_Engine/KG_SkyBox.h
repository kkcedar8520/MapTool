#pragma once
#include"KG_ShapeBox.h"
namespace JH {
#define MAX_SKYBOX_TEXTURE 6
	class KG_SkyBox :public KG_ShapeBox
	{
	public:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_pTexSRV[MAX_SKYBOX_TEXTURE];
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_pCubeTexSRV;
		bool												m_bRenderType;
	public:
		bool	CreateSkyBox(ID3D11Device* pDevice, ID3D11DeviceContext*pContext, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTextureString = 0);
		bool	Frame();
		bool	Render();
		/*HRESULT LoadTextures(ID3D11Device* pDevice, const TCHAR* pLoadTextureString);*/
		HRESULT CreateTexutureArray(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
			const TCHAR** TextureArray);
		HRESULT CreateTexuture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
			const TCHAR* Texture);
		/*HRESULT LoadTextureArray(ID3D11Device* pd3dDevice, ID3D11DeviceContext*    pContext, TCHAR** szTextureNames, int iNumTextures,
			ID3D11Texture2D** ppTex2D, ID3D11ShaderResourceView** ppSRV);*/
	public:


	public:
		KG_SkyBox();
		virtual ~KG_SkyBox();
	};

}