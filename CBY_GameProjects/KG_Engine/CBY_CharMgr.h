#pragma once
#include "CBY_SkinObj.h"
#include "CBY_BoneObj.h"

namespace CBY
{
	class CBY_CharMgr :public Singleton<CBY_CharMgr>
	{
	public:
		std::map<int, CBY_SkinObj*> m_SkinLoadList;
		std::map<int, CBY_BoneObj*> m_BoneLoadList;
		std::wstring	m_szPath;
		int				m_iSkinIndex;
		int				m_iBoneIndex;
		HFONT   m_hDefaultFont;

	public:

		bool Release();
		int SkinLoad(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
		int BoneLoad(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);

	public:
		void SkinDelete(T_STR pszLoad);
		void BoneDelete(T_STR pszLoad);

	public:
		CBY_CharMgr();
	public:
		~CBY_CharMgr();
	};

#define I_CHARACTER CBY_CharMgr::GetInstance()
}

