#pragma once
#include "KG_Std.h"
#include"JH_Map.h"
#include"QuadTree.h"
#include"CBY_Object.h"
#include"KG_BackViewCamera.h"
#include"KG_DebugCamera.h"
#include"KG_Select.h"

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
		D3DXVECTOR3			m_CharPos;
		T_STR				m_pSplattAlphaTextureFile;
		std::vector<float>		m_fHegihtList;
		std::vector<T_STR>		m_pSplattTextureFile;

		QuadTreeData		m_sQTData;
		inline void Reset()
		{
			m_BaseTextureFile.clear();
			m_NormalMapFile.clear();
			m_ShaderFile.clear();
			m_pSplattAlphaTextureFile.clear();
			m_fHegihtList.clear();
			m_pSplattTextureFile.clear();
		}


	};



	struct GAME_MAP
	{
		std::shared_ptr<JH_Map> m_pMap;
		std::shared_ptr<HQuadTree> m_pQuadTree;
		std::map <int, std::shared_ptr<CBY::CBY_Object>> m_ObjectList;
	};
	class JH_MapMgr :public Singleton<JH_MapMgr>
	{

		friend class Singleton<JH_MapMgr>;
		ID3D11Device*						m_pd3dDevice;
		ID3D11DeviceContext*				m_pContext;
		std::vector<GAME_MAP>					m_GMDataList;
		GAME_MAP							m_CurrentMap;
		MAPDATA								m_MapData;
		KG_Camera*							m_pCamera;
		KG_Select							m_Select;
	public:
		int					m_iTemp;
		TCHAR				m_pBuffer[256];
		TCHAR				m_pString[256];
	public:
		INT	AddMap(const TCHAR* LoadFile);
		GAME_MAP& GetCurrentMap() { return m_CurrentMap; };
		bool CreateMap(JH_Map* pMap, HQuadTree* pQuad, int iWidth,
			int iHeight,
			int iCellCount,
			int iCellSize,
			const TCHAR* pTexturFileName,
			const TCHAR* pNormalMapFileName,
			const TCHAR* pHeightMapFileName = nullptr);
		INT AddObject(OBJECT Obj);
		void Set(ID3D11Device*, ID3D11DeviceContext*, KG_Camera*, KG_Select& select);
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