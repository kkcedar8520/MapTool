#pragma once
#include "CBY_ObjStd.h"

namespace CBY
{
	class CBY_SkinParser
	{
	public:
		FILE*  m_pStream;
		TCHAR  m_pBuffer[256];
		TCHAR  m_pString[256];
		int    m_iTemp;
		CScene  m_Scene;
		std::vector<CBY_MeshSkin*> m_ObjList;
		std::vector<D3DXMATRIX> m_matBoneBindPoss;
		std::vector<C_Mtrl> m_MtlList;

	public:
		BOOL  OpenStream(const TCHAR* filename);
		BOOL  CloseStream();
		virtual bool  Load(const TCHAR* szFileName);
		int   GetParentIndex(C_STR szParentName);
		bool Release();

	public:
		CBY_SkinParser();
		virtual ~CBY_SkinParser();
	};
}

