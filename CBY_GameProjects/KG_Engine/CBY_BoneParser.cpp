#include "CBY_BoneParser.h"

namespace CBY
{
	bool CBY_BoneParser::Load(const TCHAR* szFileName)
	{
		if (OpenStream(szFileName) == FALSE) return false;

		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s %d %d %d %d %d %d"),
			m_pString,
			&m_Scene.iFirstFrame,
			&m_Scene.iLastFrame,
			&m_Scene.iFrameSpeed,
			&m_Scene.iTickPerFrame,
			&m_Scene.iNumMaterial,
			&m_Scene.iNumMesh);

		int iTemp = 0;
		int iNumSubMtrl = 0;
		int iNumSubTexmap = 0;

		m_MtlList.resize(m_Scene.iNumMaterial);
		for (int iMtrl = 0; iMtrl < m_Scene.iNumMaterial; iMtrl++)
		{
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s %d %d"), m_pString, &iTemp, &iNumSubMtrl);

			if (iNumSubMtrl > 0)
			{
				m_MtlList[iMtrl].SubMaterial.resize(iNumSubMtrl);
				for (int iSubMtrl = 0;
					iSubMtrl < iNumSubMtrl; iSubMtrl++)
				{
					_fgetts(m_pBuffer, 256, m_pStream);
					_stscanf(m_pBuffer, _T("%s%d%d"),
						m_pString,
						&iTemp,
						&iNumSubTexmap);
					m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList.resize(iNumSubTexmap);

					for (int iMap = 0;
						iMap < iNumSubTexmap; iMap++)
					{
						_fgetts(m_pBuffer, 256, m_pStream);
						_stscanf(m_pBuffer, _T("%s %d %d %s\n"), m_pString, &iTemp, &m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].iSubNo,
							m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].szTextureName);

					}
				}
			}
			else
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s %d %d"), m_pString, &iTemp, &iNumSubTexmap);
				m_MtlList[iMtrl].texList.resize(iNumSubTexmap);
				for (int iMap = 0;
					iMap < iNumSubTexmap; iMap++)
				{
					_fgetts(m_pBuffer, 256, m_pStream);
					_stscanf(m_pBuffer, _T("%s %d %d %s"),
						m_pString,
						&iTemp,
						&m_MtlList[iMtrl].texList[iMap].iSubNo,
						m_MtlList[iMtrl].texList[iMap].szTextureName);

				}
			}
		}

		m_ObjList.resize(m_Scene.iNumMesh);
		int iNumArray[8];
		int iNumCnt = 0;
		for (int iObj = 0; iObj < m_Scene.iNumMesh; iObj++)
		{
			m_ObjList[iObj] = new CBY_MeshSkin;
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s %d %d %d %d %d %d %d %d %d"),
				m_pString,
				&iTemp,
				&iNumArray[0],
				&iNumArray[1],
				&iNumArray[2],
				&iNumArray[3],
				&iNumArray[4],
				&iNumArray[5],
				&iNumArray[6],
				&iNumArray[7]
			);
			m_ObjList[iObj]->listSkin.resize(iNumArray[0]);
			m_ObjList[iObj]->m_iTexIndex = iNumArray[1];
			m_ObjList[iObj]->posTrack.resize(iNumArray[2]);
			m_ObjList[iObj]->rotTrack.resize(iNumArray[3]);
			m_ObjList[iObj]->sclTrack.resize(iNumArray[4]);
			m_ObjList[iObj]->subMeshSkin.resize(iNumArray[5]);
			if (iNumArray[6] == 1)
			{
				m_ObjList[iObj]->m_bBone = true;
			}
			if (iNumArray[7] == 1)
			{
				m_ObjList[iObj]->m_bRoot = true;
			}

			_fgetts(m_pBuffer, 256, m_pStream);
			TCHAR szName[255] = { 0, };
			TCHAR szParentName[255] = { 0, };
			_stscanf(m_pBuffer, _T("%s %s %s"),
				m_pString,
				szName,
				szParentName);
			m_ObjList[iObj]->m_szName = wtm(szName);
			m_ObjList[iObj]->m_szParentName = wtm(szParentName);

			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"),
				m_pString,
				&m_ObjList[iObj]->m_matWorld._11,
				&m_ObjList[iObj]->m_matWorld._12,
				&m_ObjList[iObj]->m_matWorld._13,
				&m_ObjList[iObj]->m_matWorld._14,
				&m_ObjList[iObj]->m_matWorld._21,
				&m_ObjList[iObj]->m_matWorld._22,
				&m_ObjList[iObj]->m_matWorld._23,
				&m_ObjList[iObj]->m_matWorld._24,
				&m_ObjList[iObj]->m_matWorld._31,
				&m_ObjList[iObj]->m_matWorld._32,
				&m_ObjList[iObj]->m_matWorld._33,
				&m_ObjList[iObj]->m_matWorld._34,
				&m_ObjList[iObj]->m_matWorld._41,
				&m_ObjList[iObj]->m_matWorld._42,
				&m_ObjList[iObj]->m_matWorld._43,
				&m_ObjList[iObj]->m_matWorld._44);

			// animation
			for (int iTrack = 0;
				iTrack < m_ObjList[iObj]->posTrack.size(); iTrack++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s %d %f %f %f"),
					m_pString,
					&m_ObjList[iObj]->posTrack[iTrack].iTick,
					&m_ObjList[iObj]->posTrack[iTrack].p.x,
					&m_ObjList[iObj]->posTrack[iTrack].p.y,
					&m_ObjList[iObj]->posTrack[iTrack].p.z);
			}
			for (int iTrack = 0;
				iTrack < m_ObjList[iObj]->rotTrack.size(); iTrack++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s %d %f %f %f %f"),
					m_pString,
					&m_ObjList[iObj]->rotTrack[iTrack].iTick,
					&m_ObjList[iObj]->rotTrack[iTrack].q.x,
					&m_ObjList[iObj]->rotTrack[iTrack].q.y,
					&m_ObjList[iObj]->rotTrack[iTrack].q.z,
					&m_ObjList[iObj]->rotTrack[iTrack].q.w);
			}
			for (int iTrack = 0;
				iTrack < m_ObjList[iObj]->sclTrack.size(); iTrack++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s %d %f %f %f %f %f %f %f"),
					m_pString,
					&m_ObjList[iObj]->sclTrack[iTrack].iTick,

					&m_ObjList[iObj]->sclTrack[iTrack].p.x,
					&m_ObjList[iObj]->sclTrack[iTrack].p.y,
					&m_ObjList[iObj]->sclTrack[iTrack].p.z,

					&m_ObjList[iObj]->sclTrack[iTrack].q.x,
					&m_ObjList[iObj]->sclTrack[iTrack].q.y,
					&m_ObjList[iObj]->sclTrack[iTrack].q.z,
					&m_ObjList[iObj]->sclTrack[iTrack].q.w);
			}

			if (m_ObjList[iObj]->m_bBone)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s %d %f %f %f %f %f %f %f %f %f"),
					m_pString,
					&iTemp,
					&m_ObjList[iObj]->m_Bone.m_vPos.x,
					&m_ObjList[iObj]->m_Bone.m_vPos.y,
					&m_ObjList[iObj]->m_Bone.m_vPos.z,

					&m_ObjList[iObj]->m_Bone.m_Box.vMax.x,
					&m_ObjList[iObj]->m_Bone.m_Box.vMax.y,
					&m_ObjList[iObj]->m_Bone.m_Box.vMax.z,

					&m_ObjList[iObj]->m_Bone.m_Box.vMin.x,
					&m_ObjList[iObj]->m_Bone.m_Box.vMin.y,
					&m_ObjList[iObj]->m_Bone.m_Box.vMin.z);
			}
		}

		int iBoneCount = 0;
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s %d"), m_pString, &iBoneCount);
		/*if (iBoneCount != m_Scene.iNumMesh)
		{
			iBoneCount = 0;
		}*/
		D3DXMATRIX mat;
		for (int ibone = 0; ibone < iBoneCount; ibone++)
		{
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"),
				&mat._11,
				&mat._12,
				&mat._13,
				&mat._14,
				&mat._21,
				&mat._22,
				&mat._23,
				&mat._24,
				&mat._31,
				&mat._32,
				&mat._33,
				&mat._34,
				&mat._41,
				&mat._42,
				&mat._43,
				&mat._44);

			m_matBoneBindPoss.push_back(mat);
		}

		CloseStream();
		return true;
	}

	CBY_BoneParser::CBY_BoneParser()
	{
	}


	CBY_BoneParser::~CBY_BoneParser()
	{
	}
}
