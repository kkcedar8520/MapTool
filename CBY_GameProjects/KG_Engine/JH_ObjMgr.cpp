#include "JH_ObjMgr.h"

namespace JH {
	void JH_ObjMgr::AddObject(std::shared_ptr<JH_MapObj> Obj)
	{
		m_ObjectList.insert(std::make_pair(Obj->GetID(), Obj));
	}
	JH_ObjMgr::JH_ObjMgr()
	{
	}


	JH_ObjMgr::~JH_ObjMgr()
	{
		for(auto iter : m_ObjectList)
		{
			iter.second = nullptr;
		}
	}
}