#pragma once
#include "KG_Std.h"
#include"JH_Map.h"
#include"QuadTree.h"
#include"CBY_Object.h"
#include"KG_BackViewCamera.h"
#include"KG_DebugCamera.h"
#include"KG_Select.h"
#define CTL_CHARS		31
#define SINGLE_QUOTE	39 // ( ' )
#define ALMOST_ZERO 1.0e-4f

namespace JH {
	struct MAP_OBJ_DATA
	{
		int			m_iQuadTreeIndex;
		T_STR		m_BoneName;
		T_STR		m_SkinName;
		D3DXMATRIX	m_matWorld;
		DWORD		m_Flag;
		KG_Box		m_Box;

		inline MAP_OBJ_DATA()
		{
			D3DXMatrixIdentity(&m_matWorld);
		}
	};

	struct OBJECT
	{
		std::shared_ptr<MAP_OBJ_DATA>		m_MapObj;
		inline OBJECT()
		{
		}
	};
	struct QuadTreeData
	{

		std::vector<OBJECT> m_ObjList;
		inline QuadTreeData()
		{

		}
	};
	struct MAPDATA
	{
		int					iCol;
		int					iRow;
		int					iCellCount;
		int					iCellSize;
		T_STR				m_BaseTextureFile;
		T_STR				m_NormalMapFile;
		T_STR				m_HeightMapFile;
		T_STR				m_ShaderFile;
		T_STR				m_LightFile;
		D3DXVECTOR3			m_CharPos;
		T_STR				m_pSplattAlphaTextureFile;
		std::vector<float>				m_fHegihtList;
		std::vector<PNCT_VERTEX>		m_VerTex;
		std::vector<DWORD>				m_IndexData;
		std::vector<T_STR>				m_pSplattTextureFile;

		QuadTreeData		m_sQTData;
		inline void Reset()
		{
			m_BaseTextureFile.clear();
			m_NormalMapFile.clear();
			m_ShaderFile.clear();
			m_pSplattAlphaTextureFile.clear();
			m_fHegihtList.clear();
			m_pSplattTextureFile.clear();

			m_sQTData.m_ObjList.clear();
		}


	};



	struct GAME_MAP
	{
		T_STR MapFileName;
		std::shared_ptr<JH_Map> m_pMap;
		std::shared_ptr<HQuadTree> m_pQuadTree;
		std::map <int, std::shared_ptr<CBY::CBY_Object>> m_ObjectList;
	};
	class JH_MapMgr :public Singleton<JH_MapMgr>
	{

		friend class Singleton<JH_MapMgr>;
		ID3D11Device*											m_pd3dDevice;
		ID3D11DeviceContext*									m_pContext;
		std::vector<std::shared_ptr<GAME_MAP>>					m_GMDataList;
		std::shared_ptr<GAME_MAP>								m_CurrentMap;
		MAPDATA													m_MapData;
		KG_Camera*												m_pCamera;
		KG_Select*												m_pSelect;
	public:
		int					m_ObjID;
		int					m_iTemp;
		TCHAR				m_pBuffer[256];
		TCHAR				m_pString[256];
		TCHAR		m_tmpBuffer[MAX_PATH];
	public:
			INT	AddMap(const TCHAR* LoadFile);
			INT AddGameMap(std::shared_ptr<JH_Map> pMap, std::shared_ptr<HQuadTree> pQuadTree);
	inline	void SetCurrentMap(std::shared_ptr<GAME_MAP> pMap) {  m_CurrentMap= pMap; };
	inline	std::shared_ptr<GAME_MAP> GetCurrentMap() { return m_CurrentMap; };
	inline	void SetMap(std::shared_ptr<JH_Map> pMap) {  m_CurrentMap->m_pMap= pMap; }
	inline	void SetCurrentQuadTree(std::shared_ptr< HQuadTree> pQuadTree) { m_CurrentMap->m_pQuadTree= pQuadTree; }
	inline	std::shared_ptr<JH_Map> GetMap() { return m_CurrentMap->m_pMap; }
	inline	std::shared_ptr<HQuadTree> GetCurrentQuadTree() { return m_CurrentMap->m_pQuadTree; }

		TCHAR* FixupName(T_STR name);
		bool CreateMap(JH_Map* pMap, HQuadTree* pQuad, int iWidth,
			int iHeight,
			int iCellCount,
			int iCellSize,
			const TCHAR* pTexturFileName,
			const TCHAR* pNormalMapFileName,
			const TCHAR* pHeightMapFileName = nullptr);
		bool LoadMap(JH_Map* pMap, HQuadTree* pQuad, int iWidth,
			int iHeight,
			int iCellCount,
			int iCellSize,
			const TCHAR* pTexturFileName,
			const TCHAR* pShaderFileName,
			const TCHAR* pLightShaderName,
			const TCHAR* pNormalMapFileName,
			const TCHAR* pHeightMapFileName = nullptr);
		INT AddObject(OBJECT Obj);
		void Set(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, KG_Camera* pCamera, KG_Select* select);
		bool SaveMapData(const TCHAR* LoadFile);

		void SaveToQuadTree(KG_Node* pNode, FILE* fp);
		void LoadToQuadTree(KG_Node* pNode, FILE* fp);
	public:
		//CTexture* const	GetPtr(INT iIndex);
		//CTexture* const GetPtr(T_STR strFindName);
		bool Release();
	public:
		JH_MapMgr();
		virtual ~JH_MapMgr();
	};



#define I_MapMgr JH_MapMgr::GetInstance()

}