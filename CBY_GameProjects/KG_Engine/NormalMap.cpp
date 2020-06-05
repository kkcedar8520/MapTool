#include "NormalMap.h"
namespace JH {
	void NormalMap::CreateTangentSpaceVectors(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR3 *v3,
		float v1u, float v1v,
		float v2u, float v2v,
		float v3u, float v3v,
		D3DXVECTOR3 *vTangent)
	{
		D3DXVECTOR3 vEdge1 = *v2 - *v1;
		D3DXVECTOR3 vEdge2 = *v3 - *v1;

		float fEdge1U = v2u - v1u;//s1
		float fEdge1V = v2v - v1v;//t1

		float fEdge2U = v3u - v1u;//s2
		float fEdge2V = v3v - v1v;//t2


		float fDenominator = fEdge1U * fEdge2V - fEdge1V * fEdge2U;
		float fScale = 1.0f / fDenominator;

		D3DXVECTOR3 T;//t2 -t1   (vEdge1.x,y,z)
					  //-s2  s1	(vEdge2.x,y,z)
		T.x = fScale * (fEdge2V*vEdge1.x + (-fEdge1V * vEdge2.x));
		T.y = fEdge2V * vEdge1.y + (-fEdge1V * vEdge2.y);
		T.z = fEdge2V * vEdge1.z + (-fEdge1V * vEdge2.z);

		D3DXVec3Normalize(&T, &T);
		vTangent = &T;


	}
}