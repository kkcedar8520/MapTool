#pragma once
#include "CBY_Mesh.h"
namespace CBY
{
	class CBY_MeshSkin : public CBY_Mesh
	{
	public:
		std::vector<CBY_MeshSkin> subMeshSkin;
		std::vector<PNCTIW_VERTEX> listSkin;
		std::vector<PNCTIW_VERTEX> vblistSkin;
		std::vector<DWORD> iblistSkin;
		std::vector<CBY_MeshSkin*> m_Child;
		CBY_MeshSkin* m_Parent;
	public:
		CBY_MeshSkin();
		~CBY_MeshSkin();
	};
}
