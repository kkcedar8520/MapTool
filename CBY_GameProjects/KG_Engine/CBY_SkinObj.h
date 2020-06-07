#pragma once
#include "CBY_SkinParser.h"
#include "CBY_BoneObj.h"

namespace CBY
{
#define MAX_BONE_MATRICES 255
	struct CBConstBoneWorld
	{
		D3DXMATRIX g_matConstBoneWorld[MAX_BONE_MATRICES];
	};

	class CBY_SkinObj : public KG_Model
	{
	public:
		CBY_BoneObj m_Bone;
		CScene m_Scene;
		CBY_SkinParser m_Loader;
		float m_fElapseTick;
		CBConstBoneWorld m_cbBoneWorld;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBConstBoneWorld;
		D3DXMATRIX m_pMatrixList[MAX_BONE_MATRICES];
		std::vector<CBY_MeshSkin*> m_ObjList;
		std::vector<D3DXMATRIX> m_matBoneBindPoss;
		std::vector<PNCT2IW_VERTEX>	m_VertexDataSkin;
		float m_fMove;
		std::wstring m_szname;


	public:
		bool Load(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
		void ConvertSkin(std::vector<PNCT2IW_VERTEX>& list);
		void CreateVIData(CBY_MeshSkin* mesh);
		void Draw(CBY_MeshSkin* mesh);
		bool SkinCreate();
		void PrePender();
		void VertexDataTan(CBY_MeshSkin* mesh);
		void CreateTangentSpaceVectors(D3DXVECTOR3 *v0,
			D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR2 uv0,
			D3DXVECTOR2 uv1, D3DXVECTOR2 uv2,
			D3DXVECTOR3 *vTangent, D3DXVECTOR3 *vBiNormal, D3DXVECTOR3 *vNormal);

	public:
		HRESULT CreateVertexData() override;
		HRESULT CreateIndexData() override;
		HRESULT CreateVertexBuffer() override;

	public:
		bool Render() override;
		bool Release() override;

	public:
		CBY_SkinObj();
		virtual ~CBY_SkinObj();
	};
}

