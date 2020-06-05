#include "KG_Node.h"


namespace JH {
	KG_Node::KG_Node()
	{
	}


	KG_Node::~KG_Node()
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			//	SAFE_DEL(m_pChild[iNode]);
		}
	}
}