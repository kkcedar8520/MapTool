#pragma once
#include"KG_Std.h"
#include"JH_MapObj.h"

namespace JH {
	class KG_Node
	{
	public:
		KG_Box m_Box;
		KG_Plane											m_BoxPlane[4];
		std::vector<int>									m_ObjListIndexList;
		std::vector<KG_Node*>								m_NeighborNodeList;
		std::map<int, std::shared_ptr<JH_MapObj >>			m_ObjList;
		BOOL												m_isLeaf;
		DWORD												m_dwDepth;
		DWORD												m_dwCorner[4];// 꼭지점 네점
		int													m_iQuadTreeIndex;
		KG_Node*											m_pChild[4];
		std::vector<DWORD>									m_IndexList;

		//이웃노드를 얻기위한 위치정보
		DWORD							m_dwPositionIndex[2];

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	public:
		KG_Node();
		virtual ~KG_Node();


	};
}