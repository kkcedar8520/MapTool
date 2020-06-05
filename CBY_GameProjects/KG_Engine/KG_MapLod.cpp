#include "KG_MapLod.h"
namespace JH {

	void  KG_MapLod::InitLevelOrder(KG_Node* pRootNode, int iMaxDepth)
	{
		m_LevelList.resize(iMaxDepth + 1);
		for (int iLevel = 1; iLevel < iMaxDepth + 1; iLevel++)
		{
			m_LevelList[iLevel].resize(pow(4.0f, iLevel));
		}
		m_LevelList[0].push_back(pRootNode);
	}

	KG_MapLod::KG_MapLod()
	{
	}


	KG_MapLod::~KG_MapLod()
	{
	}
}