#include "CBY_Obj.h"

namespace CBY
{
	HRESULT CBY_Obj::ObjCreate()
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


	HRESULT CBY_Obj::CreateVertexBuffer()
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

	HRESULT CBY_Obj::CreateConstantBuffer()
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
		m_obj.iCBBufferType = CB_DEFAULT;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
		InitData.pSysMem = &m_ConData;							//초기화 데이터에 대한 포인터

		m_obj.m_pConstantBuffer.Attach(CDXH::CreateConstantBuffer(m_obj.m_pd3dDevice, nullptr, 1, sizeof(CONDATA)));

		return hr;
	}
	void    CBY_Obj::Convert(std::vector<PNCT2IW_VERTEX>& list)
	{
		int iBaseVertex = 0;
		int iBaseIndex = 0;
		int iBaseOffset = 0;

		for (int iObj = 0; iObj < m_Loader.m_ObjList.size(); iObj++)
		{
			CBY_MeshSkin* mesh = m_Loader.m_ObjList[iObj];

			mesh->m_iParentIndex = m_Loader.GetParentIndex(mesh->m_szParentName);


			D3DXMATRIX matInvParent, matAnim;
			D3DXMatrixIdentity(&matInvParent);
			if (mesh->m_iParentIndex >= 0)
			{
				mesh->m_Parent = m_Loader.m_ObjList[mesh->m_iParentIndex];
				m_Loader.m_ObjList[mesh->m_iParentIndex]->m_Child.push_back(mesh);
				D3DXMatrixInverse(&matInvParent, NULL, &m_Loader.m_ObjList[mesh->m_iParentIndex]->m_matWorld);
			}
			matAnim = mesh->m_matWorld * matInvParent;
			D3DXMatrixDecompose(&mesh->m_vAnimScale, &mesh->m_qAnimRotation, &mesh->m_vAnimPos, &matAnim);
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
						if (m_Loader.m_MtlList[iRef].SubMaterial[iSubMesh].texList.size() > 0)
						{
							mesh->subMeshSkin[iSubMesh].m_iTexIndex =
								I_Texture.Add(m_obj.m_pd3dDevice,
									m_Loader.m_MtlList[iRef].SubMaterial[iSubMesh].texList[0].szTextureName,
									L"../../data/obj/");
							mesh->subMeshSkin[iSubMesh].m_pTexture =
								I_Texture.GetPtr(mesh->subMeshSkin[iSubMesh].m_iTexIndex);
						}
					}

					mesh->subMeshSkin[iSubMesh].m_iNumVertex =
						mesh->subMeshSkin[iSubMesh].listSkin.size();

					CreateVIData(&mesh->subMeshSkin[iSubMesh]);

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
					mesh->m_iTexIndex =
						I_Texture.Add(m_obj.m_pd3dDevice,
							m_Loader.m_MtlList[iRef].texList[0].szTextureName,
							L"../../data/obj/");
					mesh->m_pTexture = I_Texture.GetPtr(mesh->m_iTexIndex);
				}

				CreateVIData(mesh);


				mesh->m_iBaseVertex = iBaseVertex;
				mesh->m_iBaseIndex = iBaseIndex;

				mesh->m_iNumVertex = mesh->vblistSkin.size();
				mesh->m_iNumIndex = mesh->iblistSkin.size();

				iBaseVertex += mesh->m_iNumVertex;
				iBaseIndex += mesh->m_iNumIndex;


				std::copy(mesh->vblistSkin.begin(), mesh->vblistSkin.end(), back_inserter(list));
				std::copy(mesh->iblistSkin.begin(), mesh->iblistSkin.end(), back_inserter(m_IndexData));
			}

			if (mesh->m_bRoot)
			{
				m_iRootList.push_back(iObj);
			}

			if (mesh->m_bBone)
			{
				mesh->m_BoneBox.SetBox(mesh->m_Bone.m_Box.vMin, mesh->m_Bone.m_Box.vMax);
				mesh->m_BoneBox.Create(m_obj.m_pd3dDevice, m_obj.m_pContext);
			}
		}
		m_ObjectList.swap(m_Loader.m_ObjList);
		m_Scene = m_Loader.m_Scene;
		m_matBoneBindPoss.swap(m_Loader.m_matBoneBindPoss);
		m_Loader.Release();
	}

	HRESULT CBY_Obj::CreateVertexData()
	{
		HRESULT hr = S_OK;
		m_obj.m_VertexSize = sizeof(PNCT2IW_VERTEX);

		return hr;
	}
	HRESULT CBY_Obj::CreateIndexData()
	{
		HRESULT hr = S_OK;

		return hr;
	}
	bool	CBY_Obj::Load(
		T_STR loadfile,
		ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext)
	{
		m_obj.m_pd3dDevice = pd3dDevice;
		m_obj.m_pContext = pContext;
		if (!m_Loader.Load(loadfile.c_str()))
		{
			return false;
		}

		Convert(m_VertexDataSkin);

		ObjCreate();

		return true;
	}

	void CBY_Obj::ObjUpdate(D3DMATRIX* parmat, D3DMATRIX& matcal)
	{
		float fTime = 0;
		fTime = 1.0f * g_SecondTime * m_Scene.iTickPerFrame * m_Scene.iFrameSpeed;

		if (fTime >= m_Scene.iTickPerFrame)
		{
			fTime = m_Scene.iTickPerFrame;
		}

		m_fElapseTick += fTime;

		if (m_fElapseTick >= m_Scene.iLastFrame * m_Scene.iTickPerFrame)
		{
			m_fElapseTick = 0.0f;
		}

		CMatSetData matdata;

		for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
		{
			CMatSetData matdata(m_ObjectList[iObj]->m_vAnimPos, m_ObjectList[iObj]->m_vAnimScale,
				m_ObjectList[iObj]->m_qAnimRotation, m_ObjectList[iObj]->m_qAnimScaleRotation);
			CAnimationTrack start;
			start.iTick = 0;
			start.p = matdata.vPos;
			start.q = matdata.qR;

			if (m_ObjectList[iObj]->posTrack.size() > 0)
			{
				AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->posTrack, ANI_POS);
			}

			if (m_ObjectList[iObj]->rotTrack.size() > 0)
			{
				AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->rotTrack, ANI_ROT);
			}

			if (m_ObjectList[iObj]->sclTrack.size() > 0)
			{
				AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->sclTrack, ANI_SCL);
			}
			m_ObjectList[iObj]->m_matCalculation = matdata.matScale*matdata.matRotation;
			m_ObjectList[iObj]->m_matCalculation._41 = matdata.vPos.x;
			m_ObjectList[iObj]->m_matCalculation._42 = matdata.vPos.y;
			m_ObjectList[iObj]->m_matCalculation._43 = matdata.vPos.z;

			if (m_ObjectList[iObj]->m_Parent != nullptr)
			{
				//D3DXMATRIX matParent = m_ObjectList[iObj]->m_Parent->m_matCalculationBegin;
				//m_ObjectList[iObj]->m_matCalculation *= matParent;
			}
			else
			{
				if (parmat != nullptr)
				{
					m_ObjectList[iObj]->m_matCalculation = m_matBoneBindPoss[iObj] * m_ObjectList[iObj]->m_matCalculation *
						*parmat;
					//m_ObjectList[iObj]->m_matCalculation *= *parmat;
				}
			}
			matcal = m_ObjectList[iObj]->m_matCalculation;
			//m_ObjectList[iObj]->m_matCalculation = m_matBoneBindPoss[iObj] * m_ObjectList[iObj]->m_matCalculation;
		}
	}


	bool    CBY_Obj::Frame()
	{
		float fTime = 0;
		fTime = 1.0f * g_SecondTime * m_Scene.iTickPerFrame * m_Scene.iFrameSpeed;

		if (fTime >= m_Scene.iTickPerFrame)
		{
			fTime = m_Scene.iTickPerFrame;
		}

		m_fElapseTick += fTime;

		if (m_fElapseTick >= m_Scene.iLastFrame * m_Scene.iTickPerFrame)
		{
			m_fElapseTick = 0.0f;
		}

		CMatSetData matdata;

		for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
		{
			CMatSetData matdata(m_ObjectList[iObj]->m_vAnimPos, m_ObjectList[iObj]->m_vAnimScale,
				m_ObjectList[iObj]->m_qAnimRotation, m_ObjectList[iObj]->m_qAnimScaleRotation);
			CAnimationTrack start;
			start.iTick = 0;
			start.p = matdata.vPos;
			start.q = matdata.qR;

			if (m_ObjectList[iObj]->posTrack.size() > 0)
			{
				AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->posTrack, ANI_POS);
			}

			if (m_ObjectList[iObj]->rotTrack.size() > 0)
			{
				AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->rotTrack, ANI_ROT);
			}

			if (m_ObjectList[iObj]->sclTrack.size() > 0)
			{
				AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->sclTrack, ANI_SCL);
			}
			m_ObjectList[iObj]->m_matCalculation = matdata.matScale * matdata.matRotation;
			m_ObjectList[iObj]->m_matCalculation._41 = matdata.vPos.x;
			m_ObjectList[iObj]->m_matCalculation._42 = matdata.vPos.y;
			m_ObjectList[iObj]->m_matCalculation._43 = matdata.vPos.z;

			if (m_ObjectList[iObj]->m_Parent != nullptr)
			{
				D3DXMATRIX matParent = m_ObjectList[iObj]->m_Parent->m_matCalculationBegin;
				m_ObjectList[iObj]->m_matCalculation *= matParent;
			}

			m_ObjectList[iObj]->m_matCalculation = m_matBoneBindPoss[iObj] * m_ObjectList[iObj]->m_matCalculation;
		}

		return true;
	}

	bool CBY_Obj::AniTrackSet(CMatSetData& matdata, CAnimationTrack start, int iobj, std::vector<CAnimationTrack>& AniTrack, int tracktype)
	{
		CAnimationTrack aniStart = start;
		CAnimationTrack aniEnd;
		for (int tick = 0; tick < AniTrack.size(); tick++)
		{
			if (AniTrack[tick].iTick <= m_fElapseTick)
			{
				aniStart = AniTrack[tick];
			}
			if (AniTrack[tick].iTick > m_fElapseTick)
			{
				aniEnd = AniTrack[tick];
				break;
			}
			if (tick == AniTrack.size() - 1)
			{
				aniEnd = start;
			}
		}
		float step = aniEnd.iTick - aniStart.iTick;
		float t = 0.0f;
		if (step > 0.0f)
		{
			t = (m_fElapseTick - aniStart.iTick) / step;
		}
		switch (tracktype)
		{
		case ANI_POS:
		{
			D3DXVec3Lerp(&matdata.vPos, &aniStart.p, &aniEnd.p, t);
		}
		break;

		case ANI_ROT:
		{
			D3DXQuaternionSlerp(&matdata.qR, &aniStart.q, &aniEnd.q, t);
			D3DXMatrixRotationQuaternion(&matdata.matRotation, &matdata.qR);
		}
		break;

		case ANI_SCL:
		{
			D3DXQuaternionSlerp(&matdata.qScale, &aniStart.q, &aniEnd.q, t);
			D3DXMatrixRotationQuaternion(&matdata.matScaleRot, &matdata.qScale);
			D3DXMatrixInverse(&matdata.matScaleInverRot, NULL, &matdata.matScaleRot);

			D3DXVec3Lerp(&matdata.vScale, &aniStart.p, &aniEnd.p, t);
			D3DXMatrixScaling(&matdata.matScale, matdata.vScale.x, matdata.vScale.y, matdata.vScale.z);
			matdata.matScale = matdata.matScaleInverRot * matdata.matScale * matdata.matScaleRot;
		}
		break;
		}
		return true;
	}
	void CBY_Obj::Draw(CBY_MeshSkin* mesh)
	{
		if (mesh->m_pTexture != nullptr)
		{
			m_obj.m_pContext->PSSetShaderResources(0, 1, &mesh->m_pTexture->m_pTextureRV);
		}
		if (mesh->m_iNumIndex > 0)
		{
			m_obj.m_pContext->DrawIndexed(mesh->m_iNumIndex, mesh->m_iBaseIndex, mesh->m_iBaseVertex);
		}
		else
		{
			m_obj.m_pContext->Draw(mesh->m_iNumVertex, mesh->m_iBaseVertex);
		}
	}

	bool    CBY_Obj::Render()
	{
		PreRender();
		for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
		{

			if (m_ObjectList[iObj]->subMeshSkin.size() > 0)
			{
				for (int iSub = 0; iSub < m_ObjectList[iObj]->subMeshSkin.size(); iSub++)
				{
					Draw(&m_ObjectList[iObj]->subMeshSkin[iSub]);
				}
			}
			else
			{
				Draw(m_ObjectList[iObj]);
			}
		}
		return true;
	}

	bool    CBY_Obj::Release()
	{
		KG_Model::Release();
		/*for (int i = 0; i < m_ObjectList.size(); i++)
		{
			SAFE_DEL(m_ObjectList[i]);
		}*/
		return true;
	}
	void	CBY_Obj::CreateVIData(CBY_MeshSkin* mesh)
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

	bool CBY_Obj::PreRender()
	{
		UINT offset = 0;
		UINT stride = m_obj.m_VertexSize;
		m_obj.m_pContext->IASetVertexBuffers(0, 1, m_obj.m_pVertexBuffer.GetAddressOf(), &stride, &offset);			//vertexbuffer를 셋팅, 정점버퍼 시작슬롯, 갯수, 주소, 배열의 주소, 오프셋 배열거리
		m_obj.m_pContext->IASetIndexBuffer(m_obj.m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);			//인덱스 버퍼 셋팅 인덱스 버퍼는 여러 버퍼를 넘기는게 아니라 버퍼s가 아니고 주소를 보낼 필요도 없다

		return true;
	}

	CBY_Obj::CBY_Obj()
	{
		m_fElapseTick = 0.0f;
	}


	CBY_Obj::~CBY_Obj()
	{
	}
}
