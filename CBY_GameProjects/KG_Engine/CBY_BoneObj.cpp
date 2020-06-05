#include "CBY_BoneObj.h"
namespace CBY
{
bool CBY_BoneObj::BoneCreate()
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

bool	CBY_BoneObj::Load(T_STR loadfile, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	m_obj.m_pd3dDevice = pd3dDevice;
	m_obj.m_pContext = pContext;
	if (!m_ObjLoader.Load(loadfile.c_str()))
	{
		return false;
	}

	Convert(m_VertexDataSkin);

	//BoneCreate();

	return true;
}

void CBY_BoneObj::Update(int iStart, int iEnd, float fTime, D3DXMATRIX* pMatrixList)
{
	CMatSetData matdata;
	float Start = iStart * m_Scene.iTickPerFrame;// * m_Loader.m_Scene.iFrameSpeed;
	float afTime = 0;
	m_bAniEnd = false;
	m_bAniStart = false;

	if (m_fElapseTick == 0.0f)
	{
		m_bAniStart = true;
	}

	afTime = 1.0f*g_SecondTime * m_Scene.iTickPerFrame * m_Scene.iFrameSpeed;
	if (afTime >= m_Scene.iTickPerFrame)
	{
		afTime = m_Scene.iTickPerFrame;
	}
	m_fElapseTick += afTime;
	Start += m_fElapseTick;

	if (Start >= iEnd * m_Scene.iTickPerFrame)
	{
		m_bAniEnd = true;
		Start -= iStart;
		m_fElapseTick = 0;
	}

	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		CMatSetData matdata(m_ObjectList[iObj]->m_vAnimPos, m_ObjectList[iObj]->m_vAnimScale,
			m_ObjectList[iObj]->m_qAnimRotation, m_ObjectList[iObj]->m_qAnimScaleRotation);
		CAnimationTrack start;
		start.iTick = Start;
		start.p = matdata.vPos;
		start.q = matdata.qR;

		if (m_ObjectList[iObj]->posTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->posTrack, ANI_POS, Start);
		}

		if (m_ObjectList[iObj]->rotTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->rotTrack, ANI_ROT, Start);
		}

		if (m_ObjectList[iObj]->sclTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->sclTrack, ANI_SCL, Start);
		}
		m_ObjectList[iObj]->m_matCalculation = matdata.matScale * matdata.matRotation;
		m_ObjectList[iObj]->m_matCalculation._41 = matdata.vPos.x;
		m_ObjectList[iObj]->m_matCalculation._42 = matdata.vPos.y;
		m_ObjectList[iObj]->m_matCalculation._43 = matdata.vPos.z;


		if (m_ObjectList[iObj]->m_Parent != nullptr)
		{
			D3DXMATRIX matParent = m_ObjectList[iObj]->m_Parent->m_matCalculation;
			m_ObjectList[iObj]->m_matCalculation *= matParent;
		}


		pMatrixList[iObj] = m_ObjectList[iObj]->m_matCalculation;

	}
}

void CBY_BoneObj::MTRUpdate(int iStart, int iEnd, float fTime, D3DXMATRIX* pMatrixList)
{
	CMatSetData matdata;
	m_bAniStart = false;
	m_bAniEnd = false;

	if (m_fElapseTick==0.0f)
	{
		m_bAniStart = true;
	}

	float afTime = 0;
	afTime = 1.0f * g_SecondTime * m_Scene.iTickPerFrame * m_Scene.iFrameSpeed;
	if (afTime >= m_Scene.iTickPerFrame)
	{
		afTime = m_Scene.iTickPerFrame;
	}

	m_fElapseTick += afTime;
	if (m_fElapseTick >= m_Scene.iLastFrame * m_Scene.iTickPerFrame)
	{
		m_bAniEnd = true;
		m_fElapseTick = 0.0f;
	}

	if (m_bAniEndClip)
	{
		m_fElapseTick = m_Scene.iLastFrame * m_Scene.iTickPerFrame;
	}

	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		CMatSetData matdata(m_ObjectList[iObj]->m_vAnimPos, m_ObjectList[iObj]->m_vAnimScale,
			m_ObjectList[iObj]->m_qAnimRotation, m_ObjectList[iObj]->m_qAnimScaleRotation);
		CAnimationTrack start;
		start.iTick = iStart;
		start.p = matdata.vPos;
		start.q = matdata.qR;

		if (m_ObjectList[iObj]->posTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->posTrack, ANI_POS, m_fElapseTick);
		}

		if (m_ObjectList[iObj]->rotTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->rotTrack, ANI_ROT, m_fElapseTick);
		}

		if (m_ObjectList[iObj]->sclTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->sclTrack, ANI_SCL, m_fElapseTick);
		}
		m_ObjectList[iObj]->m_matCalculation = matdata.matScale * matdata.matRotation;
		m_ObjectList[iObj]->m_matCalculation._41 = matdata.vPos.x;
		m_ObjectList[iObj]->m_matCalculation._42 = matdata.vPos.y;
		m_ObjectList[iObj]->m_matCalculation._43 = matdata.vPos.z;


		if (m_ObjectList[iObj]->m_Parent != nullptr)
		{
			D3DXMATRIX matParent = m_ObjectList[iObj]->m_Parent->m_matCalculation;
			m_ObjectList[iObj]->m_matCalculation *= matParent;
		}


		pMatrixList[iObj] = m_ObjectList[iObj]->m_matCalculation;

	}
}


