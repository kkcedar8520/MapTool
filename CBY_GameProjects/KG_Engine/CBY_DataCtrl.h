#pragma once
#include "CBY_BoneObj.h"
#include "CBY_SkinObj.h"
#include "CBY_Object.h"
#include "KG_Core.h"

namespace CBY
{
	struct ObjInfo;

	struct MTRInfo
	{
		T_STR MtrName;
		int State;
		inline MTRInfo()
		{
			State = -2;
		}
	};

	struct cby_CharData
	{
		DWORD m_CharType;
		std::vector<T_STR> SknNameList;
		std::vector<MTRInfo> MtrList;
		DWORD dwStateNum;
		std::vector<Char_State_Info> StateList;
		CBY_CharBox CharBox;
		std::vector<CBY_CharBox> ColBoxList;

		std::vector<ObjInfo> ObjList;
	};

	struct ObjInfo
	{
		cby_CharData objdata;
		int Socket;
		int ObjSocket;
		int FireSocket;
		D3DXVECTOR3 vpos;
	};

	class CBY_DataCtrl
	{
	public:
		bool Save(const TCHAR* pszSave);
		bool Load(const TCHAR* pszName);
		TCHAR* SaveFileDlg(TCHAR* szExt, TCHAR* szTitle);

	public:
		cby_CharData m_data;

	public:
		CBY_DataCtrl();
		virtual ~CBY_DataCtrl();
	};
}

