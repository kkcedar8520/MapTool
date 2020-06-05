#pragma once
#include"KG_Core.h"
#include"QuadTree.h"
#include"KG_ShapeBox.h"
#include"KG_Minimap.h"
#include"KG_BackViewCamera.h"
#include"KG_DebugCamera.h"
#include"MaxObj.h"
#include"KG_SkyBox.h"
#include"CBY_Object.h"
#include"KG_Std.h"
#include"JH_MapMgr.h"
#include"KG_Shadow.h"

#define CTL_CHARS		31
#define SINGLE_QUOTE	39 // ( ' )
#define ALMOST_ZERO 1.0e-4f


class main :public KG_Core
{

public:
	KG_BackViewCamera				m_BackViewCamera;
	KG_DebugCamera					m_DebugCamera;
	std::shared_ptr<JH::KG_SkyBox>			m_SkyBox;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader>			m_pCS;
public:
	JH::MAPDATA m_sMapData;
public:
	std::shared_ptr<JH::JH_Map>		m_Map;
	std::shared_ptr<JH::HQuadTree>	m_QuadTree;
	TCHAR		m_tmpBuffer[MAX_PATH];
public:
	//Load
	int					m_iTemp;
	TCHAR				m_pBuffer[256];
	TCHAR				m_pString[256];
	//Save
	const TCHAR*		m_pSPTAFile;
public:
	TCHAR* FixupName(T_STR name);
	//Load

	//OVERRIDE
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	main();
	virtual ~main();
};
KGCA37_RUN(main, xx, xx)