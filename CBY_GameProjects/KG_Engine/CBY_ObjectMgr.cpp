#include "CBY_ObjectMgr.h"

namespace CBY
{
	int CBY_ObjectMgr::ObjLoad(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
	{
		std::wstring nameExt = pszLoad;

		std::map<int, CBY_Obj*>::iterator iter;
		for (iter = m_ObjLoadList.begin(); iter != m_ObjLoadList.end(); iter++)
		{
			if (iter->second->m_szname == nameExt)
			{
				return iter->first;
			}
		}

		std::wstring loadfile;
		loadfile = nameExt; //m_szPath + ;

		CBY_Obj* pData = new CBY_Obj;
		pData->m_szname = nameExt;

		if (pData->Load(loadfile.c_str(), pd3dDevice, Context) >= 0)
		{
			m_ObjLoadList.insert(std::make_pair(++m_iObjIndex, pData));
			return m_iObjIndex;
		}
		delete pData;
		return m_iObjIndex;
	}

	void CBY_ObjectMgr::ObjDelete(T_STR pszLoad)
	{
		std::wstring nameExt = pszLoad;
		std::map<int, CBY_Obj*>::iterator iter;
		for (iter = m_ObjLoadList.begin(); iter != m_ObjLoadList.end(); iter++)
		{
			if (iter->second->m_szname == nameExt)
			{
				m_ObjLoadList.erase(iter);
				return;
			}
		}
	}

	bool CBY_ObjectMgr::Release()
	{
		for (auto iter : m_ObjLoadList)
		{
			CBY_Obj* temp = iter.second;
			temp->Release();
			delete temp;
		}

		m_ObjLoadList.clear();
		return true;
	}

	CBY_ObjectMgr::CBY_ObjectMgr()
	{
		//m_szPath = L"../../data/Char/";
		m_iObjIndex = 0;
	}


	CBY_ObjectMgr::~CBY_ObjectMgr()
	{
		Release();
	}
}
