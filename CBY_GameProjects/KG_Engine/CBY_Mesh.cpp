#include "CBY_Mesh.h"


namespace CBY
{
	CBY_Mesh::CBY_Mesh()
	{
		m_iTexsize = 0;
		m_bBone = false;
		m_bRoot = false;
		m_iParentIndex = -1;
		m_iTexIndex = -1;
		m_iBaseVertex = 0;
		m_iNumVertex = 0;
		m_iBaseIndex = 0;
		m_iNumIndex = 0;
		m_pTexture = nullptr;
		m_iType = GEOM_MESH;
		ZeroMemory(&m_cbData, sizeof(CONDATA));
	}


	CBY_Mesh::~CBY_Mesh()
	{
		m_BoneBox.Release();
	}
}