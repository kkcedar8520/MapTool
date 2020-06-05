#pragma once
//#include "CBY_SkinObj.h"
#include "CBY_Obj.h"

namespace CBY
{
	class CBY_ObjectMgr :public Singleton<CBY_ObjectMgr>
	{
	public:
		std::map<int, CBY_Obj*> m_ObjLoadList;
		std::wstring	m_szPath;
		int				m_iObjIndex;
		HFONT   m_hDefaultFont;

	public:

		bool Release();
		int ObjLoad(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);

	public:
		void ObjDelete(T_STR pszLoad);

	public:
		CBY_ObjectMgr();
	public:
		~CBY_ObjectMgr();
	};

#define I_OBJECT CBY_ObjectMgr::GetInstance()
}
