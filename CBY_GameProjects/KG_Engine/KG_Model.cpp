#include "KG_Model.h"

bool KG_Model::GameDataLoad(const TCHAR* pszLoad)
{
	TCHAR pBuffer[256] = { 0 };
	TCHAR pTemp[256] = { 0 };

	int iLoadInfoNum = 0;
	FILE* fp_src;
	_wfopen_s(&fp_src, pszLoad, _T("rt"));
	if (fp_src == NULL) return false;

	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s%d%s"), pTemp, _countof(pTemp), &iLoadInfoNum);
	m_LightData.resize(iLoadInfoNum);

	std::vector<D3DXVECTOR4> data;
	for (int iCnt = 0; iCnt < iLoadInfoNum; iCnt++)
	{
		data.clear();
		int iNumFrame = 0;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s %d"), pTemp, _countof(pTemp), &iNumFrame);
		//m_rtSpriteList[iCnt].resize(iNumFrame);

		data.resize(iNumFrame);
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%s %f %f %f %f"), pTemp, _countof(pTemp),
				&data[iFrame].x, &data[iFrame].y, &data[iFrame].z, &data[iFrame].w);
		}
		LIGHTINFO src;
		src.cAmbientLightColor = data[0];
		src.cDifuseLightColor = data[1];
		src.cSpecularLightColor = data[2];
		src.LightPos = data[3];
		src.vAt = data[4];
		src.vSpot.x = D3DXToRadian(data[5].x / 2.0f);
		src.vSpot.y = D3DXToRadian(data[5].y / 2.0f);
		src.vSpot.z = data[5].z;
		src.vSpot.w = data[5].w;

		m_LightData[iCnt] = src;
	}
	fclose(fp_src);
	data.clear();
	return true;
}

void KG_Model::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)		//행렬 셋팅
{
	if (world != nullptr)
	{
		m_matWorld = *world;
	}
	if (view != nullptr)
	{
		m_matView = *view;
	}
	if (proj != nullptr)
	{
		m_matProj = *proj;
	}

	D3DXMatrixTranspose(&m_ConData.World, &m_matWorld);
	D3DXMatrixTranspose(&m_ConData.View, &m_matView);
	D3DXMatrixTranspose(&m_ConData.Proj, &m_matProj);


	if (m_obj.iCBBufferType == CB_DYNAMIC)
	{
		D3D11_MAPPED_SUBRESOURCE mss;
		if (m_obj.m_pConstantBuffer != nullptr)
		{
			if (SUCCEEDED(m_obj.m_pContext->Map(m_obj.m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mss)))
			{
				CONDATA* pData = (CONDATA*)mss.pData;
				memcpy(pData, &m_ConData, sizeof(CONDATA));
				m_obj.m_pContext->Unmap(m_obj.m_pConstantBuffer.Get(), 0);
			}
		}
	}
	else if (m_obj.iCBBufferType == CB_DEFAULT)
	{
		if (m_obj.m_pConstantBuffer != nullptr)
		{
			m_obj.m_pContext->UpdateSubresource(m_obj.m_pConstantBuffer.Get(), 0, NULL, &m_ConData, 0, 0);
		}
	}

	m_vLook.x = m_matWorld._31;
	m_vLook.y = m_matWorld._32;
	m_vLook.z = m_matWorld._33;
	m_vSide.x = m_matWorld._11;
	m_vSide.y = m_matWorld._12;
	m_vSide.z = m_matWorld._13;
	m_vUp.x = m_matWorld._21;
	m_vUp.y = m_matWorld._22;
	m_vUp.z = m_matWorld._23;

	D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Normalize(&m_vSide, &m_vSide);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

}

bool KG_Model::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context, const TCHAR* ShaderFileName, const TCHAR* TexFileName, const CHAR* VSName, const CHAR* PSName)
{
	m_obj.m_pd3dDevice = pd3dDevice;
	m_obj.m_pContext = Context;

	if (FAILED(LoadShader(ShaderFileName, VSName, PSName)))
	{
		return false;
	}
	if (FAILED(CreateVertexData()))
	{
		return false;
	}
	if (FAILED(CreateIndexData()))
	{
		return false;
	}
	if (FAILED(CreateVertexBuffer()))
	{
		return false;
	}
	if (FAILED(CreateIndexBuffer()))
	{
		return false;
	}
	if (FAILED(CreateConstantBuffer()))
	{
		return false;
	}

	if (FAILED(CreateInputLayout()))
	{
		return false;
	}
	if (FAILED(LoadTexture(TexFileName)))
	{
		return false;
	}
	return true;
}



