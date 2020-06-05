#pragma once
#include "KG_Model.h"
#include "CBY_BoneObj.h"
#include "CBY_SkinObj.h"

namespace CBY
{
	enum CBY_CHARDATATYPE
	{
		CHAR_FRAMETYPE = 0,
		CHAR_MTRTYPE
	};

	struct Char_State_Info
	{
		int m_iStartFrame;
		int m_iEndFrame;

		CBY_BoneObj m_Bone;
		inline Char_State_Info()
		{
			m_iStartFrame = 0;
			m_iEndFrame = 0;
		}
	};


	class CBY_Object :public KG_Model
	{
	protected:
		DWORD m_dwAniType;
		int m_iRoot;
		float m_fElapseTick;
		DWORD m_dwState;
		std::vector<Char_State_Info> m_StateList;
		CBY_CharBox m_CharBox;
		KG_Box m_SkinOriginalBox;
		std::vector<CBY_CharBox> m_BoxList;

	public:
		float m_iStartFrame;
		float m_iEndFrame;

	public:
		CScene m_Scene;
		D3DXMATRIX m_pMatrixList[255];
		CBConstBoneWorld m_cbBoneWorld;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBConstBoneWorld;
		ID3D11ShaderResourceView* m_pTextView;


	public:
		std::vector<std::shared_ptr<CBY_SkinObj>> m_ObjList;
		std::shared_ptr<CBY_BoneObj> m_BindBone;
		CBY_BoneObj* m_Bone;
		std::vector<int> m_SkinObjIndex;
		int m_BoneIndex;

	public:
		virtual bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context, const TCHAR* ShaderFileName,
			const TCHAR* TexFileName, const CHAR* VSName, const CHAR* PSName) override;
		virtual HRESULT CreateConstantBuffer() override;
		virtual HRESULT CreateInputLayout() override;
		virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj) override;
		virtual bool PreRender();
		virtual void CharPostRender();
		virtual void BoneBoxRender(CBY_MeshSkin* obj, int index);	//캐릭터의 뼈대 출력

	public:
		virtual bool SkinLoad(T_STR pszLoad);
		virtual bool BoneLoad(T_STR pszLoad);
		virtual void SetStateNum(int statenum);
		virtual void SetFrameTime(int m_dwState, float start, float end);
		virtual void SetMTRApply(T_STR pszLoad, int state);		//캐릭터 상태설정

	public:
		virtual bool Init();
		virtual bool Frame();
		virtual bool Release();
		virtual bool Render();

	public:
		virtual void SetState(int dw);							//캐릭터 상태 설정
		virtual int GetStateNum();								//캐릭터의 상태번호 받아오기
		virtual void SetCharRootBone(int ibone);				//캐릭터의 위치정보를 가질 루트오브젝트 설정
		virtual D3DXVECTOR3 GetCharPos();						//캐릭터의 위치 받기


	public:				//Box
		virtual void SetCharBox();
		virtual KG_Box GetCharBox();
		virtual void SetColBoxList(std::vector<CBY_CharBox>& boxlist);
		virtual CBY_CharBox SetColBoxList(int ibox);
		virtual int GetColBoxSize();
		virtual void CreateColBox();

	public:
		CBY_Object();
		virtual ~CBY_Object();
	};
}

