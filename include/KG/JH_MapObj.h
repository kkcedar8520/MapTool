#pragma once
#include"JH_BaseObj.h"
namespace JH {
	class JH_MapObj :public JH_BaseObj
	{
		T_STR	m_FileName;
		T_STR	m_SkinFileName;
		T_STR	m_BoneFileName;
		int				m_ID;
		//int				m_iObjIndex;
		int				m_iQuadIndex;
	public:
		//int					GetObjIndex() { return m_iObjIndex; };
		//void				SetObjIndex(int Index) { m_iObjIndex = Index; };
		int					GetID() { return m_ID; };
		void				SetID(int Index) { m_ID = Index; };
		T_STR				GetName() { return m_FileName; };
		void				SetName(T_STR FileName) { m_FileName = FileName; };
		T_STR				GetSkinName() { return m_SkinFileName; };
		void				SetSkinName(const T_STR FileName) { m_SkinFileName = FileName; };
		T_STR				GetBoneName() { return m_BoneFileName; };
		void				SetBoneName(const T_STR FileName) { m_BoneFileName = FileName; };

		int					GetQuadIndex() { return m_iQuadIndex; };
		void				SetQuadIndex(int Index) { m_iQuadIndex = Index; };
		JH_MapObj();
		virtual ~JH_MapObj();
	};

}