HRESULT KG_Model::CreateVertexData()					//정점 버퍼 데이터 생성
{
	HRESULT hr = S_OK;

	m_obj.m_VertexSize = sizeof(PNCT_VERTEX);

	m_VerTex.resize(4);
	m_VerTex[0].p = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_VerTex[1].p = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_VerTex[2].p = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	m_VerTex[3].p = D3DXVECTOR3(1.0f, -1.0f, 0.0f);				//정점 좌표 입력

	m_VerTex[0].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VerTex[1].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VerTex[2].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VerTex[3].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_VerTex[0].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
	m_VerTex[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VerTex[2].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VerTex[3].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);			//정점 컬러값 입력

	m_VerTex[0].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VerTex[1].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VerTex[2].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VerTex[3].t = D3DXVECTOR2(1.0f, 1.0f);			//정점 텍스쳐 좌표

	return hr;
}

HRESULT KG_Model::CreateIndexData()					//인덱스 버퍼 데이터 생성
{
	HRESULT hr = S_OK;

	m_IndexData.resize(6);
	m_IndexData[0] = 0;
	m_IndexData[1] = 1;
	m_IndexData[2] = 2;

	m_IndexData[3] = 2;
	m_IndexData[4] = 1;
	m_IndexData[5] = 3;

	return hr;
}

void KG_Model::UpdateVertexData()
{
	m_VerTex[0].p = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_VerTex[1].p = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_VerTex[2].p = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	m_VerTex[3].p = D3DXVECTOR3(1.0f, -1.0f, 0.0f);				//정점 좌표 입력
}

HRESULT KG_Model::CreateVertexBuffer()				//정점 버퍼 생성
{
	HRESULT hr = S_OK;

	m_obj.m_Vertexnum = m_VerTex.size();

	D3D11_BUFFER_DESC bd;										//버퍼 생성의 입력 정보를 받는다
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = m_obj.m_VertexSize * m_VerTex.size();				//버퍼의 크기
	bd.Usage = D3D11_USAGE_DEFAULT;								//버퍼의 읽기 쓰기 방법
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;						//버퍼의 파이프라인 해당 위치

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = &m_VerTex.at(0);							//초기화 데이터에 대한 포인터

	hr = m_obj.m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_obj.m_pVertexBuffer);			//버퍼생성

	return hr;
}

HRESULT KG_Model::CreateIndexBuffer()					//인덱스 버퍼 생성
{
	HRESULT hr = S_OK;
	if (m_IndexData.size() <= 0) return S_OK;
	m_obj.m_Indexnum = m_IndexData.size();

	D3D11_BUFFER_DESC bd;										//버퍼 생성의 입력 정보를 받는다
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD)*m_IndexData.size();				//버퍼의 크기
	bd.Usage = D3D11_USAGE_DEFAULT;								//버퍼의 읽기 쓰기 방법
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;						//버퍼의 파이프라인 해당 위치 //버퍼의 용도

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = &m_IndexData.at(0);							//초기화 데이터에 대한 포인터

	hr = m_obj.m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_obj.m_pIndexBuffer);			//버퍼생성

	return hr;
}

HRESULT KG_Model::CreateConstantBuffer()				//상수 버퍼 생성
{
	HRESULT hr = S_OK;

	ZeroMemory(&m_ConData, sizeof(CONDATA));		//상수버퍼에 들어갈 데이터 정보 초기화
	D3DXMatrixIdentity(&m_ConData.World);
	D3DXMatrixIdentity(&m_ConData.View);
	D3DXMatrixIdentity(&m_ConData.Proj);
	m_ConData.ddata = D3DXVECTOR4(1, 1, 1, 1);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(CONDATA);								//버퍼의 크기
	bd.Usage = D3D11_USAGE_DYNAMIC;								//버퍼의 읽기 쓰기 방법, Usage를 DYNAMIC으로 설정을 해야 cpu가 접근 가능하다
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;					//cpu가 접근하여 쓸수 있다.
	m_obj.iCBBufferType = CB_DYNAMIC;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = &m_ConData;							//초기화 데이터에 대한 포인터

	hr = m_obj.m_pd3dDevice->CreateBuffer(&bd, &InitData, m_obj.m_pConstantBuffer.GetAddressOf());			//버퍼생성

	return hr;
}

