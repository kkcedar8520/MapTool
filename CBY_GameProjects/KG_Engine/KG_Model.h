#pragma once
#include "KG_Std.h"
#include "KG_DxState.h"

struct P_VERTEX
{
	D3DXVECTOR3 p;
};
struct PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;
};

struct PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;
};

struct PCT_VERTEX				//쉐이더의 정보 구조체
{
	D3DXVECTOR3 p;				//좌표
	D3DXVECTOR4 c;			//색
	D3DXVECTOR2 t;

	bool operator == (const PCT_VERTEX& v)
	{
		if (p == v.p && c == v.c && t == v.t)
		{
			return true;
		}
		return false;
	}
	PCT_VERTEX() {}
	PCT_VERTEX(D3DXVECTOR3 p, D3DXVECTOR4 c, D3DXVECTOR2 t)
	{
		this->p = p;
		this->c = c;
		this->t = t;
	}
};

struct PNCT_VERTEX				//쉐이더의 정보 구조체
{
	D3DXVECTOR3 p;				//좌표
	D3DXVECTOR3 n;
	D3DXVECTOR4 c;			//색
	D3DXVECTOR2 t;

	bool operator == (const PNCT_VERTEX& v)
	{
		if (p == v.p && n == v.n && c == v.c && t == v.t)
		{
			return true;
		}
		return false;
	}
	PNCT_VERTEX() {}
	PNCT_VERTEX(D3DXVECTOR3 p, D3DXVECTOR3 n, D3DXVECTOR4 c, D3DXVECTOR2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};

struct CONDATA				//상수 버퍼의 정보 구조체
{
	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Proj;
	D3DXMATRIX Normal;
	D3DXVECTOR4 ddata;	//273/345
	CONDATA()
	{
		ddata.x = 1.0f;
	}
};

struct BufType
{
	int i;
	float f;
};

struct CRectUV
{
	D3DXVECTOR4 UV;
	RECT Rect;
};

struct CInstatnce
{
	D3DXMATRIX matWorld;
	D3DXVECTOR4 uv[4];
	D3DXVECTOR4 color;
};

struct CB_LIGHT
{
	D3DXVECTOR4 g_cAmbientLightColor;
	D3DXVECTOR4 g_cDifuseLightColor;
	D3DXVECTOR4 g_cSpecularLightColor;
	D3DXVECTOR4 g_vLightDir;		//w=감쇠
	D3DXVECTOR4 g_vLightPos;
	D3DXVECTOR4 g_vEyeDir;
	D3DXVECTOR4 g_vEyePos;
	D3DXVECTOR4 g_vSpot;			//내부,외부,히도값
	D3DXMATRIX g_LTM;
};

struct LIGHTINFO
{
	D3DXVECTOR4 cAmbientLightColor;
	D3DXVECTOR4 cDifuseLightColor;
	D3DXVECTOR4 cSpecularLightColor;
	D3DXVECTOR4 LightPos;
	D3DXVECTOR4 vAt;
	D3DXVECTOR4 vSpot;
};

class KG_Model
{
public:
	KG_DxHelperEx m_obj;

public:
	D3DXMATRIX	m_matWorld;
	D3DXMATRIX	m_matView;
	D3DXMATRIX  m_matProj;
	D3DXVECTOR3	m_vPos;
	D3DXVECTOR3	m_vLook;
	D3DXVECTOR3	m_vUp;
	D3DXVECTOR3	m_vSide;

public:
	std::vector<PNCT_VERTEX> m_VerTex;
	std::vector<DWORD> m_IndexData;				//인덱스 버퍼의 데이터
	CONDATA m_ConData;

	//Light
public:
	std::vector<LIGHTINFO> m_LightData;

	//Splite
public:
	RECT m_Rect;
	float m_AnimTime;
	std::vector<CRectUV> m_RectList;
	BufType m_vBuf0;
	ID3D11Buffer* m_pBuf0;


public:
	void SetRectAnimation(float fAnimTime, int iWidth, int iWidthSize, int iHeight, int iHeightSize);
	D3DXVECTOR4 SetUV(RECT& Rect);
	//End Splite

public:
	virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);		//행렬 셋팅
	virtual bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context, const TCHAR* ShaderFileName = L"../../data/shader/DefaultShader.txt", const TCHAR* TexFileName = nullptr, const CHAR* VSName = "VSmat", const CHAR* PSName = "PS");
	//bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context, const TCHAR* verFileName, const TCHAR* pixFileName,const TCHAR* TexFileName, const CHAR* VSName = "VS", const CHAR* PSName = "PS");
	virtual HRESULT CreateVertexData();					//정점 버퍼 데이터 생성
	virtual HRESULT CreateIndexData();					//인덱스 버퍼 데이터 생성
	virtual HRESULT CreateVertexBuffer();				//정점 버퍼 생성
	virtual HRESULT CreateIndexBuffer();				//인덱스 버퍼 생성
	virtual HRESULT CreateConstantBuffer();				//상수 버퍼 생성

	virtual HRESULT CreateInputLayout();				//레이아웃 생성

	virtual HRESULT LoadTexture(const TCHAR* TexFileName);		//텍스쳐 로드
	virtual HRESULT LoadShader(const TCHAR* ShaderFileName, const CHAR* VSName = "VS", const CHAR* PSName = "PS");			//쉐이더 로드
	virtual bool GameDataLoad(const TCHAR* pszLoad);
	//virtual HRESULT LoadShader(const TCHAR* verFileName, const TCHAR* pixFileName, const CHAR* VSName = "VS", const CHAR* PSName = "PS");

	virtual void Convert(std::vector<PT_VERTEX>& list);
	virtual void Convert(std::vector<PNCT_VERTEX>& list);
	virtual void UVConvert(std::vector<PNCT_VERTEX>& vertex, RECT& Rect, int Width, int Height);

	virtual void UpdatePSShader(ID3D11ShaderResourceView* textview);
	virtual void UpdateVertexData(std::vector<PNCT_VERTEX>& list);					//정점 버퍼 데이터 업데이트
	//virtual void UVConvert();

public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	PreRender();
	virtual bool	Render();
	virtual bool	PostRender();
	virtual bool	Release();

public:
	KG_Model();
	~KG_Model();
};
