#include "JH_Map.h"


//툴관련 함수
namespace JH
{
	INT JH_Map::AddSplattTexture(const TCHAR* pFileName, int Num, float Alpha)
	{
		if (!this) return -1;
		int index = I_Texture.Add(m_obj.m_pd3dDevice, pFileName);
		std::map<int, CTexture*>::iterator iter;

		iter = m_vSplattTextureList.find(Num - 1);


		if (iter != m_vSplattTextureList.end())
		{
			m_vSplattTextureList.erase(iter);
		}


		m_vSplattTextureList.insert(std::make_pair(Num - 1, (I_Texture.GetPtr(index))));



		return-1;
	}
	// Base Function
	bool	JH_Map::UpdateTangentBuffer()
	{
		D3DXVECTOR3 vTangent, vBiNormal, vNormal;
		int iIndex = 0;
		int i0, i1, i2, i3, i4, i5;


		m_TangentList.resize(m_VerTex.size());
		for (int iIndex = 0; iIndex < m_obj.m_Indexnum; iIndex += 3)
		{

			i0 = m_IndexData[iIndex + 0];
			i1 = m_IndexData[iIndex + 1];
			i2 = m_IndexData[iIndex + 2];

			m_NormalMap.CreateTangentSpaceVectors(&m_VerTex[i0].p, &m_VerTex[i1].p, &m_VerTex[i2].p,
				m_VerTex[i0].t.x, m_VerTex[i0].t.y,
				m_VerTex[i1].t.x, m_VerTex[i1].t.y,
				m_VerTex[i2].t.x, m_VerTex[i2].t.y,
				&vTangent);

			m_TangentList[i0] += vTangent;

			i0 = m_IndexData[iIndex + 1];
			i1 = m_IndexData[iIndex + 2];
			i2 = m_IndexData[iIndex + 0];

			m_NormalMap.CreateTangentSpaceVectors(&m_VerTex[i0].p, &m_VerTex[i1].p, &m_VerTex[i2].p,
				m_VerTex[i0].t.x, m_VerTex[i0].t.y,
				m_VerTex[i1].t.x, m_VerTex[i1].t.y,
				m_VerTex[i2].t.x, m_VerTex[i2].t.y,
				&vTangent);

			m_TangentList[i0] += vTangent;

			i0 = m_IndexData[iIndex + 2];
			i1 = m_IndexData[iIndex + 0];
			i2 = m_IndexData[iIndex + 1];

			m_NormalMap.CreateTangentSpaceVectors(&m_VerTex[i0].p, &m_VerTex[i1].p, &m_VerTex[i2].p,
				m_VerTex[i0].t.x, m_VerTex[i0].t.y,
				m_VerTex[i1].t.x, m_VerTex[i1].t.y,
				m_VerTex[i2].t.x, m_VerTex[i2].t.y,
				&vTangent);

			m_TangentList[i0] += vTangent;

		}
		for (int i = 0; i < m_iRowNum*m_iColumNum; i++)
		{
			D3DXVec3Normalize(&m_TangentList[i0], &m_TangentList[i0]);
		}


		m_pTangentVB.Attach(CDXH::CreateVertexBuffer(m_obj.m_pd3dDevice, &m_TangentList.at(0), m_TangentList.size(), sizeof(D3DXVECTOR3)));


		return true;
	}
	float JH_Map::GetHeight(float fX, float fZ)
	{
		
		// fPosX/fPosZ의 위치에 해당하는 높이맵셀을 찾는다.
		// m_iNumCols와m_iNumRows은 가로/세로의 실제 크기값임.
		float fCellX = (float)(m_iCellCol*m_fCellDistance / 2.0f + fX);
		float fCellZ = (float)(m_iCellRow*m_fCellDistance / 2.0f - fZ);

		// 셀의 크기로 나누어 0~1 단위의 값으로 바꾸어 높이맵 배열에 접근한다.
		fCellX /= (float)m_fCellDistance;
		fCellZ /= (float)m_fCellDistance;

		// fCellX, fCellZ 값보다 작거나 같은 최대 정수( 소수부분을 잘라낸다.)
		float fVertexCol = ::floorf(fCellX);
		float fVertexRow = ::floorf(fCellZ);

		// 높이맵 범위를 벗어나면 강제로 초기화 한다.
		if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
		if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
		if ((float)(m_iColumNum - 2) < fVertexCol)	fVertexCol = (float)(m_iColumNum - 2);
		if ((float)(m_iRowNum - 2) < fVertexRow)	fVertexRow = (float)(m_iRowNum - 2);

		// 계산된 셀의 플랜을 구성하는 4개 정점의 높이값을 찾는다. 
		//  A   B
		//  *---*
		//  | / |
		//  *---*  
		//  C   D
		float A = GetHeightMap((int)fVertexRow, (int)fVertexCol);
		float B = GetHeightMap((int)fVertexRow, (int)fVertexCol + 1);
		float C = GetHeightMap((int)fVertexRow + 1, (int)fVertexCol);
		float D = GetHeightMap((int)fVertexRow + 1, (int)fVertexCol + 1);

		// A정점의 위치에서 떨어진 값(변위값)을 계산한다. 0 ~ 1.0f
		float fDeltaX = fCellX - fVertexCol;
		float fDeltaZ = fCellZ - fVertexRow;
		// 보간작업를 위한 기준 페잇스를 찾는다. 
		float fHeight = 0.0f;
		// 윗페이스를 기준으로 보간한다.
		// fDeltaZ + fDeltaX < 1.0f
		if (fDeltaZ < (1.0f - fDeltaX))  //ABC
		{
			float uy = B - A; // A->B
			float vy = C - A; // A->C	
							  // 두 정점의 높이값의 차이를 비교하여 델타X의 값에 따라 보간값을 찾는다.		
			fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
		}
		// 아래페이스를 기준으로 보간한다.
		else // DCB
		{
			float uy = C - D; // D->C
			float vy = B - D; // D->B
							  // 두 정점의 높이값의 차이를 비교하여 델타Z의 값에 따라 보간값을 찾는다.		
			fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
		}
		return fHeight;
	}

