#include "CBY_MeshSkin.h"


namespace CBY
{
	CBY_MeshSkin::CBY_MeshSkin()
	{
		m_Parent = nullptr;
	}


	CBY_MeshSkin::~CBY_MeshSkin()
	{
		m_BoneBox.Release();
	}
}
