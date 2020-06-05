#include "JH_BaseObj.h"

namespace JH
{
	JH_BaseObj::JH_BaseObj()
	{
		D3DXMatrixIdentity(&m_matWorld);
	}


	JH_BaseObj::~JH_BaseObj()
	{
	}
}