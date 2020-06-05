#pragma once
#include "CBY_Parser.h"

namespace CBY
{
	class CBY_Obj : public KG_Model
	{
	private:
		CBY_Parser m_Loader;

	public:
		CScene m_Scene;
		std::vector<int> m_iRootList;
		std::vector<CBY_MeshSkin*> m_ObjectList;
		float m_fElapseTick;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBConstBoneWorld;
		std::vector<PNCTIW_VERTEX>	m_VertexDataSkin;
		std::vector<D3DXMATRIX> m_matBoneBindPoss;
		std::wstring m_szname;

	public:
		virtual bool Load(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
		virtual void Convert(std::vector<PNCTIW_VERTEX>& list);
		virtual bool AniTrackSet(CMatSetData& matdata, CAnimationTrack start, int iobj, std::vector<CAnimationTrack>& AniTrack, int tracktype);
		virtual void CreateVIData(CBY_MeshSkin* mesh);
		virtual void Draw(CBY_MeshSkin* mesh);
		virtual bool PreRender();
		virtual void ObjUpdate(D3DMATRIX* parmat, D3DMATRIX& matcal);

	public:
		virtual HRESULT ObjCreate();
		virtual HRESULT CreateVertexData() override;
		virtual HRESULT CreateIndexData() override;
		virtual HRESULT CreateVertexBuffer() override;
		virtual HRESULT CreateConstantBuffer() override;

	public:
		virtual bool Render() override;
		virtual bool Frame() override;
		virtual bool Release() override;

	public:
		CBY_Obj();
		virtual ~CBY_Obj();
	};
}


