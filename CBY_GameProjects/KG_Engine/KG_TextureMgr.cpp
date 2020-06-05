#include "KG_TextureMgr.h"

HRESULT CTexture::Load(ID3D11Device* pDevice, const TCHAR* strFilePath, bool btga)
{

	if (strFilePath == NULL)
		return S_OK;
	HRESULT hr = S_OK;
	TCHAR szFileName[MAX_PATH] = { 0, };
	_stprintf_s(szFileName, _T("%s%s"), m_szPath.c_str(), strFilePath);
	m_szName = strFilePath;

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	loadInfo.Format = DXGI_FORMAT_FROM_FILE;//DXGI_FORMAT_BC1_UNORM;

	
	DirectX::TexMetadata imageMetadata;
	DirectX::ScratchImage* pScratchImage = new DirectX::ScratchImage();

	if (btga)
	{
		hr = DirectX::LoadFromTGAFile(szFileName, &imageMetadata, *pScratchImage);

		hr = DirectX::CreateShaderResourceView(pDevice, pScratchImage->GetImages(), pScratchImage->GetImageCount(), pScratchImage->GetMetadata(), &m_pTextureRV);
	}
	else
	{
		D3DX11_IMAGE_LOAD_INFO loadInfo;
		ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
		loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		loadInfo.Format = DXGI_FORMAT_FROM_FILE;//DXGI_FORMAT_BC1_UNORM;

												// Load the Texture
		hr = D3DX11CreateShaderResourceViewFromFile(pDevice, szFileName, &loadInfo, NULL, &m_pTextureRV, NULL);
		if (FAILED(hr))
		{
			//ErrorQuit(szFileName);
			return hr;
		}
	}
	if (pScratchImage!=nullptr)
	{
		delete pScratchImage;
	}
	return hr;
}

void CTexture::SetPath(const TCHAR* pPath)
{
	m_szPath = pPath;

}

bool CTexture::Apply(ID3D11DeviceContext* pContext)
{
	assert(pContext);
	if (!m_pTextureRV)
	{
		return false;
	}
	pContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	//pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerState);
	return true;
}

bool CTexture::Release()
{
	if (m_pTextureRV)
	{
		m_pTextureRV->Release();
		m_pTextureRV = NULL;
	}
	if (m_pSamplerState)
	{
		m_pSamplerState->Release();
		m_pSamplerState = NULL;
	}
	return true;
}

CTexture::CTexture()
{
	m_pTextureRV = nullptr;
	m_pSamplerState = nullptr;
	m_szPath = _T("");
}
CTexture::~CTexture()
{
}

INT KG_TextureMgr::Add(ID3D11Device* pDevice, const TCHAR* pFileName)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	bool bTga = false;
	if (pFileName)
	{
		_tsplitpath(pFileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
		//D3DX11_IMAGE_FILE_FORMAT 
		if (_tcsicmp(Ext, _T(".tga")) == 0)
		{
			bTga = true;
			//_tcscpy(Ext, _T(".bmp"));
		}
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (TEMPLATE_MAPitor itor = m_Map.begin(); itor != m_Map.end(); itor++)
		{
			CTexture* pPoint = (CTexture*)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				return (*itor).first;
			}
		}
	}
	CTexture* pPoint = NULL;
	pPoint = new CTexture;

	TCHAR szPath[MAX_PATH] = { 0, };
	_stprintf_s(szPath, _T("%s%s"), Drive, Dir);
	pPoint->SetPath(szPath);

	if (FAILED(pPoint->Load(pDevice, szFileName, bTga)))
	{
		return 0;
	}
	m_Map.insert(std::make_pair(++m_iIndex, pPoint));
	return m_iIndex;
}

INT	KG_TextureMgr::Add(ID3D11Device*	 pDevice, const TCHAR* pFileName, const TCHAR* szPath)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	bool bTga = false;
	if (pFileName)
	{
		_tsplitpath(pFileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
		if (_tcsicmp(Ext, _T(".tga")) == 0)
		{
			bTga = true;
			//_tcscpy(Ext, _T(".dds"));
		}
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (TEMPLATE_MAPitor itor = m_Map.begin(); itor != m_Map.end(); itor++)
		{
			CTexture* pPoint = (CTexture*)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				return (*itor).first;
			}
		}
	}
	CTexture* pPoint = NULL;
	pPoint = new CTexture;
	pPoint->SetPath(szPath);

	if (FAILED(pPoint->Load(pDevice, szFileName, bTga)))
	{
		return 0;
	}
	m_Map.insert(std::make_pair(++m_iIndex, pPoint));
	return m_iIndex;
}

CTexture* const	KG_TextureMgr::GetPtr(INT iIndex)
{
	TEMPLATE_MAP::iterator iter;
	iter = m_Map.find(iIndex);
	if (iter == m_Map.end())
	{
		return nullptr;
	}
	return iter->second;
}

CTexture* const KG_TextureMgr::GetPtr(T_STR strFindName)
{
	for (TEMPLATE_MAPitor itor = m_Map.begin(); itor != m_Map.end(); itor++)
	{
		CTexture* pPoint = (CTexture*)(*itor).second;
		if (pPoint->m_szName == strFindName)
		{
			return (*itor).second;
		}
	}
	return NULL;
}

bool KG_TextureMgr::Release()
{
	TEMPLATE_MAP::iterator iter;
	for (iter = m_Map.begin(); iter != m_Map.end(); iter++)
	{
		CTexture* pData = (CTexture*)iter->second;
		pData->Release();
		delete pData;
	}
	m_Map.clear();
	return true;
}

KG_TextureMgr::KG_TextureMgr()
{
	m_pd3dDevice = NULL;
	m_iIndex = 0;
	m_Map.clear();
}


KG_TextureMgr::~KG_TextureMgr()
{
	CTexture* pPoint;
	for (TEMPLATE_MAPitor itor = m_Map.begin(); itor != m_Map.end(); itor++)
	{
		pPoint = (CTexture*)(*itor).second;
		pPoint->Release();
		delete pPoint;
	}
	m_Map.clear();
	m_iIndex = 0;
	Release();
}
