#pragma once
#include"KG_Select.h"

namespace JH
{
	struct SPHERE
	{
		D3DXVECTOR3 vCenter;
		float	Radius;

	};
	class JH_BaseObj
	{
	public:
		KG_Box m_Box;
		//SPHERE		m_Sphere;
	public:
		D3DXMATRIX m_matWorld;
	public:
		JH_BaseObj();
		virtual ~JH_BaseObj();
	};

}