HRESULT KG_Model::CreateInputLayout()					//레이아웃 생성
{
	HRESULT hr = S_OK;

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POINT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },						//레이 아웃에 쉐이더의 어떤 정보를 받을지 입력
		{ "TEXT",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT layoutCount = sizeof(layout) / sizeof(layout[0]);
	hr = m_obj.m_pd3dDevice->CreateInputLayout(layout, layoutCount, m_obj.m_pVertexCode->GetBufferPointer(), m_obj.m_pVertexCode->GetBufferSize(), &m_obj.m_pVertexLayout);	//레이 아웃 생성
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}


HRESULT KG_Model::LoadTexture(const TCHAR* TexFileName)		//텍스쳐 로드
{
	HRESULT hr = S_OK;
	if (TexFileName == NULL) return S_OK;
	hr = D3DX11CreateShaderResourceViewFromFile(m_obj.m_pd3dDevice, TexFileName, NULL, NULL, &m_obj.m_pSRV, NULL);

	return hr;
}

HRESULT KG_Model::LoadShader(const TCHAR* ShaderFileName, const CHAR* VSName, const CHAR* PSName)			//쉐이더 로드
{
	HRESULT hr = S_OK;
	ID3DBlob* pErrorBlob;

	hr = D3DX11CompileFromFile(ShaderFileName, NULL, NULL, VSName, "vs_5_0", 0, 0, NULL, &m_obj.m_pVertexCode, &pErrorBlob, NULL);		//정점 쉐이더 컴파일
	if (FAILED(hr))
	{
		if (pErrorBlob == nullptr)
			return hr;
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		return hr;
	}
	hr = m_obj.m_pd3dDevice->CreateVertexShader(m_obj.m_pVertexCode->GetBufferPointer(), m_obj.m_pVertexCode->GetBufferSize(), NULL, &m_obj.m_pVS);	//정점 쉐이더 생성
	if (FAILED(hr))
	{
		return hr;
	}


	hr = D3DX11CompileFromFile(ShaderFileName, NULL, NULL, PSName, "ps_5_0", 0, 0, NULL, &m_obj.m_pPixelCode, &pErrorBlob, NULL);			//픽셀 쉐이더 컴파일
	if (FAILED(hr))
	{
		if (pErrorBlob == nullptr)
			return hr;
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		return hr;
	}
	hr = m_obj.m_pd3dDevice->CreatePixelShader(m_obj.m_pPixelCode->GetBufferPointer(), m_obj.m_pPixelCode->GetBufferSize(), NULL, &m_obj.m_pPS);		//픽셀 쉐이더 생성
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

//HRESULT KG_Model::CreateComputeShader()
//{
//	HRESULT hr;
//
//	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
//#if defined( DEBUG ) || defined( _DEBUG ) 
//	dwShaderFlags |= D3DCOMPILE_DEBUG;
//#endif
//
//	const D3D_SHADER_MACRO defines[] =
//	{
//#ifdef USE_STRUCTURED_BUFFERS
//		"USE_STRUCTURED_BUFFERS", "1",
//#endif
//		NULL, NULL
//	};
//
//	// We generally prefer to use the higher CS shader profile when possible as CS 5.0 is better performance on 11-class hardware
//	LPCSTR pProfile = (m_obj.m_pd3dDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "cs_5_0" : "cs_4_0";
//
//	ID3DBlob* pErrorBlob = NULL;
//	ID3DBlob* pBlob = m_obj.m_pCSCode;
//	hr = D3DX11CompileFromFile(L"../../data/shader/BasicCompute.hlsl", defines, NULL, "main", pProfile, dwShaderFlags, NULL, NULL, &pBlob, &pErrorBlob, NULL);
//	if (FAILED(hr))
//	{
//		if (pErrorBlob)
//			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
//
//		SAFE_RELEASE(pErrorBlob);
//		SAFE_RELEASE(pBlob);
//
//		return hr;
//	}
//
//	hr = m_obj.m_pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_obj.m_pCS);
//
//	SAFE_RELEASE(pErrorBlob);
//	SAFE_RELEASE(pBlob);
//
//	return hr;
//}

void KG_Model::Convert(std::vector<PT_VERTEX>& list)
{
	for (int i = 0; i < list.size(); i++)
	{
		PT_VERTEX v = list[i];
		list[i].p.x = (v.p.x / Winrt.right) * 2.0f - 1.0f;
		list[i].p.y = ((v.p.y / Winrt.bottom) * 2.0f - 1.0f)*-1.0f;
		list[i].p.z = v.p.z;
	}
}


void KG_Model::Convert(std::vector<PNCT_VERTEX>& list)
{
	for (int i = 0; i < list.size(); i++)
	{
		PNCT_VERTEX v = list[i];
		list[i].p.x = (v.p.x / Winrt.right) * 2.0f - 1.0f;
		list[i].p.y = ((v.p.y / Winrt.bottom) * 2.0f - 1.0f)*-1.0f;
		list[i].p.z = v.p.z;
	}
}

void KG_Model::UVConvert(std::vector<PNCT_VERTEX>& vertex, RECT& Rect, int Width, int Height)
{
	vertex[0].t.x = Rect.left / (float)Width;
	vertex[0].t.y = Rect.top / (float)Height;
	vertex[1].t.x = (Rect.left + Rect.right) / (float)Width;
	vertex[1].t.y = (Rect.top) / (float)Height;

	vertex[2].t.x = (Rect.left) / (float)Width;
	vertex[2].t.y = (Rect.top + Rect.bottom) / (float)Height;

	vertex[3].t.x = (Rect.left + Rect.right) / (float)Width;
	vertex[3].t.y = (Rect.top + Rect.bottom) / (float)Height;
}

bool	KG_Model::Init()
{
	return true;
}
bool	KG_Model::Frame()
{
	return true;
}

bool	KG_Model::PreRender()
{
	m_obj.PrePender();
	return true;
}


bool	KG_Model::Render()
{
	PreRender();
	PostRender();
	return true;
}

bool	KG_Model::PostRender()
{
	m_obj.PostPender();
	return true;
}

bool	KG_Model::Release()
{
	return true;
}

KG_Model::KG_Model()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}


