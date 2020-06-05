#pragma once
#include "CBY_BoneBox.h"
#include "CBY_CharBox.h"
#include "KG_TextureMgr.h"

namespace CBY
{
	enum ASE_NODE_TYPE
	{
		GEOM_MESH = 0,
		HELPER_DUMMEY,
		HELPER_BONE,
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

	struct CBY_Bone
	{
		KG_Box	m_Box;
		D3DXVECTOR3	m_vPos;
	};

	struct PNCTIW_VERTEX
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		D3DXVECTOR4 c;
		D3DXVECTOR2 t;
		D3DXVECTOR4 i;
		D3DXVECTOR4 w;
		bool inline operator == (const PNCTIW_VERTEX& v)
		{
			if (p == v.p && c == v.c && t == v.t && n == v.n)
			{
				return true;
			}
			return false;
		}
		inline PNCTIW_VERTEX() {
			i[0] = 0;
			i[1] = 0;
			i[2] = 0;
			i[3] = 0;
			w[0] = 0;
			w[1] = 0;
			w[2] = 0;
			w[3] = 0;
		}
		inline PNCTIW_VERTEX(D3DXVECTOR3 p0, D3DXVECTOR3 n0, D3DXVECTOR4 c0, D3DXVECTOR2 t0,
			D3DXVECTOR4 i0, D3DXVECTOR4 w0)
		{
			p = p0;
			n = n0;
			c = c0;
			t = t0;
			i[0] = i0.x;
			i[1] = i0.y;
			i[2] = i0.z;
			i[3] = i0.w;
			w[0] = w0.x;
			w[1] = w0.y;
			w[2] = w0.z;
			w[3] = w0.w;

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

	class CBY_Mesh
	{
	public:
		C_STR       m_szName;
		C_STR       m_szParentName;
		int m_iTexIndex;
		bool m_bBone;
		bool m_bRoot;
		UINT m_iBaseVertex;
		UINT m_iNumVertex;
		UINT m_iBaseIndex;
		UINT m_iNumIndex;
		int m_iType;
		KG_Box m_CharBox;
		CBY_Bone m_Bone;
		CBY_BoneBox m_BoneBox;

	public:
		int m_iParentIndex;
		D3DXMATRIX m_matWorld;
		D3DXMATRIX m_matInverse;
		CTexture* m_pTexture;
		CONDATA m_cbData;
		std::vector<CBY_Mesh> m_subMesh;
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
		CBY_Mesh();
		virtual ~CBY_Mesh();
	};
}

