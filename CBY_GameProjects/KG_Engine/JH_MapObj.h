#pragma once
#include"JH_BaseObj.h"
#include"CBY_Object.h"
namespace JH {
	
	enum OBJ_FLAG
	{
		PATH=1,
		WALL,
	};
	class JH_MapObj 
	{
		T_STR	m_FileName;
		T_STR	m_SkinFileName;
		T_STR	m_BoneFileName;
		void*	m_pNode;
		DWORD	m_dwFlag;

		std::shared_ptr<CBY::CBY_Object> m_Object;
		int				m_ID;
		//int				m_iObjIndex;
		int				m_iQuadIndex;
	public:
		//int					GetObjIndex() { return m_iObjIndex; };
		//void				SetObjIndex(int Index) { m_iObjIndex = Index; };
		int					GetID() { return m_ID; }
		void				SetID(int Index) { m_ID = Index; }
		T_STR				GetName() { return m_FileName; }
		void				SetName(T_STR FileName) { m_FileName = FileName; }
		T_STR				GetSkinName() { return m_SkinFileName; }
		void				SetSkinName(const T_STR FileName) { m_SkinFileName = FileName; }
		T_STR				GetBoneName() { return m_BoneFileName; }
		void				SetBoneName(const T_STR FileName) { m_BoneFileName = FileName; }
		void*				GetNode() { return m_pNode; }
		void				SetNode(void* pNode) { m_pNode = pNode; }
		DWORD				GetFlag() { return m_dwFlag; }
		void				SetFlag(DWORD dwFlag) { m_dwFlag = dwFlag; }

		void				SetObj(const std::shared_ptr<CBY::CBY_Object> Obj) { m_Object = Obj; }
		std::shared_ptr<CBY::CBY_Object> GetObj() { return m_Object; }



		int					GetQuadIndex() { return m_iQuadIndex; }
		void				SetQuadIndex(int Index) { m_iQuadIndex = Index; }
		JH_MapObj();
		virtual ~JH_MapObj();
	};

}