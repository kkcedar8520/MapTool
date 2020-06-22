#pragma once
#include"KG_Core.h"
#include"QuadTree.h"
#include"KG_ShapeBox.h"
#include"KG_Minimap.h"
#include"KG_BackViewCamera.h"
#include"KG_DebugCamera.h"
#include"KG_ModelViewCamera.h"
#include"KG_SkyBox.h"
#include "CBY_Object.h"
#include"KG_Shadow.h"
//#include"KG_ObjStd.h"
#define CTL_CHARS		31
#define SINGLE_QUOTE	39 // ( ' )
#define ALMOST_ZERO 1.0e-4f
enum TOOL_EVENT
{
	NORMAL = 0,
	HEIGHT_EDIT,
	FLATTING,
	SPLATTING,
	ADDOBJECT,
	SELECT,
	ROTATION,
	MOVE,
	SCALE,
	SETTING,
	TRANSLATION,
	
	
};
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
	int			iCol;
	int			iRow;
	int			iCellCount;
	int			iCellSize;
	T_STR		m_BaseTextureFile;
	T_STR		m_NormalMapFile;
	T_STR		m_HeightMapFile;
	T_STR		m_ShaderFile;
	D3DXVECTOR3 m_CharPos;

	T_STR						m_pSplattAlphaTextureFile;
	std::vector<float>			m_fHegihtList;
	std::vector<PNCT_VERTEX>	m_VerTex;
	std::vector<T_STR>			m_pSplattTextureFile;

	QuadTreeData		m_sQTData;

	void Reset()
	{
		m_pSplattAlphaTextureFile.clear();
		m_fHegihtList.clear();
		m_pSplattTextureFile.clear();
		m_sQTData.m_ObjList.clear();
	}


};
struct CSBUFF
{
	D3DXVECTOR3 vPickPos;
	float		fRadius;
	float		Alpha[4];
	int			iIndex;
	int			iCol;
	int			iRow;


};

struct CSPOSBF
{
	D3DXVECTOR3 vPos;
};
class Sample :public KG_Core
{
public:
	//Obj
	std::map< int, std::shared_ptr<CBY::CBY_Object>>					m_ObjectList;
	std::shared_ptr < CBY::CBY_Object>							m_Object;
	std::shared_ptr<CBY::CBY_Object>								m_SelectObj;
public:

	std::shared_ptr<KG_BackViewCamera>				m_BackViewCamera;
	std::shared_ptr<KG_DebugCamera>					m_DebugCamera;
	std::shared_ptr<KG_ModelViewCamera>				m_ModelViewCamera;
	std::shared_ptr<JH::KG_SkyBox>						m_SkyBox;
public:
	//Texture Splatting
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_SplattingTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_pDestSrv;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_pSplSrv;
	CTexture*							m_pTexture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDestTexture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pSrcTexture;
	//ComputeShader Splatting
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pUAVTexture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>	pReadTexture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pLoadAlphaTexture;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_pStructureBF;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_pBufSrv;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_pStructureEditBF;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_pEditBufSrv;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_pReadSrv;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_pLoadAlphaSrv;

	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>	m_pUAV;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader>			m_pCS;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader>			m_pEditCS;
	CSBUFF								m_vBuf0[1];
	CSPOSBF								m_vBuf1[1];
	//Shadow
	std::vector<KG_DxRT>				m_RTTextureList;
public:
	KG_Minimap								m_MiniMap;
	D3DXMATRIX								m_matTopView;
	D3DXMATRIX								m_matTopProj;
	D3DXMATRIX								m_matScale;
	D3DXMATRIX								m_matRot;
	D3DXMATRIX								m_matMove;
	D3DXVECTOR3								m_NearPoint;
	std::shared_ptr<JH::JH_MapObj>				m_CurrentMapObj;
	std::shared_ptr<JH::JH_MapObj>			m_pSelectMapObj;
	DWORD									m_SelectObjID;

	KG_Shadow								m_Shadow;
	KG_ArcBall								m_WorldArcBall;
	KG_ArcBall								m_ViewArcBall;


	D3D11_VIEWPORT	m_vp;


	MAPDATA m_sMapData;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pLoadingSplSrv;

public:
	std::shared_ptr<KG_ShapeLine>m_DebugLine;
	std::shared_ptr<JH::JH_Map>		m_Map;
	std::shared_ptr<JH::HQuadTree>	m_QuadTree;
	TCHAR		m_tmpBuffer[MAX_PATH];
public:
	bool		bAttach;					//TOOL 관련 변수들
	bool		bSplatting;
	bool		bMapFlatting;
	bool		bObjectAdd;
	bool		bSelect;
	bool		bSet;
	int			CurrentObjIndex;
	int			m_ObjID;
	float		m_HeightVlaue;
	float		m_fTimer;
	float		m_Interval;
	float		m_fYaw;
	float		m_fPitch;
	float		m_fRoll;

	float		m_fScaleX;
	float		m_fScaleY;
	float		m_fScaleZ;
	float		m_fValue;

	
	DWORD		m_ToolState;
	
	//Load
	int					m_iTemp;
	
	
	TCHAR				m_pBuffer[256];
	TCHAR				m_pString[256];
	//Save
	const TCHAR*		m_pSPTAFile;
public:
	bool CreateMap(int iWidth,
		int iHeight,
		int iCellCount,
		int iCellSize,
		const TCHAR* pTexturFileName,
		const TCHAR* pNormalMapFileName = nullptr,
		const TCHAR* pHeightMapFileName = nullptr);
	int CreateObj(const TCHAR* pSkinFileName, const TCHAR* pBoneFileName, D3DXMATRIX& matWorld);
	void SelectObject();
	void MapUpDown(JH::SPHERE Sphere);
	void MapFlatting(JH::SPHERE Sphere);
	void ObjRotation();
	void ObjTranslation();
	void ObjScale();
	void ResetSRTValue();
	//
	//Cpu Splatting
	//HRESULT MapSplatting(SPHERE Sphere);
	HRESULT CreateCSTexture();
	HRESULT	CreateSplattingTexture();
	HRESULT	CreateSplattingTexture(JH::KG_Node* pNode);
	void GetNearPoint();

	void 	RunComputeShaderSplatting(ID3D11ComputeShader* pCS, ID3D11ShaderResourceView* pShaderResourceView,
		ID3D11ShaderResourceView* pBufSrv,
		ID3D11UnorderedAccessView* pUnorderedAccessView, UINT X, UINT Y, UINT Z);
	//저장
	TCHAR*	SaveFileDlg(TCHAR* szExt, TCHAR* szTitle);
	bool	SaveMapData(const TCHAR* LoadFile);
	bool	SaveDataReset();
	bool	ObjectDelete();
	TCHAR* FixupName(T_STR name);
	//Load
	bool   LoadMapData(const TCHAR* LoadFile);
	INT	   AddObject(OBJECT Obj);
	//OVERRIDE

	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	Sample();
	virtual ~Sample();
};