#include "CBY_CharMgr.h"

namespace CBY
{
	int CBY_CharMgr::SkinLoad(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
	{

		std::wstring nameExt = pszLoad;

		std::map<int, CBY_SkinObj*>::iterator iter;
		for (iter = m_SkinLoadList.begin(); iter != m_SkinLoadList.end(); iter++)
		{
			if (iter->second->m_szname == nameExt)
			{
				return iter->first;
			}
		}

		std::wstring loadfile;
		loadfile = nameExt; //m_szPath + ;

		CBY_SkinObj* pData = new CBY_SkinObj;
		pData->m_szname = nameExt;

		if (pData->Load(loadfile.c_str(), pd3dDevice, Context) >= 0)
		{
			m_SkinLoadList.insert(std::make_pair(m_iSkinIndex, pData));
			return m_iSkinIndex++;
		}
		delete pData;
		return m_iSkinIndex++;
	}

	int CBY_CharMgr::BoneLoad(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
	{
		std::wstring nameExt = pszLoad;

		std::map<int, CBY_BoneObj*>::iterator iter;
		for (iter = m_BoneLoadList.begin(); iter != m_BoneLoadList.end(); iter++)
		{
			if (iter->second->m_szname == nameExt)
			{
				return iter->first;
			}
		}

		std::wstring loadfile;
		loadfile = m_szPath + nameExt;

		CBY_BoneObj* pData = new CBY_BoneObj;
		pData->m_szname = nameExt;

		if (pData->Load(loadfile.c_str(), pd3dDevice, Context) >= 0)
		{
			m_BoneLoadList.insert(std::make_pair(m_iBoneIndex, pData));
			return m_iBoneIndex++;
		}
		delete pData;
		return m_iBoneIndex++;
	}

	void CBY_CharMgr::SkinDelete(T_STR pszLoad)
	{
		std::wstring nameExt = pszLoad;
		std::map<int, CBY_SkinObj*>::iterator iter;
		for (iter = m_SkinLoadList.begin(); iter != m_SkinLoadList.end(); iter++)
		{
			if (iter->second->m_szname == nameExt)
			{
				m_SkinLoadList.erase(iter);
				return;
			}
		}
	}

	void CBY_CharMgr::BoneDelete(T_STR pszLoad)
	{
		std::wstring nameExt = pszLoad;
		std::map<int, CBY_BoneObj*>::iterator iter;
		for (iter = m_BoneLoadList.begin(); iter != m_BoneLoadList.end(); iter++)
		{
			if (iter->second->m_szname == nameExt)
			{
				m_BoneLoadList.erase(iter);
				return;
			}
		}
	}

	bool CBY_CharMgr::Release()
	{
		for (auto iter : m_SkinLoadList)
		{
			CBY_SkinObj* temp = iter.second;
			temp->Release();
			delete temp;
		}

		for (auto iter : m_BoneLoadList)
		{
			CBY_BoneObj* temp = iter.second;
			temp->Release();
			delete temp;
		}


		m_BoneLoadList.clear();
		m_SkinLoadList.clear();
		return true;
	}

	CBY_CharMgr::CBY_CharMgr()
	{
		//m_szPath = L"../../data/Char/";
		m_iSkinIndex = 0;
		m_iBoneIndex = 0;
	}


	CBY_CharMgr::~CBY_CharMgr()
	{
		Release();
	}
}