	float JH_Map::Lerp(float fStart, float fEnd, float fTangent)
	{
		return fStart - (fStart*fTangent) + (fEnd*fTangent);
	}
	float JH_Map::GetHeightMap(int iRow, int iCol)
	{
		return	m_vHeightList[iRow*m_iRowNum + iCol];
	}

	void  JH_Map::UpdateConstantBuffer(ID3D11Buffer* pConstantBuffer, void* Data)
	{
		m_obj.m_pContext->UpdateSubresource(pConstantBuffer, 0, nullptr, Data, 0, 0);
	}
	void    JH_Map::SetMatrix(D3DXMATRIX* matWorld,
		D3DXMATRIX* matView,
		D3DXMATRIX* matProj)
	{
		if (matWorld != nullptr)
		{
			m_matWorld = *matWorld;
		}
		if (matView != nullptr)
		{
			m_matView = *matView;
		}
		if (matProj != nullptr)
		{
			m_matProj = *matProj;
		}

		D3DXMatrixInverse(&m_matNormal, 0, &m_matWorld);


		D3DXMatrixTranspose(&m_ConData.World, &m_matWorld);
		D3DXMatrixTranspose(&m_ConData.View, &m_matView);
		D3DXMatrixTranspose(&m_ConData.Proj, &m_matProj);
		D3DXMatrixTranspose(&m_matNormal, &m_matNormal);

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



		m_obj.m_pContext->UpdateSubresource(
			m_obj.m_pConstantBuffer.Get(),
			0, NULL, &m_matNormal, 0, 0);

	}


