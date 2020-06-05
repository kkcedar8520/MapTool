#pragma once
#include "KG_Model.h"
#include "KG_TextureMgr.h"

enum SET_DATA_TYPES
{
	NULL_DATA = 0,
	BOOL_DATA,
	INT_DATA,
	FLOAT_DATA,
	STRING_DATA,
	VECTOR_DATA,
	VERTEX_DATA,
	INDEX_DATA,
	FACE_DATA,
	MATRIX_DATA,
	SCENE_DATA
};
enum ASE_NODE_TYPE
{
	GEOM_MESH = 0,
	HELPER_DUMMEY,
	HELPER_BONE,
};


enum AniType
{
	ANI_POS = 0,
	ANI_ROT,
	ANI_SCL
};

struct CIndex
{
	UINT i[3], subMtrl;
};

struct CSubMap
{
	int  iSubNo;
	TCHAR szTextureName[256];
	inline CSubMap()
	{
		iSubNo = -1;
		ZeroMemory(szTextureName, sizeof(TCHAR) * 256);
	}
};

struct C_Mtrl
{
	int  iSubNo;
	int  m_iTexIndex;
	CHAR szName[256];
	CHAR szTextureName[256];
	std::vector<C_Mtrl> SubMaterial;
	std::vector<CSubMap> texList;
};


struct CScene
{
	int iVersion;		// 버전
	int	iFirstFrame;    // 시작 프레임
	int	iLastFrame;     // 마지막 프레임
	int	iFrameSpeed;    // 1초당 프레임 개수(30)
	int	iTickPerFrame;  // 1프레임의 틱 값(160)
	int iNumMesh;		// 메쉬오브젝트 개수
	int iNumMaterial;   // 루트매터리얼 개수
	int iMaxWeight;		// 정점 당 가중치
	int iBindPose;		// 바인딩 포즈 에니메이션 여부
};

struct CAnimationTrack
{
	int iTick;
	D3DXVECTOR3 p;
	D3DXQUATERNION q;

	inline CAnimationTrack()
	{
		iTick = 0;
		p = { 0,0,0 };
		D3DXQuaternionIdentity(&q);
	}

	inline CAnimationTrack(int tick, D3DXVECTOR3 pos, D3DXQUATERNION qua)
	{
		iTick = tick;
		p = pos;
		q = qua;
	}
};

struct PNCTIW_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR4 c;
	D3DXVECTOR2 t;
	D3DXVECTOR4 i;
	D3DXVECTOR4 w;
	bool operator == (const PNCTIW_VERTEX& v)
	{
		if (p == v.p && c == v.c && t == v.t && n == v.n)
		{
			return true;
		}
		return false;
	}
	inline PNCTIW_VERTEX() {
		this->i[0] = 0;
		this->i[1] = 0;
		this->i[2] = 0;
		this->i[3] = 0;
		this->w[0] = 0;
		this->w[1] = 0;
		this->w[2] = 0;
		this->w[3] = 0;
	}
	inline PNCTIW_VERTEX(D3DXVECTOR3 p, D3DXVECTOR3 n, D3DXVECTOR4 c, D3DXVECTOR2 t,
		D3DXVECTOR4 i, D3DXVECTOR4 w)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
		this->i[0] = i.x;
		this->i[1] = i.y;
		this->i[2] = i.z;
		this->i[3] = i.w;
		this->w[0] = w.x;
		this->w[1] = w.y;
		this->w[2] = w.z;
		this->w[3] = w.w;

	}
};


struct CMatSetData
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vScale;
	D3DXQUATERNION qR;
	D3DXQUATERNION qScale;
	D3DXMATRIX matRotation;
	D3DXMATRIX matScale;
	D3DXMATRIX matScaleRot;
	D3DXMATRIX matScaleInverRot;

	inline CMatSetData()
	{
		D3DXMatrixIdentity(&matRotation);
		D3DXMatrixIdentity(&matScale);
		D3DXMatrixIdentity(&matScaleRot);
		D3DXMatrixIdentity(&matScaleInverRot);
		D3DXQuaternionIdentity(&qR);
		D3DXQuaternionIdentity(&qScale);
	}

	inline CMatSetData(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXQUATERNION r, D3DXQUATERNION scl)
	{
		vPos = pos;
		vScale = scale;
		qR = r;
		qScale = scl;
		D3DXMatrixRotationQuaternion(&matRotation, &qR);
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	}
};

class CMesh
{
public:
	C_STR       m_szName;
	C_STR       m_szParentName;
	int m_iTexIndex;
	UINT m_iBaseVertex;
	UINT m_iNumVertex;
	UINT m_iBaseIndex;
	UINT m_iNumIndex;
	int m_iType;

public:
	int m_iParentIndex;
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matInverse;
	CTexture* m_pTexture;
	CONDATA m_cbData;
	std::vector<CMesh> m_subMesh;
	std::vector<PNCT_VERTEX> m_vtlist;
	D3DXMATRIX m_matCalculation;
	D3DXMATRIX m_matCalculationBegin;
	std::vector<CAnimationTrack> posTrack;
	std::vector<CAnimationTrack> rotTrack;
	std::vector<CAnimationTrack> sclTrack;

public:
	std::vector<PNCT_VERTEX> vblist;
	std::vector<DWORD> iblist;

public:
	///////행렬 분해 원소
	D3DXVECTOR3	m_vAnimScale;
	D3DXQUATERNION  m_qAnimScaleRotation;
	D3DXQUATERNION  m_qAnimRotation;
	D3DXVECTOR3 m_vAnimPos;
	//////////////////

public:
	inline CMesh()
	{
		m_iParentIndex = -1;
		m_iTexIndex = -1;
		m_iBaseVertex = 0;
		m_iNumVertex = 0;
		m_iBaseIndex = 0;
		m_iNumIndex = 0;
		m_pTexture = nullptr;
		m_iType = GEOM_MESH;
		ZeroMemory(&m_cbData, sizeof(CONDATA));
	}
	virtual ~CMesh() {}
};


class CMeshSkin : public CMesh
{
public:
	std::vector<CMeshSkin> subMeshSkin;
	std::vector<PNCTIW_VERTEX> listSkin;
	std::vector<PNCTIW_VERTEX> vblistSkin;
	std::vector<DWORD> iblistSkin;
	inline CMeshSkin() {}
	virtual ~CMeshSkin() {}
};