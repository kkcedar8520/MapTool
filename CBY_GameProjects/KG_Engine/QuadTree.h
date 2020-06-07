#pragma once
#include"JH_Map.h"
#include"KG_Camera.h"
#include"KG_Select.h"
#include"KG_ShapeLine.h"
#include"KG_Node.h"
#include"KG_MapLod.h"

namespace JH {


	class HQuadTree :public KG_MapLod
	{
	public:
		bool										m_bFind;
		float										m_fTimer;
		KG_Node*									m_pRootNode;
		KG_Node*									m_pFindNode;
		JH_Map*										m_pMap;
		KG_Camera*									m_pCamera;
		std::vector<KG_Node*>							m_SelectNodeList;
		std::vector< std::shared_ptr<JH_MapObj>>	m_SelectObjList;

		KG_Node*									m_pNearPointNode;
		float										m_fdistance;
		KG_Select*									m_pSelect;
		int											m_iQuadTreeIndex;
		std::vector<KG_Node*>							m_ContainObjNode;

		std::vector<KG_Node*>							m_DrawNodeList;
		std::vector<KG_Node*>							m_DrawObjNodeList;

		DWORD										m_dwWidth;
		DWORD										m_dwHeight;
		int											m_iNumFace;

		std::shared_ptr<KG_ShapeLine>	m_BoxLine;
	public:
		BOOL Build(JH_Map* pMap, KG_Camera* pCamera);
		BOOL BuildTree(KG_Node* pNode);
		void FindDrawNode(KG_Node* pNode);
		void VisibleNode(KG_Node* pNode);
		BOOL DivideNode(KG_Node*pNode);
		KG_Box SetBB(JH_MapObj* Obj);

		KG_Node* CreateNode(KG_Node* pParent, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
		void GetSelectNode(KG_Node* pNode);
		void GetSelectObj(KG_Node* pNode);
		bool GetObjectAddNode(std::shared_ptr<JH_MapObj> Obj);
		bool GetCotainObjNode(KG_Node* pNode);
		bool ChangeObjectNode(std::shared_ptr<JH_MapObj> Obj);
		KG_Node* FindNode(KG_Node* pNode, JH_MapObj* Obj);
		void FindObjectNode(KG_Node* pNode, std::shared_ptr<JH_MapObj> Obj);
		void SetNeighborNode(KG_Node* pNode);
		void FindNeighborNode(KG_Node* pNode);

		bool CheckRect(KG_Node* pNode, JH_MapObj* Obj);
		void CreateIndexList(KG_Node* pNode);
		void CreateBB(KG_Node* pNode);
		void ChangeBB(KG_Node* pNode, KG_Box Box);
		void GetMinMax(KG_Box& Obj);
		JH_MapObj* UpdateBB(KG_Node* pNode, JH_MapObj* Obj);
		
		void DirectSelect(KG_Node* pNode);
		void Set();
		bool Frame();
		bool Render();
		bool Release();
		bool Draw(KG_Node* pNode, ID3D11DeviceContext* pContext);
		void DrawNodeLine(KG_Node* pNode);
		void DrawLine(KG_Node* pNode);
	public:
		bool FindInterSection();
		void FindNearInterSectionNode(KG_Node* pNode, D3DXVECTOR3 vInterSection);
	public:
		HQuadTree();
		virtual ~HQuadTree();
	};

}