bool CBY_BoneObj::AniTrackSet(CMatSetData& matdata, CAnimationTrack start, int iobj, std::vector<CAnimationTrack>& AniTrack, int tracktype, float fETick)
{
	CAnimationTrack aniStart = start;
	CAnimationTrack aniEnd;
	for (int tick = 0; tick < AniTrack.size(); tick++)
	{
		if (AniTrack[tick].iTick <= fETick)
		{
			aniStart = AniTrack[tick];
		}
		if (AniTrack[tick].iTick > fETick)
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
		t = (fETick - aniStart.iTick) / step;
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

void CBY_BoneObj::ObjUpdate(int iStart, int iEnd, float fTime, D3DXMATRIX* pMatrixList, D3DXMATRIX* parmat, int socket)
{
	CMatSetData matdata;

	float afTime = 0;
	afTime = 1.0f * g_SecondTime * m_Scene.iTickPerFrame * m_Scene.iFrameSpeed;
	if (afTime >= m_Scene.iTickPerFrame)
	{
		afTime = m_Scene.iTickPerFrame;
	}

	m_fElapseTick += afTime;
	if (m_fElapseTick >= m_Scene.iLastFrame * m_Scene.iTickPerFrame)
	{
		m_fElapseTick = 0.0f;
	}

	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		CMatSetData matdata(m_ObjectList[iObj]->m_vAnimPos, m_ObjectList[iObj]->m_vAnimScale,
			m_ObjectList[iObj]->m_qAnimRotation, m_ObjectList[iObj]->m_qAnimScaleRotation);
		CAnimationTrack start;
		start.iTick = iStart;
		start.p = matdata.vPos;
		start.q = matdata.qR;

		if (m_ObjectList[iObj]->posTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->posTrack, ANI_POS, m_fElapseTick);
		}

		if (m_ObjectList[iObj]->rotTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->rotTrack, ANI_ROT, m_fElapseTick);
		}

		if (m_ObjectList[iObj]->sclTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_ObjectList[iObj]->sclTrack, ANI_SCL, m_fElapseTick);
		}
		m_ObjectList[iObj]->m_matCalculation = matdata.matScale * matdata.matRotation;
		m_ObjectList[iObj]->m_matCalculation._41 = matdata.vPos.x;
		m_ObjectList[iObj]->m_matCalculation._42 = matdata.vPos.y;
		m_ObjectList[iObj]->m_matCalculation._43 = matdata.vPos.z;


		if (m_ObjectList[iObj]->m_Parent != nullptr)
		{
			D3DXMATRIX matParent = m_ObjectList[iObj]->m_Parent->m_matCalculation;
			m_ObjectList[iObj]->m_matCalculation *= matParent;
		}

		if (socket == iObj)
		{
			m_ObjectList[iObj]->m_matCalculation *= *parmat;
		}

		pMatrixList[iObj] = m_ObjectList[iObj]->m_matCalculation;
	}
}

void    CBY_BoneObj::Convert(std::vector<PNCTIW_VERTEX>& list)
{
	int iBaseVertex = 0;
	int iBaseIndex = 0;
	int iBaseOffset = 0;

	for (int iObj = 0; iObj < m_ObjLoader.m_ObjList.size(); iObj++)
	{
		CBY_MeshSkin* mesh = m_ObjLoader.m_ObjList[iObj];

		mesh->m_iParentIndex = m_ObjLoader.GetParentIndex(mesh->m_szParentName);


		D3DXMATRIX matInvParent, matAnim;
		D3DXMatrixIdentity(&matInvParent);
		if (mesh->m_iParentIndex >= 0)
		{
			mesh->m_Parent = m_ObjLoader.m_ObjList[mesh->m_iParentIndex];
			m_ObjLoader.m_ObjList[mesh->m_iParentIndex]->m_Child.push_back(mesh);
			D3DXMatrixInverse(&matInvParent, NULL, &m_ObjLoader.m_ObjList[mesh->m_iParentIndex]->m_matWorld);
		}
		matAnim = mesh->m_matWorld * matInvParent;
		D3DXMatrixDecompose(&mesh->m_vAnimScale, &mesh->m_qAnimRotation, &mesh->m_vAnimPos, &matAnim);
		D3DXQuaternionIdentity(&mesh->m_qAnimScaleRotation);


		int iRef = mesh->m_iTexIndex;
		if (iRef >= 0)
		{
			mesh->subMeshSkin.resize(
				m_ObjLoader.m_MtlList[iRef].SubMaterial.size());
		}


		if (mesh->subMeshSkin.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < mesh->subMeshSkin.size(); iSubMesh++)
			{
				if (iRef >= 0)
				{
					if (m_ObjLoader.m_MtlList[iRef].SubMaterial[iSubMesh].texList.size() > 0)
					{
						mesh->subMeshSkin[iSubMesh].m_iTexIndex =
							I_Texture.Add(m_obj.m_pd3dDevice,
								m_ObjLoader.m_MtlList[iRef].SubMaterial[iSubMesh].texList[0].szTextureName,
								L"../../data/Char/texture/");
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
						m_ObjLoader.m_MtlList[iRef].texList[0].szTextureName,
						L"../../data/Char/texture/");
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
	m_ObjectList.swap(m_ObjLoader.m_ObjList);
	m_Scene = m_ObjLoader.m_Scene;
	m_matBoneBindPoss.swap(m_ObjLoader.m_matBoneBindPoss);
	m_ObjLoader.Release();
}

CBY_BoneObj::CBY_BoneObj()
{
	m_fElapseTick = 0.0f;
	m_bAniStart = false;
	m_bAniEnd = false;
	m_bAniEndClip = false;
}


CBY_BoneObj::~CBY_BoneObj()
{
}
}