	void JH_Map::SetMapDesc(
		const TCHAR* TextureFileName,
		const TCHAR* ShaderFileName,
		int ColumNum,
		int RowNum,
		float fSellDistance,
		float fScaleHeight)
	{
		m_MapDesc.iColumNum = ColumNum;
		m_MapDesc.iRowNum = RowNum;
		m_MapDesc.TextureFileName = TextureFileName;
		m_MapDesc.ShaderFileName = ShaderFileName;
		m_MapDesc.fSellDistance = fSellDistance;
		m_MapDesc.fScaleHeight = fScaleHeight;

	}

	HRESULT JH_Map::CreateHeightMap(ID3D11Device* pD3D11Device, ID3D11DeviceContext* pD3D11DeviceContext, const TCHAR* TextureFileName)
	{
		D3DX11_IMAGE_INFO imageInfo;
		D3DX11_IMAGE_LOAD_INFO LoadInfo;
		ZeroMemory(&LoadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
		LoadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		LoadInfo.Usage = D3D11_USAGE_STAGING;
		LoadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		LoadInfo.pSrcInfo = &imageInfo;
		LoadInfo.MipLevels = 1;

		ID3D11Resource* pResource = nullptr;

		HRESULT hr;
		hr = D3DX11CreateTextureFromFile(pD3D11Device, TextureFileName, &LoadInfo, NULL, &pResource, NULL);
		if (hr != S_OK)
		{
			return false;
		}

		ID3D11Texture2D* pTexture = nullptr;
		if (FAILED(pResource->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture)))
		{
			return false;
		}


		D3D11_TEXTURE2D_DESC desc;

		pTexture->GetDesc(&desc);
		m_iRowNum = desc.Height;
		m_iColumNum = desc.Width;
		m_iCellCount = 1;
		m_vHeightList.resize(desc.Height*desc.Width);

		D3D11_MAPPED_SUBRESOURCE MapResrc;

		if (SUCCEEDED(pD3D11DeviceContext->Map((ID3D11Resource*)pTexture, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, NULL, &MapResrc)))
		{
			UCHAR* pTexcell = (UCHAR*)MapResrc.pData;
			for (int iRow = 0; iRow < desc.Height; iRow++)
			{
				int irowStart = MapResrc.RowPitch*iRow;
				for (int iCol = 0; iCol < desc.Width; iCol++)
				{
					BYTE R = pTexcell[irowStart + iCol * 4];

					m_vHeightList[iRow*desc.Width + iCol] = R;

				}

			}
			pD3D11DeviceContext->Unmap((ID3D11Resource*)pTexture, D3D11CalcSubresource(0, 0, 1));
		}
		pTexture->Release();

		pResource->Release();

	}
	HRESULT  JH_Map::Load(ID3D11Device* pD3D11Device, ID3D11DeviceContext* pD3D11DeviceContext)
	{

		m_iColumNum = m_MapDesc.iColumNum;
		m_iRowNum = m_MapDesc.iRowNum;
		m_iCellRow = m_MapDesc.iRowNum - 1;
		m_iCellCol = m_MapDesc.iColumNum - 1;
		m_iVertices = m_iColumNum * m_iRowNum;
		m_TextureFileName = m_MapDesc.TextureFileName;
		m_ShaderFileName = m_MapDesc.ShaderFileName;
		m_fCellDistance = m_MapDesc.fSellDistance;
		m_fScaleHeight = m_MapDesc.fScaleHeight;





		if (!Create(pD3D11Device, pD3D11DeviceContext, m_MapDesc.ShaderFileName, m_MapDesc.TextureFileName, "VS","PS"))
		{
			return  false;
		}

		m_SkyBox = std::make_shared<KG_SkyBox>();
		m_SkyBox->CreateSkyBox(m_obj.m_pd3dDevice, m_obj.m_pContext, L"SkyObj.hlsl");
		m_SkyBox->CreateTexuture(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/sky/StarFiled2.dds");



		UpdateTangentBuffer();
		m_CBSub.Attach(CDXH::CreateConstantBuffer(m_obj.m_pd3dDevice, nullptr, sizeof(CB_SPT), 1));
		return true;
	}
	HRESULT JH_Map::CreateVertexData()
	{

		HRESULT hr = S_OK;
		m_obj.m_VertexSize = sizeof(PNCT_VERTEX);
		m_VerTex.resize(m_iVertices);

		float fHalfCols = m_iCellCol / 2.0f;
		float fHalfRows = m_iCellRow / 2.0f;
		float ftxOffsetU = 1.0f / m_iCellCol;
		float ftxOffsetV = 1.0f / m_iCellRow;

		for (int iRow = 0; iRow < m_iRowNum; iRow++)
		{
			for (int iCol = 0; iCol < m_iColumNum; iCol++)
			{

				int  iVertexIndex = iRow * m_iColumNum + iCol;

				if (m_vHeightList.size())
				{
					m_VerTex[iVertexIndex].p.y = m_vHeightList[iVertexIndex] / m_MapDesc.fScaleHeight;
				}
				else
				{
					m_VerTex[iVertexIndex].p.y = 0.0f;
				}
				m_VerTex[iVertexIndex].p.x = (iCol - fHalfCols) * m_fCellDistance;
				m_VerTex[iVertexIndex].p.z = -((iRow - fHalfRows) * m_fCellDistance);
				m_VerTex[iVertexIndex].n = D3DXVECTOR3(0, 1.0f, 0);
				m_VerTex[iVertexIndex].c = D3DXVECTOR4(1.0F, 1.0F, 1.0F, 1.0F);
				m_VerTex[iVertexIndex].t = D3DXVECTOR2(ftxOffsetU * iCol, ftxOffsetV * iRow);

			}
		}

		return true;
	}

	HRESULT JH_Map::CreateIndexData()
	{
		m_IndexData.resize(m_iCellRow*m_iCellCol * 2 * 3);

		int iCurIndex = 0;
		for (int iRow = 0; iRow < m_iCellRow; iRow++)
		{
			for (int iCol = 0; iCol < m_iCellCol; iCol++)
			{


				int iNextRow = iRow + 1;
				int iNextCol = iCol + 1;
				m_IndexData[iCurIndex + 0] = iRow * m_iColumNum + iCol;
				m_IndexData[iCurIndex + 1] = iRow * m_iColumNum + iNextCol;
				m_IndexData[iCurIndex + 2] = iNextRow * m_iColumNum + iCol;

				m_IndexData[iCurIndex + 3] = m_IndexData[iCurIndex + 2];
				m_IndexData[iCurIndex + 4] = m_IndexData[iCurIndex + 1];
				m_IndexData[iCurIndex + 5] = iNextRow * m_iColumNum + iNextCol;




				D3DXVECTOR3 vFaceNormal, E0, E1;
				int i = 0;

				E0 = m_VerTex[m_IndexData[iCurIndex + 1]].p - m_VerTex[m_IndexData[iCurIndex + 0]].p;
				E1 = m_VerTex[m_IndexData[iCurIndex + 2]].p - m_VerTex[m_IndexData[iCurIndex + 0]].p;

				D3DXVec3Cross(&vFaceNormal, &E0, &E1);


				m_VerTex[m_IndexData[iCurIndex + 0]].n += vFaceNormal;
				m_VerTex[m_IndexData[iCurIndex + 1]].n += vFaceNormal;
				m_VerTex[m_IndexData[iCurIndex + 2]].n += vFaceNormal;



				E0 = m_VerTex[m_IndexData[iCurIndex + 4]].p - m_VerTex[m_IndexData[iCurIndex + 3]].p;
				E1 = m_VerTex[m_IndexData[iCurIndex + 5]].p - m_VerTex[m_IndexData[iCurIndex + 3]].p;


				D3DXVec3Cross(&vFaceNormal, &E0, &E1);


				m_VerTex[m_IndexData[iCurIndex + 3]].n += vFaceNormal;
				m_VerTex[m_IndexData[iCurIndex + 4]].n += vFaceNormal;
				m_VerTex[m_IndexData[iCurIndex + 5]].n += vFaceNormal;


				iCurIndex += 6;
			}
		}

		D3DXVECTOR3 vLight(0, -1, 0);
		for (size_t i = 0; i < m_VerTex.size(); i++)
		{

			D3DXVec3Normalize(&m_VerTex[i].n, &m_VerTex[i].n);
			/*	float fDot = D3DXVec3Dot(&-vLight, &m_VerTex[i].n);



				m_VerTex[i].c.x = fDot;
				m_VerTex[i].c.y = fDot;
				m_VerTex[i].c.z = fDot;
				m_VerTex[i].c.w = 1.0f;*/
		}





		return true;
	}
	HRESULT JH_Map::CreateInputLayout()
	{
		HRESULT hr = S_OK;
		const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEX",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{"TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,1,0,D3D11_INPUT_PER_VERTEX_DATA,0},
			// 가중치 인덱스 
			/*{ "TEX",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEX",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 64,  D3D11_INPUT_PER_VERTEX_DATA, 0 },*/
		};
		UINT iElementCount = sizeof(layout) /
			sizeof(layout[0]);
		m_obj.m_pd3dDevice->CreateInputLayout(
			layout,
			iElementCount,
			m_obj.m_pVertexCode->GetBufferPointer(),
			m_obj.m_pVertexCode->GetBufferSize(),
			&m_obj.m_pVertexLayout);
		return hr;
	}
	HRESULT	JH_Map::LoadTexture(const TCHAR* pszTexFileName)
	{
		HRESULT hr = S_OK;

		if (pszTexFileName == NULL) return S_OK;
		hr = D3DX11CreateShaderResourceViewFromFile(m_obj.m_pd3dDevice, pszTexFileName, NULL, NULL, &m_obj.m_pSRV, NULL);

		if (m_pNormMapFileName)
		{
			m_iTexNum = I_Texture.Add(m_obj.m_pd3dDevice, m_pNormMapFileName);
		}
		m_pTexture = I_Texture.GetPtr(m_iTexNum);

		return hr;
	}

	bool JH_Map::Frame()
	{


		return true;
	}
	bool JH_Map::RenderSet(KG_Camera* pCamera)
	{
		SetMatrix(nullptr, &pCamera->m_View, &pCamera->m_Proj);
		ID3D11Buffer*               pBuffers[3];
		pBuffers[0] = I_LIGHT_MGR.m_pLightConstantBuffer[0].Get();
		LightConstantBuffer mcb = I_LIGHT_MGR.m_cbLight;
		UpdateConstantBuffer(I_LIGHT_MGR.m_pLightConstantBuffer[0].Get(), &I_LIGHT_MGR.m_cbLight);
		m_obj.m_pContext->PSSetConstantBuffers(1, 1, pBuffers);
		m_obj.m_pContext->PSSetConstantBuffers(1, 1, pBuffers);
		UINT offset = 0;
		UINT stride = sizeof(D3DXVECTOR3);
		if (m_pNormMapFileName)
		{
			m_obj.m_pContext->PSSetShaderResources(1, 1, &m_pTexture->m_pTextureRV);
			m_obj.m_pContext->IASetVertexBuffers(1, 1, &m_pTangentVB, &stride, &offset);
		}
		I_LIGHT_MGR.Render();
		return true;
	}
	bool JH_Map::Release()
	{
		m_vHeightList.clear();
		m_vSplattTextureList.clear();
		return true;
	}
	JH_Map::JH_Map()
	{
		D3DXMatrixIdentity(&m_matNormal);
		m_pNormMapFileName = nullptr;
		m_iTexNum = -1;
		m_bMapEdit = false;
	}
	JH_Map::~JH_Map()
	{
		Release();
	}
}