#pragma once
#include "CBY_DataCtrl.h"
#include "CBY_Weapon.h"

namespace CBY
{
	class CBY_Character :public CBY_Object
	{
	protected:
		std::vector<std::shared_ptr<CBY_Weapon>> m_ObjectList;
		CBY_DataCtrl m_CharData;

	public:
		bool Init() override;
		bool Frame() override;
		bool Render() override;
		bool Release() override;
		virtual bool NonCharacterRender();
		virtual bool SkinLoad(T_STR pszLoad) override;

	public: //object
		virtual bool ObjLoad(T_STR pszSkinLoad);
		virtual void SetObjectSocket(int iSocket, int iObjSocket, int iFireSocket, int iobj);

	public:
		virtual bool CharacterLoad(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context ,T_STR pszLoad,
			const TCHAR* ShaderFileName= L"../../data/shader/SkinShader.txt", const TCHAR* TexFileName= nullptr, const CHAR* VSName= "VSSKIN", const CHAR* PSName= "PS");
		virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)override;
		
	public:
		D3DXVECTOR3 GetColPos(int i);
		D3DXVECTOR3 GetFirePos(int iObj);
		inline KG_Box GetColBox(int i)
		{
			return m_BoxList[i].GetBox();
		}

		inline bool GetStateAnimationStart(int i)
		{
			return m_StateList[i].m_Bone.GetAniStart();
		}
		inline bool GetStateAnimationEnd(int i)
		{
			return m_StateList[i].m_Bone.GetAniEnd();
		}

	public:
		virtual void SetCharAction(int state, float start, float end);
		inline virtual void SetEndClip(int iState, bool bclip) { m_StateList[iState].m_Bone.SetEndClip(bclip); }

	public:
		CBY_Character();
		virtual ~CBY_Character();
	};
}
