#pragma once
#include"KG_Model.h"
#include"KG_TextureMgr.h"
struct SCENEINFO
{
	int FirstFrame;
	int LastFrame;
	int FrameSpeed;
	int TickPerFrame;
	int iNumMesh;		// 메쉬오브젝트 개수
	int iNumMaterial;   // 루트매터리얼 개수
	int iMaxWeight;		// 정점 당 가중치
	int iBindPose;		// 바인딩 포즈 에니메이션 여부
};

struct MapInfo
{
	int iSubNo;
	TCHAR szTextureName[256];

	MapInfo()
	{
		iSubNo = -1;
		ZeroMemory(&szTextureName, sizeof(TCHAR) * 256);
	}
};
struct Mtrl
{

	std::vector<MapInfo> m_MapList;
	std::vector<Mtrl> submtl;


};
struct FaceNormal
{
	float fn[3];
	float vn[3][3];
};
struct aseVertex
{

};
struct aseINDEX
{
	UINT i[3], SubMtrl;


};
struct AnimTrack
{
	int iTick;
	D3DXVECTOR3		p;
	D3DXQUATERNION	q;
};
struct HTri
{
	PNCT_VERTEX	v[3];
	int			iSubIndex;
	HTri()
	{
		iSubIndex = -1;
	}
};
struct DescendingDateSort
{
	bool operator()(HTri& rpStart, HTri& rpEnd)
	{
		return rpStart.iSubIndex < rpEnd.iSubIndex;
	}
};
class HMesh
{
public:
	TCHAR		m_szName[256];
	TCHAR		m_szParentName[256];


	int			m_iRef;
	int			m_iTextIndex;

	UINT        m_iBaseVertex;
	UINT        m_iNumVertex;
	UINT		m_iNumFace;
	int			m_iParentIndex;

	std::vector<HTri> m_TriList;
	D3DXMATRIX  m_matWorld;
	//_D3D_MATRIX	m_matInverseWorld;
	D3DXMATRIX  m_matCalculation;

	D3DXVECTOR3		m_vAnimScale;
	D3DXQUATERNION		m_qAnimScaleRotation;
	D3DXQUATERNION		m_qAnimRotation;
	D3DXVECTOR3		m_vAnimPos;


	CTexture*	m_pTexture;
	std::vector<HMesh> subMesh;


	std::vector<AnimTrack> m_AnimTrackPos;
	std::vector<AnimTrack> m_AnimTrackRotate;
	std::vector<AnimTrack> m_AnimTrackScale;


	CONDATA     m_cbData;


public:
	HMesh()
	{
		m_iRef = -1;
		m_iTextIndex = -1;
		m_iBaseVertex = 0;
		m_iNumVertex = 0;
		m_iNumFace = 0;
		m_pTexture = nullptr;
		m_iParentIndex = -1;

		D3DXMatrixIdentity(&m_matCalculation);
		
		ZeroMemory(&m_szParentName, sizeof(TCHAR) * 256);
		ZeroMemory(&m_szName, sizeof(TCHAR) * 256);
	}
	~HMesh() {};
};
class MaxObj :public KG_Model
{
public:
	std::vector<PNCT_VERTEX>	 m_PNCTBuffer;
	std::vector<HTri>		 m_FaceBuffer;
	//vector<Mesh>		 m_MeshList;

public:
	TCHAR  m_pBuffer[256];
	TCHAR  m_pString[256];
	int    m_iTemp;
	SCENEINFO	 m_Scene;
	std::vector<Mtrl> m_ExpMtlList;
	std::vector<HMesh>	m_ExpObject;
	float		 m_fElapseTick;
	FILE*		 m_pStream;
public:
	bool	Load(ID3D11Device* pdvice, ID3D11DeviceContext* pContext, const TCHAR* LoadFile,const TCHAR* ShaderFile);
	void	Convert();
	bool	OpenStream(const TCHAR* FileName);
	bool	CloseStream();
	bool	Parsing(const TCHAR* FileName);
	int		GetParentIndex(const TCHAR*	strName);
	//bool	AddVertex(int iObj, int iFace, int iVertex, Mesh& mesh);
public:
	HRESULT CreateVertexData()override;
	HRESULT CreateIndexData()override;
	bool Init()override;
	bool Frame()override;
	bool Render()override;
	bool Release()override;
public:
	MaxObj() { m_fElapseTick = 0.0f; };
	virtual ~MaxObj() { Release(); };
};