KG_Model::~KG_Model()
{
}


//Splite
void KG_Model::SetRectAnimation(float AnimTime, int Width, int WidthSize, int Height, int HeightSize)
{
	m_AnimTime = AnimTime;

	m_Rect.left = Width;
	m_Rect.right = WidthSize; // 텍스쳐 가로 셋 갯수 및 크기	
	m_Rect.top = Height;
	m_Rect.bottom = HeightSize; // 텍스쳐 세로 셋 갯수 및 크기

	/*if (Width*Height <= 1)
	{
		m_fSecPerRender = AnimTime / m_iNumTexture;
		return;
	}
	else
	{
		m_fSecPerRender = AnimTime / (Width*Height);
		m_iNumTexture = Width * Height;
	}*/

	for (int iHeight = 0; iHeight < m_Rect.left; iHeight++)
	{
		for (int iWidth = 0; iWidth < m_Rect.top; iWidth++)
		{
			CRectUV  Rect;
			Rect.Rect.left = iWidth * m_Rect.right;
			Rect.Rect.right = (iWidth + 1) * m_Rect.right;
			Rect.Rect.top = iHeight * m_Rect.bottom;
			Rect.Rect.bottom = (iHeight + 1)* m_Rect.bottom;

			Rect.UV = SetUV(Rect.Rect);
			m_RectList.push_back(Rect);
		}
	}
}

D3DXVECTOR4 KG_Model::SetUV(RECT& Rect)
{
	D3DXVECTOR4 UV;
	float OffSetX = 0.0f;
	if (Rect.left > 0)
	{
		OffSetX = (float)Rect.left / (float)(m_Rect.left*m_Rect.right);
	}
	float OffSetY = 0.0f;
	if (Rect.top > 0)
	{
		OffSetY = (float)Rect.top / (float)(m_Rect.top*m_Rect.bottom);
	}

	UV.x = OffSetX;
	UV.y = OffSetY;

	OffSetX = 1.0f;
	if (Rect.right > 0)
	{
		OffSetX = (float)Rect.right / (float)(m_Rect.left*m_Rect.right);
	}
	OffSetY = 1.0f;
	if (Rect.bottom > 0)
	{
		OffSetY = (float)Rect.bottom / (float)(m_Rect.top*m_Rect.bottom);
	}
	UV.z = OffSetX;
	UV.w = OffSetY;
	return UV;
}

void KG_Model::UpdatePSShader(ID3D11ShaderResourceView* textview)
{
	m_obj.m_pContext->PSSetShaderResources(0, 1, &textview);
	
}
