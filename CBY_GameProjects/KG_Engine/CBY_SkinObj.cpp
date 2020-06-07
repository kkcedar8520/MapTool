#include "CBY_SkinObj.h"

namespace CBY
{
	bool CBY_SkinObj::SkinCreate()
	{
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
	}

	HRESULT CBY_SkinObj::CreateVertexBuffer()
	{
		HRESULT hr = S_OK;
		if (m_VertexDataSkin.size() <= 0) return E_FAIL;
		m_obj.m_Vertexnum = m_VertexDataSkin.size();
		D3D11_BUFFER_DESC pDesc;
		ZeroMemory(&pDesc, sizeof(D3D11_BUFFER_DESC));
		pDesc.Usage = D3D11_USAGE_DEFAULT;
		pDesc.ByteWidth = m_obj.m_VertexSize * m_VertexDataSkin.size();
		pDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA pInitialData;
		ZeroMemory(&pInitialData,
			sizeof(D3D11_SUBRESOURCE_DATA));
		pInitialData.pSysMem = &m_VertexDataSkin.at(0);

		hr = m_obj.m_pd3dDevice->CreateBuffer(&pDesc, &pInitialData, m_obj.m_pVertexBuffer.GetAddressOf());
		return hr;
	}

	void    CBY_SkinObj::ConvertSkin(std::vector<PNCT2IW_VERTEX>& list)
	{
		int iBaseVertex = 0;
		int iBaseIndex = 0;
		int iBaseOffset = 0;

		for (int iObj = 0; iObj < m_Loader.m_ObjList.size(); iObj++)
		{
			CBY_MeshSkin* mesh = m_Loader.m_ObjList[iObj];
			mesh->m_iParentIndex = m_Loader.GetParentIndex(mesh->m_szParentName);

			D3DXMatrixInverse(&mesh->m_matInverse, NULL, &mesh->m_matWorld);

			D3DXMATRIX matInvParent, matAnim;
			D3DXMatrixIdentity(&matInvParent);
			if (mesh->m_iParentIndex >= 0)
			{
				mesh->m_Parent = m_Loader.m_ObjList[mesh->m_iParentIndex];
				m_Loader.m_ObjList[mesh->m_iParentIndex]->m_Child.push_back(mesh);
				D3DXMatrixInverse(&matInvParent, NULL,
					&m_Loader.m_ObjList[mesh->m_iParentIndex]->m_matWorld);
			}
			matAnim = mesh->m_matWorld * matInvParent;
			D3DXVECTOR3 vPos, vScale;
			D3DXQUATERNION qR;
			D3DXMatrixDecompose(&vScale, &qR, &vPos, &matAnim);
			mesh->m_qAnimRotation = qR;
			mesh->m_vAnimPos = vPos;
			mesh->m_vAnimScale = vScale;
			D3DXQuaternionIdentity(&mesh->m_qAnimScaleRotation);

			int iRef = mesh->m_iTexIndex;
			if (iRef >= 0)
			{
				mesh->subMeshSkin.resize(
					m_Loader.m_MtlList[iRef].SubMaterial.size());
			}


			if (mesh->subMeshSkin.size() > 0)
			{
				for (int iSubMesh = 0; iSubMesh < mesh->subMeshSkin.size(); iSubMesh++)
				{
					if (iRef >= 0)
					{
						mesh->subMeshSkin[iSubMesh].m_pTexture = nullptr;
						mesh->subMeshSkin[iSubMesh].m_pNormalTexture = nullptr;
						mesh->subMeshSkin[iSubMesh].m_pSpecTexture = nullptr;
						mesh->subMeshSkin[iSubMesh].m_pEmsTexture = nullptr;

						for (int i = 0; i < m_Loader.m_MtlList[iRef].SubMaterial[iSubMesh].texList.size(); i++)
						{
							mesh->subMeshSkin[iSubMesh].m_iTexIndex =
								I_Texture.Add(m_obj.m_pd3dDevice,
									m_Loader.m_MtlList[iRef].SubMaterial[iSubMesh].texList[i].szTextureName,
									L"../../data/obj/");

							switch (mesh->subMeshSkin[iSubMesh].m_iTexsize)
							{
							case 0:
							{
								mesh->subMeshSkin[iSubMesh].m_pTexture =
									I_Texture.GetPtr(mesh->subMeshSkin[iSubMesh].m_iTexIndex);
							}
							break;

							case 1:
							{
								mesh->subMeshSkin[iSubMesh].m_pNormalTexture =
									I_Texture.GetPtr(mesh->subMeshSkin[iSubMesh].m_iTexIndex);
							}
							break;

							case 2:
							{
								mesh->subMeshSkin[iSubMesh].m_pSpecTexture =
									I_Texture.GetPtr(mesh->subMeshSkin[iSubMesh].m_iTexIndex);
							}
							break;

							case 3:
							{
								mesh->subMeshSkin[iSubMesh].m_pEmsTexture =
									I_Texture.GetPtr(mesh->subMeshSkin[iSubMesh].m_iTexIndex);
							}
							break;
							}
							mesh->subMeshSkin[iSubMesh].m_iTexsize++;


						}
					}

					mesh->subMeshSkin[iSubMesh].m_iNumVertex =
						mesh->subMeshSkin[iSubMesh].listSkin.size();

					CreateVIData(&mesh->subMeshSkin[iSubMesh]);
					VertexDataTan(&mesh->subMeshSkin[iSubMesh]);

					mesh->subMeshSkin[iSubMesh].m_iBaseIndex = iBaseIndex;
					mesh->subMeshSkin[iSubMesh].m_iBaseVertex = iBaseVertex;

					mesh->subMeshSkin[iSubMesh].m_iNumVertex = mesh->subMeshSkin[iSubMesh].vblistSkin.size();
					mesh->subMeshSkin[iSubMesh].m_iNumIndex = mesh->subMeshSkin[iSubMesh].iblistSkin.size();

					iBaseVertex += mesh->subMeshSkin[iSubMesh].m_iNumVertex;
					iBaseIndex += mesh->subMeshSkin[iSubMesh].m_iNumIndex;


					std::copy(mesh->subMeshSkin[iSubMesh].vblistSkin.begin(),
						mesh->subMeshSkin[iSubMesh].vblistSkin.end(),
						back_inserter(list));

					std::copy(mesh->subMeshSkin[iSubMesh].iblistSkin.begin(),
						mesh->subMeshSkin[iSubMesh].iblistSkin.end(),
						back_inserter(m_IndexData));
				}
			}
			else
			{
				if (iRef >= 0)
				{

					mesh->m_pTexture = nullptr;
					mesh->m_pNormalTexture = nullptr;
					mesh->m_pSpecTexture = nullptr;
					mesh->m_pEmsTexture = nullptr;

					for (int i = 0; i < m_Loader.m_MtlList[iRef].texList.size(); i++)
					{
						mesh->m_iTexIndex =
							I_Texture.Add(m_obj.m_pd3dDevice,
								m_Loader.m_MtlList[iRef].texList[i].szTextureName,
								L"../../data/obj/");

						switch (mesh->m_iTexsize)
						{
						case 0:
						{
							mesh->m_pTexture = I_Texture.GetPtr(mesh->m_iTexIndex);
						}
						break;

						case 1:
						{
							mesh->m_pNormalTexture = I_Texture.GetPtr(mesh->m_iTexIndex);
						}
						break;

						case 2:
						{
							mesh->m_pSpecTexture = I_Texture.GetPtr(mesh->m_iTexIndex);
						}
						break;

						case 3:
						{
							mesh->m_pEmsTexture = I_Texture.GetPtr(mesh->m_iTexIndex);
						}
						break;
						}

						mesh->m_iTexsize++;
					}
		
				}

				CreateVIData(mesh);
				VertexDataTan(mesh);


				mesh->m_iBaseVertex = iBaseVertex;
				mesh->m_iBaseIndex = iBaseIndex;

				mesh->m_iNumVertex = mesh->vblistSkin.size();
				mesh->m_iNumIndex = mesh->iblistSkin.size();

				iBaseVertex += mesh->m_iNumVertex;
				iBaseIndex += mesh->m_iNumIndex;


				std::copy(mesh->vblistSkin.begin(), mesh->vblistSkin.end(), back_inserter(list));
				std::copy(mesh->iblistSkin.begin(), mesh->iblistSkin.end(), back_inserter(m_IndexData));
			}
		}
		m_ObjList.swap(m_Loader.m_ObjList);
		m_matBoneBindPoss.swap(m_Loader.m_matBoneBindPoss);
		m_Loader.Release();
	}
	HRESULT CBY_SkinObj::CreateVertexData()
	{
		HRESULT hr = S_OK;
		m_obj.m_VertexSize = sizeof(PNCT2IW_VERTEX);

		return hr;
	}
	HRESULT CBY_SkinObj::CreateIndexData()
	{
		HRESULT hr = S_OK;

		return hr;
	}
	bool	CBY_SkinObj::Load(T_STR loadfile, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
	{
		m_obj.m_pd3dDevice = pd3dDevice;
		m_obj.m_pContext = pContext;


		if (!m_Loader.Load(loadfile.c_str()))
		{
			return false;
		}

		ConvertSkin(m_VertexDataSkin);
		SkinCreate();
		return true;
	}

	void	CBY_SkinObj::Draw(CBY_MeshSkin* mesh)
	{
		if (mesh->m_iNumIndex <= 0)
		{
			return;
		}

		if (mesh->m_pTexture != nullptr)
		{
			m_obj.m_pContext->PSSetShaderResources(0, 1,
				&mesh->m_pTexture->m_pTextureRV);
			if (mesh->m_pNormalTexture != nullptr)
				m_obj.m_pContext->PSSetShaderResources(1, 1,
					&mesh->m_pNormalTexture->m_pTextureRV);
			if (mesh->m_pSpecTexture != nullptr)
				m_obj.m_pContext->PSSetShaderResources(2, 1,
					&mesh->m_pSpecTexture->m_pTextureRV);
			if (mesh->m_pEmsTexture != nullptr)
				m_obj.m_pContext->PSSetShaderResources(3, 1,
					&mesh->m_pEmsTexture->m_pTextureRV);
		}
		if (mesh->m_iNumIndex > 0)
		{
			m_obj.m_pContext->DrawIndexed(
				mesh->m_iNumIndex,
				mesh->m_iBaseIndex,
				mesh->m_iBaseVertex);
		}
		else
		{
			m_obj.m_pContext->Draw(
				mesh->m_iNumVertex,
				mesh->m_iBaseVertex);
		}
	}
	bool    CBY_SkinObj::Render()
	{
		PrePender();
		for (int iMesh = 0; iMesh < m_ObjList.size(); iMesh++)
		{
			int iSubMesh = m_ObjList[iMesh]->subMeshSkin.size();
			if (iSubMesh > 0)
			{
				for (int iSub = 0; iSub < iSubMesh; iSub++)
				{
					Draw(&m_ObjList[iMesh]->subMeshSkin[iSub]);
				}
			}
			else
			{
				Draw(m_ObjList[iMesh]);
			}
		}
		return true;
	}
	bool    CBY_SkinObj::Release()
	{
		KG_Model::Release();
		for (int i = 0; i < m_ObjList.size(); i++)
		{
			SAFE_DEL(m_ObjList[i]);
		}
		return true;
	}
	void	CBY_SkinObj::CreateVIData(CBY_MeshSkin* mesh)
	{
		HRESULT hr = S_OK;
		mesh->iblistSkin.clear();
		if (mesh->listSkin.size() <= 0)
		{
			return;
		}
		mesh->vblistSkin.reserve(mesh->listSkin.size());
		for (int iVertex = 0; iVertex < mesh->listSkin.size(); iVertex++)
		{
			int iPos = -1;
			for (int i = 0; i < mesh->vblistSkin.size(); i++)
			{
				if (mesh->vblistSkin[i] == mesh->listSkin[iVertex])
				{
					iPos = i;
					break;
				}
			}
			if (iPos < 0)
			{
				mesh->vblistSkin.push_back(mesh->listSkin[iVertex]);
				iPos = mesh->vblistSkin.size() - 1;
			}
			mesh->iblistSkin.push_back(iPos);
		}
	}

	void CBY_SkinObj::VertexDataTan(CBY_MeshSkin* mesh)
	{
		D3DXVECTOR3 vTangent, vBiNormal, vNormal;

		int iIndex = 0;
		int i0, i1, i2, i3, i4, i5;

		for (int iIndex = 0; iIndex < mesh->iblistSkin.size(); iIndex += 3)
		{
			i0 = mesh->iblistSkin[iIndex + 0];
			i1 = mesh->iblistSkin[iIndex + 1];
			i2 = mesh->iblistSkin[iIndex + 2];

			CreateTangentSpaceVectors(&mesh->vblistSkin[i0].p, &mesh->vblistSkin[i1].p, &mesh->vblistSkin[i2].p,
				mesh->vblistSkin[i0].t, mesh->vblistSkin[i1].t, mesh->vblistSkin[i2].t,
				&vTangent, &vBiNormal, &mesh->vblistSkin[i0].n);

			mesh->vblistSkin[i0].tan += vTangent;

			i0 = mesh->iblistSkin[iIndex + 1];
			i1 = mesh->iblistSkin[iIndex + 2];
			i2 = mesh->iblistSkin[iIndex + 0];

			CreateTangentSpaceVectors(&mesh->vblistSkin[i0].p, &mesh->vblistSkin[i1].p, &mesh->vblistSkin[i2].p,
				mesh->vblistSkin[i0].t, mesh->vblistSkin[i1].t, mesh->vblistSkin[i2].t,
				&vTangent, &vBiNormal, &mesh->vblistSkin[i0].n);

			mesh->vblistSkin[i0].tan += vTangent;

			i0 = mesh->iblistSkin[iIndex + 2];
			i1 = mesh->iblistSkin[iIndex + 0];
			i2 = mesh->iblistSkin[iIndex + 1];

			CreateTangentSpaceVectors(&mesh->vblistSkin[i0].p, &mesh->vblistSkin[i1].p, &mesh->vblistSkin[i2].p,
				mesh->vblistSkin[i0].t, mesh->vblistSkin[i1].t, mesh->vblistSkin[i2].t,
				&vTangent, &vBiNormal, &mesh->vblistSkin[i0].n);

			mesh->vblistSkin[i0].tan += vTangent;

		}
		for (int i = 0; i < mesh->vblistSkin.size(); i++)
		{
			D3DXVec3Normalize(&mesh->vblistSkin[i].tan, &mesh->vblistSkin[i].tan);
		}
	}

	void CBY_SkinObj::CreateTangentSpaceVectors(D3DXVECTOR3 *v0,
		D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR2 uv0,
		D3DXVECTOR2 uv1, D3DXVECTOR2 uv2,
		D3DXVECTOR3 *vTangent, D3DXVECTOR3 *vBiNormal, D3DXVECTOR3 *vNormal)
	{
		D3DXVECTOR3 vEdge1 = *v1 - *v0;
		D3DXVECTOR3 vEdge2 = *v2 - *v0;
		D3DXVec3Normalize(&vEdge1, &vEdge1);
		D3DXVec3Normalize(&vEdge2, &vEdge2);
		// UV delta
		D3DXVECTOR2 deltaUV1 = uv1 - uv0;
		D3DXVECTOR2 deltaUV2 = uv2 - uv0;
		D3DXVec2Normalize(&deltaUV1, &deltaUV1);
		D3DXVec2Normalize(&deltaUV2, &deltaUV2);


		D3DXVECTOR3 biNormal;
		float fDet = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		if (fabsf(fDet) < 1e-6f)
		{
			*vTangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			biNormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		}
		else
		{
			*vTangent = (vEdge1 * deltaUV2.y - vEdge2 * deltaUV1.y)*fDet;
			biNormal = (vEdge2 * deltaUV1.x - vEdge1 * deltaUV2.x)*fDet;
		}
		D3DXVec3Normalize(vTangent, vTangent);
		D3DXVec3Normalize(&biNormal, &biNormal);

		D3DXVec3Cross(vBiNormal, vNormal, vTangent);
		float crossinv = (D3DXVec3Dot(vBiNormal, &biNormal) < 0.0f) ? -1.0f : 1.0f;
		*vBiNormal *= crossinv;
	}

	void CBY_SkinObj::PrePender()
	{
		UINT offset = 0;
		UINT stride = m_obj.m_VertexSize;
		m_obj.m_pContext->IASetVertexBuffers(0, 1, m_obj.m_pVertexBuffer.GetAddressOf(), &stride, &offset);			//vertexbuffer를 셋팅, 정점버퍼 시작슬롯, 갯수, 주소, 배열의 주소, 오프셋 배열거리
		m_obj.m_pContext->IASetIndexBuffer(m_obj.m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);			//인덱스 버퍼 셋팅 인덱스 버퍼는 여러 버퍼를 넘기는게 아니라 버퍼s가 아니고 주소를 보낼 필요도 없다

	}

	CBY_SkinObj::CBY_SkinObj()
	{
	}


	CBY_SkinObj::~CBY_SkinObj()
	{
	}
}
