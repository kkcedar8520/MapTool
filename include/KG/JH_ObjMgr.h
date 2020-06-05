#pragma once
#include"CBY_Object.h"
#include"JH_MapObj.h"
namespace JH {
	class JH_ObjMgr :public Singleton<JH_ObjMgr>
	{
		friend class Singleton<JH_ObjMgr>;
	public:
		std::map<int, std::shared_ptr<JH_MapObj>> m_ObjectList;
	public:
		void AddObject(std::shared_ptr<JH_MapObj> Obj);
	public:
		JH_ObjMgr();
		virtual ~JH_ObjMgr();
	};

#define I_ObjMgr JH_ObjMgr::GetInstance()
}