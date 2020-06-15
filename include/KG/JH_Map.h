#pragma once

#include"KG_ShapeMap.h"
#include"KG_ShapeLine.h"
#include"KG_TextureMgr.h"
#include"NormalMap.h"
#include"KG_SkyBox.h"
#include"LightMgr.h"
#include"KG_Camera.h"
namespace JH
{


	struct CB_SPT
	{
		D3DXVECTOR4 MapSubData;//x:splattTexture Num
		CB_SPT()
		{
			MapSubData = D3DXVECTOR4(0, 0, 0, 0);
		}
	};
	struct SplattTextureDesc
	{
		int TexNum;
		float Alpha;
	};
	struct MapDesc
	{
		int iColumNum;
		int iRowNum;
		const TCHAR* TextureFileName;
		const TCHAR* ShaderFileName;
		const TCHAR* pNormMapFileName;
		const TCHAR* pHeightFileName;
		float fSellDistance;
		float fScaleHeight;
		//텍스트파일
		//쉐이더 
		//셀 간격
		//행
		//열
		//정점 갯 수

	};

	class JH_Map :public KG_Model
	{
	public:
		bool	m_bMapEdit;

		std::shared_ptr<KG_SkyBox>					 m_SkyBox;
		std::map<int, CTexture*>						 m_vSplattTextureList;
		std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_vSplattSRVList;
		//
		CB_SPT m_CBSubData;
		Microsoft::WRL::ComPtr<ID3D11Buffer>					 m_CBSub;
	public:
		//NormalMap
		std::vector<D3DXVECTOR3>						 m_TangentList;
		NormalMap								 m_NormalMap;
		D3DXMATRIX								 m_matNormal;

		Microsoft::WRL::ComPtr<ID3D11Buffer>					 m_pTangentVB;
		CTexture*								 m_pTexture;
		int										 m_iTexNum;
		const TCHAR*							 m_pNormMapFileName;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		 m_pNormSrv;


	public:
		D3DXVECTOR3 m_vEyePos;
		MapDesc m_MapDesc;
		int m_iColumNum;
		int m_iRowNum;
		int m_iVertices;
		int m_iCellCol;
		int m_iCellRow;
		int m_iCellCount;
		const TCHAR* m_TextureFileName;
		const TCHAR* m_ShaderFileName;
		const TCHAR* m_HegithFileName;
		float m_fCellDistance;
		float m_fScaleHeight;

		D3DXVECTOR3 m_CharPos;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_pCopySrv;
	public:
		std::vector<float> m_vHeightList;
	public:
		float GetHeight(float fX, float fZ);
		float GetHeightMap(int iRow, int iCol);
		HRESULT Load(ID3D11Device* pD3D11Device, ID3D11DeviceContext* pD3D11DeviceContext);
		HRESULT CreateHeightMap(ID3D11Device* pD3D11Device, ID3D11DeviceContext* pD3D11DeviceContext, const TCHAR* TextureFileName);
		void SetMapDesc(const TCHAR* TextureFileName,
			const TCHAR* ShaderFileName,
			int ColumNum,
			int RowNum,
			float fSellDistance,
			float fScaleHeight);
		void    SetMatrix(D3DXMATRIX* matWorld,
			D3DXMATRIX* matView,
			D3DXMATRIX* matProj)override;
		void UpdateConstantBuffer(ID3D11Buffer* pConstantBuffer, void* Data);
		float Lerp(float fStart, float fEnd, float fTangent);
		//노말맵 접선 벡터 계산

	//툴 관련 함수
		INT AddSplattTexture(const TCHAR* pFileName, int Num, float Alpha = 0.0f);
		D3DXVECTOR3 GetCharPos(){ return m_CharPos; }
		void		SetCharPos(D3DXVECTOR3 vPos) { m_CharPos = vPos; }
	public:
		bool RenderSet(KG_Camera* Camera);
	public:
		HRESULT CreateInputLayout()override;
		HRESULT	LoadTexture(const TCHAR* pszTexFileName)override;
		bool	UpdateTangentBuffer();
		bool Frame()override;
		bool Release()override;

	public:

		HRESULT		CreateVertexData();
		HRESULT		CreateIndexData();
	public:
		JH_Map();
		virtual ~JH_Map();
	};
}