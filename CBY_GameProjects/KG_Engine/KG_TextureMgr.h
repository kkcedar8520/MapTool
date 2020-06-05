#pragma once
#include "KG_Std.h"
class CTexture
{
public:
	T_STR  m_szName;
	T_STR  m_szPath;
	ID3D11ShaderResourceView* m_pTextureRV;
	ID3D11SamplerState* m_pSamplerState;
	D3D11_SAMPLER_DESC			m_SampleDesc;
public:
	bool Release();
	HRESULT Load(ID3D11Device* pDevice, const TCHAR* strFilePath, bool btga);
	void SetPath(const TCHAR* pPath);
	bool Apply(ID3D11DeviceContext* pContext);
public:
	CTexture();
	virtual ~CTexture();
};

class KG_TextureMgr : public Singleton<KG_TextureMgr>
{
private:
	friend class Singleton<KG_TextureMgr>;
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pContext;

public:
	typedef std::unordered_map<INT, CTexture*> TEMPLATE_MAP;
	typedef TEMPLATE_MAP::iterator TEMPLATE_MAPitor;
	TEMPLATE_MAPitor m_Mapitor;
	TEMPLATE_MAP m_Map;
	INT			 m_iIndex;
public:
	INT	Add(ID3D11Device* pDevice, const TCHAR* pFileName, const TCHAR* szPath);
	INT Add(ID3D11Device* pDevice, const TCHAR* pFileName);
	CTexture* const	GetPtr(INT iIndex);
	CTexture* const GetPtr(T_STR strFindName);
	bool Release();

public:
	KG_TextureMgr();
	virtual ~KG_TextureMgr();
};

#define I_Texture KG_TextureMgr::GetInstance()


