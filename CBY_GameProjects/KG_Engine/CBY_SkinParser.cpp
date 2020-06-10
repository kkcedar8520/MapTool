#include "CBY_SkinParser.h"

namespace CBY
{
	int  CBY_SkinParser::GetParentIndex(C_STR szParentName)
	{
		for (int iObj = 0; iObj < m_ObjList.size(); iObj++)
		{
			C_STR szName = m_ObjList[iObj]->m_szName;
			if (szName == szParentName)
			{
				return iObj;
			}
		}
		return -1;
	}
	bool CBY_SkinParser::Load(const TCHAR* szFileName)
	{
		if (OpenStream(szFileName) == FALSE) return false;

		int ibuf[6] = { 0, };
		fread((void*)ibuf, sizeof(int), 6, m_pStream);
		m_Scene.iFirstFrame = ibuf[0];
		m_Scene.iLastFrame = ibuf[1];
		m_Scene.iFrameSpeed = ibuf[2];
		m_Scene.iTickPerFrame = ibuf[3];
		m_Scene.iNumMaterial = ibuf[4];
		m_Scene.iNumMesh = ibuf[5];

		m_MtlList.resize(m_Scene.iNumMaterial);

		for (int iMtrl = 0; iMtrl < m_Scene.iNumMaterial; iMtrl++)
		{
			fread((void*)ibuf, sizeof(int), 2, m_pStream);

			if (ibuf[1] > 0)
			{
				m_MtlList[iMtrl].SubMaterial.resize(ibuf[1]);
				for (int iSubMtrl = 0; iSubMtrl < m_MtlList[iMtrl].SubMaterial.size(); iSubMtrl++)
				{
					fread((void*)ibuf, sizeof(int), 2, m_pStream);
					m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList.resize(ibuf[1]);

					for (int iMap = 0; iMap < m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList.size(); iMap++)
					{
						TCHAR cbuf[256] = { 0, };
						fread((void*)ibuf, sizeof(int), 2, m_pStream);
						m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].iSubNo = ibuf[1];

						int length[1];
						fread((void*)length, sizeof(int), 1, m_pStream);

						fread((void*)cbuf, sizeof(TCHAR), length[0], m_pStream);
						_tcsncpy_s(m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].szTextureName, cbuf, sizeof(TCHAR)*length[0]);
					}
				}
			}
			else
			{
				fread((void*)ibuf, sizeof(int), 2, m_pStream);
				m_MtlList[iMtrl].texList.resize(ibuf[1]);

				for (int iMap = 0; iMap < m_MtlList[iMtrl].texList.size(); iMap++)
				{
					TCHAR cbuf[256] = { 0, };
					fread((void*)ibuf, sizeof(int), 2, m_pStream);
					m_MtlList[iMtrl].texList[iMap].iSubNo = ibuf[1];

					int length[1];
					fread((void*)length, sizeof(int), 1, m_pStream);

					fread((void*)cbuf, sizeof(TCHAR), length[0], m_pStream);
					_tcsncpy_s(m_MtlList[iMtrl].texList[iMap].szTextureName, cbuf, sizeof(TCHAR)*length[0]);
				}
			}
		}
		m_ObjList.resize(m_Scene.iNumMesh);
		for (int iObj = 0; iObj < m_Scene.iNumMesh; iObj++)
		{
			m_ObjList[iObj] = new CBY_MeshSkin;
			TCHAR cbuf[256] = { 0, };
			TCHAR cbuf2[256] = { 0, };

			int objibuf[7];
			fread((void*)objibuf, sizeof(int), 7, m_pStream);

			m_ObjList[iObj]->vblistSkin.resize(objibuf[0]);
			m_ObjList[iObj]->iblistSkin.resize(objibuf[1]);
			m_ObjList[iObj]->m_iTexIndex = objibuf[2];
			m_ObjList[iObj]->posTrack.resize(objibuf[3]);
			m_ObjList[iObj]->rotTrack.resize(objibuf[4]);
			m_ObjList[iObj]->sclTrack.resize(objibuf[5]);
			m_ObjList[iObj]->subMeshSkin.resize(objibuf[6]);


			int length[1];
			fread((void*)length, sizeof(int), 1, m_pStream);

			fread((void*)cbuf, sizeof(TCHAR), length[0], m_pStream);
			m_ObjList[iObj]->m_szName = wtm(cbuf);							//자기 이름

			fread((void*)length, sizeof(int), 1, m_pStream);
			fread((void*)cbuf2, sizeof(TCHAR), length[0], m_pStream);		//부모 이름
			m_ObjList[iObj]->m_szParentName = wtm(cbuf2);

			float fBuf[16];
			fread((void*)fBuf, sizeof(float), 16, m_pStream);
			m_ObjList[iObj]->m_matWorld._11 = fBuf[0];
			m_ObjList[iObj]->m_matWorld._12 = fBuf[1];
			m_ObjList[iObj]->m_matWorld._13 = fBuf[2];
			m_ObjList[iObj]->m_matWorld._14 = fBuf[3];
			m_ObjList[iObj]->m_matWorld._21 = fBuf[4];
			m_ObjList[iObj]->m_matWorld._22 = fBuf[5];
			m_ObjList[iObj]->m_matWorld._23 = fBuf[6];
			m_ObjList[iObj]->m_matWorld._24 = fBuf[7];
			m_ObjList[iObj]->m_matWorld._31 = fBuf[8];
			m_ObjList[iObj]->m_matWorld._32 = fBuf[9];
			m_ObjList[iObj]->m_matWorld._33 = fBuf[10];
			m_ObjList[iObj]->m_matWorld._34 = fBuf[11];
			m_ObjList[iObj]->m_matWorld._41 = fBuf[12];
			m_ObjList[iObj]->m_matWorld._42 = fBuf[13];
			m_ObjList[iObj]->m_matWorld._43 = fBuf[14];
			m_ObjList[iObj]->m_matWorld._44 = fBuf[15];

			float verfbuf[10];

			if (m_ObjList[iObj]->subMeshSkin.size() > 0)
			{
				for (int iSubMesh = 0; iSubMesh <
					m_ObjList[iObj]->subMeshSkin.size(); iSubMesh++)
				{
					int subibuf[7];
					fread((void*)subibuf, sizeof(int), 7, m_pStream);
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.resize(subibuf[1]);
					m_ObjList[iObj]->subMeshSkin[iSubMesh].iblistSkin.resize(subibuf[2]);
					m_ObjList[iObj]->subMeshSkin[iSubMesh].m_iTexIndex = subibuf[3];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].posTrack.resize(subibuf[4]);
					m_ObjList[iObj]->subMeshSkin[iSubMesh].rotTrack.resize(subibuf[5]);
					m_ObjList[iObj]->subMeshSkin[iSubMesh].sclTrack.resize(subibuf[6]);


					for (int iV = 0; iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
					{
						fread((void*)verfbuf, sizeof(float), 3, m_pStream);
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].p.x = verfbuf[0];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].p.y = verfbuf[1];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].p.z = verfbuf[2];
					}


					for (int iV = 0;
						iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
					{
						fread((void*)verfbuf, sizeof(float), 3, m_pStream);
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].n.x = verfbuf[0];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].n.y = verfbuf[1];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].n.z = verfbuf[2];
					}


					for (int iV = 0;
						iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
					{
						fread((void*)verfbuf, sizeof(float), 4, m_pStream);
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].c.x = verfbuf[0];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].c.y = verfbuf[1];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].c.z = verfbuf[2];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].c.w = verfbuf[3];
					}


					for (int iV = 0;
						iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
					{
						fread((void*)verfbuf, sizeof(float), 2, m_pStream);
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].t.x = verfbuf[0];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].t.y = verfbuf[1];
					}


					for (int iV = 0;
						iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
					{
						fread((void*)verfbuf, sizeof(float), 4, m_pStream);
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].i.x = verfbuf[0];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].i.y = verfbuf[1];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].i.z = verfbuf[2];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].i.w = verfbuf[3];
					}

					for (int iV = 0;
						iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
					{
						fread((void*)verfbuf, sizeof(float), 4, m_pStream);
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].w.x = verfbuf[0];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].w.y = verfbuf[1];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].w.z = verfbuf[2];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].w.w = verfbuf[3];
					}

					for (int iV = 0;
						iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
					{
						fread((void*)verfbuf, sizeof(float), 3, m_pStream);

						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].tan.x = verfbuf[0];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].tan.y = verfbuf[1];
						m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].tan.z = verfbuf[2];
					}

					for (int iV = 0;
						iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].iblistSkin.size(); iV++)
					{
						int indexbuf[1] = { 0, };
						fread((void*)indexbuf, sizeof(int), 1, m_pStream);

						m_ObjList[iObj]->subMeshSkin[iSubMesh].iblistSkin[iV] = indexbuf[0];
					}
				}
			}
			else
			{
				for (int iV = 0; iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
				{
					fread((void*)verfbuf, sizeof(float), 3, m_pStream);
					m_ObjList[iObj]->vblistSkin[iV].p.x = verfbuf[0];
					m_ObjList[iObj]->vblistSkin[iV].p.y = verfbuf[1];
					m_ObjList[iObj]->vblistSkin[iV].p.z = verfbuf[2];
				}


				for (int iV = 0;
					iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
				{
					fread((void*)verfbuf, sizeof(float), 3, m_pStream);
					m_ObjList[iObj]->vblistSkin[iV].n.x = verfbuf[0];
					m_ObjList[iObj]->vblistSkin[iV].n.y = verfbuf[1];
					m_ObjList[iObj]->vblistSkin[iV].n.z = verfbuf[2];
				}


				for (int iV = 0;
					iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
				{
					fread((void*)verfbuf, sizeof(float), 4, m_pStream);
					m_ObjList[iObj]->vblistSkin[iV].c.x = verfbuf[0];
					m_ObjList[iObj]->vblistSkin[iV].c.y = verfbuf[1];
					m_ObjList[iObj]->vblistSkin[iV].c.z = verfbuf[2];
					m_ObjList[iObj]->vblistSkin[iV].c.w = verfbuf[3];
				}


				for (int iV = 0;
					iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
				{
					fread((void*)verfbuf, sizeof(float), 2, m_pStream);
					m_ObjList[iObj]->vblistSkin[iV].t.x = verfbuf[0];
					m_ObjList[iObj]->vblistSkin[iV].t.y = verfbuf[1];
				}


				for (int iV = 0;
					iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
				{
					fread((void*)verfbuf, sizeof(float), 4, m_pStream);
					m_ObjList[iObj]->vblistSkin[iV].i.x = verfbuf[0];
					m_ObjList[iObj]->vblistSkin[iV].i.y = verfbuf[1];
					m_ObjList[iObj]->vblistSkin[iV].i.z = verfbuf[2];
					m_ObjList[iObj]->vblistSkin[iV].i.w = verfbuf[3];
				}

				for (int iV = 0;
					iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
				{
					fread((void*)verfbuf, sizeof(float), 4, m_pStream);
					m_ObjList[iObj]->vblistSkin[iV].w.x = verfbuf[0];
					m_ObjList[iObj]->vblistSkin[iV].w.y = verfbuf[1];
					m_ObjList[iObj]->vblistSkin[iV].w.z = verfbuf[2];
					m_ObjList[iObj]->vblistSkin[iV].w.w = verfbuf[3];
				}

				for (int iV = 0;
					iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
				{
					fread((void*)verfbuf, sizeof(float), 3, m_pStream);

					m_ObjList[iObj]->vblistSkin[iV].tan.x = verfbuf[0];
					m_ObjList[iObj]->vblistSkin[iV].tan.y = verfbuf[1];
					m_ObjList[iObj]->vblistSkin[iV].tan.z = verfbuf[2];
				}

				for (int iV = 0;
					iV < m_ObjList[iObj]->iblistSkin.size(); iV++)
				{
					int indexbuf[1] = { 0, };
					fread((void*)indexbuf, sizeof(int), 1, m_pStream);

					m_ObjList[iObj]->iblistSkin[iV] = indexbuf[0];
				}
			}

			for (int iTrack = 0;
				iTrack < m_ObjList[iObj]->posTrack.size(); iTrack++)
			{
				fread((void*)verfbuf, sizeof(float), 4, m_pStream);
				m_ObjList[iObj]->posTrack[iTrack].iTick = verfbuf[0];
				m_ObjList[iObj]->posTrack[iTrack].p.x = verfbuf[1];
				m_ObjList[iObj]->posTrack[iTrack].p.y = verfbuf[2];
				m_ObjList[iObj]->posTrack[iTrack].p.z = verfbuf[3];
			}

			for (int iTrack = 0;
				iTrack < m_ObjList[iObj]->rotTrack.size(); iTrack++)
			{
				fread((void*)verfbuf, sizeof(float), 5, m_pStream);
				m_ObjList[iObj]->rotTrack[iTrack].iTick = verfbuf[0];
				m_ObjList[iObj]->rotTrack[iTrack].q.x = verfbuf[1];
				m_ObjList[iObj]->rotTrack[iTrack].q.y = verfbuf[2];
				m_ObjList[iObj]->rotTrack[iTrack].q.z = verfbuf[3];
				m_ObjList[iObj]->rotTrack[iTrack].q.w = verfbuf[4];
			}
			for (int iTrack = 0;
				iTrack < m_ObjList[iObj]->sclTrack.size(); iTrack++)
			{
				fread((void*)verfbuf, sizeof(float), 8, m_pStream);
				m_ObjList[iObj]->sclTrack[iTrack].iTick = verfbuf[0];

				m_ObjList[iObj]->sclTrack[iTrack].p.x = verfbuf[1];
				m_ObjList[iObj]->sclTrack[iTrack].p.y = verfbuf[2];
				m_ObjList[iObj]->sclTrack[iTrack].p.z = verfbuf[3];

				m_ObjList[iObj]->sclTrack[iTrack].q.x = verfbuf[4];
				m_ObjList[iObj]->sclTrack[iTrack].q.y = verfbuf[5];
				m_ObjList[iObj]->sclTrack[iTrack].q.z = verfbuf[6];
				m_ObjList[iObj]->sclTrack[iTrack].q.w = verfbuf[7];
			}

			fread((void*)verfbuf, sizeof(float), 10, m_pStream);

			m_ObjList[iObj]->m_CharBox.vCenter.x = verfbuf[1];
			m_ObjList[iObj]->m_CharBox.vCenter.y = verfbuf[2];
			m_ObjList[iObj]->m_CharBox.vCenter.z = verfbuf[3];

			m_ObjList[iObj]->m_CharBox.vMax.x = verfbuf[4];
			m_ObjList[iObj]->m_CharBox.vMax.y = verfbuf[5];
			m_ObjList[iObj]->m_CharBox.vMax.z = verfbuf[6];
			m_ObjList[iObj]->m_CharBox.vMin.x = verfbuf[7];
			m_ObjList[iObj]->m_CharBox.vMin.y = verfbuf[8];
			m_ObjList[iObj]->m_CharBox.vMin.z = verfbuf[9];
		}

		int objsize[1];
		fread((void*)objsize, sizeof(int), 1, m_pStream);
		m_matBoneBindPoss.resize(objsize[0]);

		for (int ibone = 0; ibone < m_matBoneBindPoss.size(); ibone++)
		{
			float fbone[16];
			fread((void*)fbone, sizeof(float), 16, m_pStream);

			m_matBoneBindPoss[ibone]._11 = fbone[0];
			m_matBoneBindPoss[ibone]._12 = fbone[1];
			m_matBoneBindPoss[ibone]._13 = fbone[2];
			m_matBoneBindPoss[ibone]._14 = fbone[3];
			m_matBoneBindPoss[ibone]._21 = fbone[4];
			m_matBoneBindPoss[ibone]._22 = fbone[5];
			m_matBoneBindPoss[ibone]._23 = fbone[6];
			m_matBoneBindPoss[ibone]._24 = fbone[7];
			m_matBoneBindPoss[ibone]._31 = fbone[8];
			m_matBoneBindPoss[ibone]._32 = fbone[9];
			m_matBoneBindPoss[ibone]._33 = fbone[10];
			m_matBoneBindPoss[ibone]._34 = fbone[11];
			m_matBoneBindPoss[ibone]._41 = fbone[12];
			m_matBoneBindPoss[ibone]._42 = fbone[13];
			m_matBoneBindPoss[ibone]._43 = fbone[14];
			m_matBoneBindPoss[ibone]._44 = fbone[15];
		}
		//_fgetts(m_pBuffer, 256, m_pStream);
		//_stscanf(m_pBuffer, _T("%s %d %d %d %d %d %d"),
		//	m_pString,
		//	&m_Scene.iFirstFrame,
		//	&m_Scene.iLastFrame,
		//	&m_Scene.iFrameSpeed,
		//	&m_Scene.iTickPerFrame,
		//	&m_Scene.iNumMaterial,
		//	&m_Scene.iNumMesh);

		//int iTemp = 0;
		//int iNumSubMtrl = 0;
		//int iNumSubTexmap = 0;

		//m_MtlList.resize(m_Scene.iNumMaterial);
		//for (int iMtrl = 0; iMtrl < m_Scene.iNumMaterial; iMtrl++)
		//{
		//	_fgetts(m_pBuffer, 256, m_pStream);
		//	_stscanf(m_pBuffer, _T("%s %d %d"), m_pString, &iTemp, &iNumSubMtrl);

		//	if (iNumSubMtrl > 0)
		//	{
		//		m_MtlList[iMtrl].SubMaterial.resize(iNumSubMtrl);
		//		for (int iSubMtrl = 0;
		//			iSubMtrl < iNumSubMtrl; iSubMtrl++)
		//		{
		//			int isubo;
		//			_fgetts(m_pBuffer, 256, m_pStream);
		//			_stscanf(m_pBuffer, _T("%s%d%d"),
		//				m_pString,
		//				&isubo,
		//				&iNumSubTexmap);
		//			m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList.resize(iNumSubTexmap);

		//			for (int iMap = 0;
		//				iMap < iNumSubTexmap; iMap++)
		//			{
		//				_fgetts(m_pBuffer, 256, m_pStream);
		//				_stscanf(m_pBuffer, _T("%s %d %d %s\n"), m_pString, &iTemp, &m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].iSubNo,
		//					m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].szTextureName);

		//			}
		//		}
		//	}
		//	else
		//	{
		//		_fgetts(m_pBuffer, 256, m_pStream);
		//		_stscanf(m_pBuffer, _T("%s %d %d"), m_pString, &iTemp, &iNumSubTexmap);
		//		m_MtlList[iMtrl].texList.resize(iNumSubTexmap);
		//		for (int iMap = 0;
		//			iMap < iNumSubTexmap; iMap++)
		//		{
		//			_fgetts(m_pBuffer, 256, m_pStream);
		//			_stscanf(m_pBuffer, _T("%s %d %d %s"),
		//				m_pString,
		//				&iTemp,
		//				&m_MtlList[iMtrl].texList[iMap].iSubNo,
		//				m_MtlList[iMtrl].texList[iMap].szTextureName);

		//		}
		//	}
		//}

		//m_ObjList.resize(m_Scene.iNumMesh);
		//int iNumArray[6];
		//int iNumCnt = 0;
		//for (int iObj = 0; iObj < m_Scene.iNumMesh; iObj++)
		//{
		//	m_ObjList[iObj] = new CBY_MeshSkin;
		//	_fgetts(m_pBuffer, 256, m_pStream);
		//	_stscanf(m_pBuffer, _T("%s %d %d %d %d %d %d %d\n"),
		//		m_pString,
		//		&iTemp,
		//		&iNumArray[0],
		//		&iNumArray[1],
		//		&iNumArray[2],
		//		&iNumArray[3],
		//		&iNumArray[4],
		//		&iNumArray[5]
		//	);
		//	m_ObjList[iObj]->listSkin.resize(iNumArray[0]);
		//	m_ObjList[iObj]->m_iTexIndex = iNumArray[1];
		//	m_ObjList[iObj]->posTrack.resize(iNumArray[2]);
		//	m_ObjList[iObj]->rotTrack.resize(iNumArray[3]);
		//	m_ObjList[iObj]->sclTrack.resize(iNumArray[4]);
		//	m_ObjList[iObj]->subMeshSkin.resize(iNumArray[5]);


		//	_fgetts(m_pBuffer, 256, m_pStream);
		//	TCHAR szName[255] = { 0, };
		//	TCHAR szParentName[255] = { 0, };
		//	_stscanf(m_pBuffer, _T("%s %s %s"),
		//		m_pString,
		//		szName,
		//		szParentName);
		//	m_ObjList[iObj]->m_szName = wtm(szName);
		//	m_ObjList[iObj]->m_szParentName = wtm(szParentName);

		//	_fgetts(m_pBuffer, 256, m_pStream);
		//	_stscanf(m_pBuffer, _T("%s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"),
		//		m_pString,
		//		&m_ObjList[iObj]->m_matWorld._11,
		//		&m_ObjList[iObj]->m_matWorld._12,
		//		&m_ObjList[iObj]->m_matWorld._13,
		//		&m_ObjList[iObj]->m_matWorld._14,
		//		&m_ObjList[iObj]->m_matWorld._21,
		//		&m_ObjList[iObj]->m_matWorld._22,
		//		&m_ObjList[iObj]->m_matWorld._23,
		//		&m_ObjList[iObj]->m_matWorld._24,
		//		&m_ObjList[iObj]->m_matWorld._31,
		//		&m_ObjList[iObj]->m_matWorld._32,
		//		&m_ObjList[iObj]->m_matWorld._33,
		//		&m_ObjList[iObj]->m_matWorld._34,
		//		&m_ObjList[iObj]->m_matWorld._41,
		//		&m_ObjList[iObj]->m_matWorld._42,
		//		&m_ObjList[iObj]->m_matWorld._43,
		//		&m_ObjList[iObj]->m_matWorld._44);

		//	if (m_ObjList[iObj]->subMeshSkin.size() > 0)
		//	{
		//		for (int iSubMesh = 0; iSubMesh <
		//			m_ObjList[iObj]->subMeshSkin.size(); iSubMesh++)
		//		{
		//			_fgetts(m_pBuffer, 256, m_pStream);
		//			_stscanf(m_pBuffer, _T("%s %d %d %d %d %d %d"),
		//				m_pString,
		//				&iTemp,
		//				&iNumArray[0],
		//				&iNumArray[1],
		//				&iNumArray[2],
		//				&iNumArray[3],
		//				&iNumArray[4],
		//				&iNumArray[5]);


		//			m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin.resize(iNumArray[0]);
		//			m_ObjList[iObj]->subMeshSkin[iSubMesh].m_iTexIndex = iNumArray[1];
		//			m_ObjList[iObj]->subMeshSkin[iSubMesh].posTrack.resize(iNumArray[2]);
		//			m_ObjList[iObj]->subMeshSkin[iSubMesh].rotTrack.resize(iNumArray[3]);
		//			m_ObjList[iObj]->subMeshSkin[iSubMesh].sclTrack.resize(iNumArray[4]);
		//			//m_ObjList[iObj].subMeshSkin[iSubMesh].subMeshSkin.resize(iNumArray[5]);


		//			//position
		//			for (int iV = 0;
		//				iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin.size(); iV++)
		//			{
		//				_fgetts(m_pBuffer, 256, m_pStream);
		//				_stscanf(m_pBuffer,
		//					_T("%s %f %f %f"),
		//					m_pString,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].p.x,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].p.y,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].p.z);
		//			}

		//			//nomal
		//			for (int iV = 0;
		//				iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin.size(); iV++)
		//			{
		//				_fgetts(m_pBuffer, 256, m_pStream);
		//				_stscanf(m_pBuffer,
		//					_T("%s %f %f %f"),
		//					m_pString,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].n.x,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].n.y,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].n.z);
		//			}

		//			//color
		//			for (int iV = 0;
		//				iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin.size(); iV++)
		//			{
		//				_fgetts(m_pBuffer, 256, m_pStream);
		//				_stscanf(m_pBuffer,
		//					_T("%s %f %f %f %f"),
		//					m_pString,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].c.x,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].c.y,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].c.z,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].c.w);
		//			}

		//			//texture
		//			for (int iV = 0;
		//				iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin.size(); iV++)
		//			{
		//				_fgetts(m_pBuffer, 256, m_pStream);
		//				_stscanf(m_pBuffer,
		//					_T("%s %f %f"),
		//					m_pString,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].t.x,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].t.y);
		//			}

		//			//index
		//			for (int iV = 0;
		//				iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin.size(); iV++)
		//			{
		//				_fgetts(m_pBuffer, 256, m_pStream);
		//				_stscanf(m_pBuffer,
		//					_T("%s %f %f %f %f"),
		//					m_pString,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].i.x,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].i.y,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].i.z,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].i.w);
		//			}

		//			//weight
		//			for (int iV = 0;
		//				iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin.size(); iV++)
		//			{
		//				_fgetts(m_pBuffer, 256, m_pStream);
		//				_stscanf(m_pBuffer,
		//					_T("%s %f %f %f %f"),
		//					m_pString,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].w.x,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].w.y,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].w.z,
		//					&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].w.w);
		//			}

		//		}
		//	}
		//	else
		//	{
		//		//position
		//		for (int iV = 0;
		//			iV < m_ObjList[iObj]->listSkin.size(); iV++)
		//		{
		//			_fgetts(m_pBuffer, 256, m_pStream);
		//			_stscanf(m_pBuffer,
		//				_T("%s %f %f %f"),
		//				m_pString,
		//				&m_ObjList[iObj]->listSkin[iV].p.x,
		//				&m_ObjList[iObj]->listSkin[iV].p.y,
		//				&m_ObjList[iObj]->listSkin[iV].p.z);
		//		}

		//		//nomal
		//		for (int iV = 0;
		//			iV < m_ObjList[iObj]->listSkin.size(); iV++)
		//		{
		//			_fgetts(m_pBuffer, 256, m_pStream);
		//			_stscanf(m_pBuffer,
		//				_T("%s %f %f %f"),
		//				m_pString,
		//				&m_ObjList[iObj]->listSkin[iV].n.x,
		//				&m_ObjList[iObj]->listSkin[iV].n.y,
		//				&m_ObjList[iObj]->listSkin[iV].n.z);
		//		}

		//		//color
		//		for (int iV = 0;
		//			iV < m_ObjList[iObj]->listSkin.size(); iV++)
		//		{
		//			_fgetts(m_pBuffer, 256, m_pStream);
		//			_stscanf(m_pBuffer,
		//				_T("%s %f %f %f %f"),
		//				m_pString,
		//				&m_ObjList[iObj]->listSkin[iV].c.x,
		//				&m_ObjList[iObj]->listSkin[iV].c.y,
		//				&m_ObjList[iObj]->listSkin[iV].c.z,
		//				&m_ObjList[iObj]->listSkin[iV].c.w);
		//		}

		//		//texture
		//		for (int iV = 0;
		//			iV < m_ObjList[iObj]->listSkin.size(); iV++)
		//		{
		//			_fgetts(m_pBuffer, 256, m_pStream);
		//			_stscanf(m_pBuffer,
		//				_T("%s %f %f"),
		//				m_pString,
		//				&m_ObjList[iObj]->listSkin[iV].t.x,
		//				&m_ObjList[iObj]->listSkin[iV].t.y);
		//		}

		//		//index
		//		for (int iV = 0;
		//			iV < m_ObjList[iObj]->listSkin.size(); iV++)
		//		{
		//			_fgetts(m_pBuffer, 256, m_pStream);
		//			_stscanf(m_pBuffer,
		//				_T("%s %f %f %f %f"),
		//				m_pString,
		//				&m_ObjList[iObj]->listSkin[iV].i.x,
		//				&m_ObjList[iObj]->listSkin[iV].i.y,
		//				&m_ObjList[iObj]->listSkin[iV].i.z,
		//				&m_ObjList[iObj]->listSkin[iV].i.w);
		//		}

		//		//weight
		//		for (int iV = 0;
		//			iV < m_ObjList[iObj]->listSkin.size(); iV++)
		//		{
		//			_fgetts(m_pBuffer, 256, m_pStream);
		//			_stscanf(m_pBuffer,
		//				_T("%s %f %f %f %f"),
		//				m_pString,
		//				&m_ObjList[iObj]->listSkin[iV].w.x,
		//				&m_ObjList[iObj]->listSkin[iV].w.y,
		//				&m_ObjList[iObj]->listSkin[iV].w.z,
		//				&m_ObjList[iObj]->listSkin[iV].w.w);
		//		}
		//	}


		//	// animation
		//	for (int iTrack = 0;
		//		iTrack < m_ObjList[iObj]->posTrack.size(); iTrack++)
		//	{
		//		_fgetts(m_pBuffer, 256, m_pStream);
		//		_stscanf(m_pBuffer, _T("%d %f %f %f"),
		//			&m_ObjList[iObj]->posTrack[iTrack].iTick,
		//			&m_ObjList[iObj]->posTrack[iTrack].p.x,
		//			&m_ObjList[iObj]->posTrack[iTrack].p.y,
		//			&m_ObjList[iObj]->posTrack[iTrack].p.z);
		//	}
		//	for (int iTrack = 0;
		//		iTrack < m_ObjList[iObj]->rotTrack.size(); iTrack++)
		//	{
		//		_fgetts(m_pBuffer, 256, m_pStream);
		//		_stscanf(m_pBuffer, _T("%d %f %f %f %f"),
		//			&m_ObjList[iObj]->rotTrack[iTrack].iTick,
		//			&m_ObjList[iObj]->rotTrack[iTrack].q.x,
		//			&m_ObjList[iObj]->rotTrack[iTrack].q.y,
		//			&m_ObjList[iObj]->rotTrack[iTrack].q.z,
		//			&m_ObjList[iObj]->rotTrack[iTrack].q.w);
		//	}
		//	for (int iTrack = 0;
		//		iTrack < m_ObjList[iObj]->sclTrack.size(); iTrack++)
		//	{
		//		_fgetts(m_pBuffer, 256, m_pStream);
		//		_stscanf(m_pBuffer, _T("%d %f %f %f %f %f %f %f"),
		//			&m_ObjList[iObj]->sclTrack[iTrack].iTick,

		//			&m_ObjList[iObj]->sclTrack[iTrack].p.x,
		//			&m_ObjList[iObj]->sclTrack[iTrack].p.y,
		//			&m_ObjList[iObj]->sclTrack[iTrack].p.z,

		//			&m_ObjList[iObj]->sclTrack[iTrack].q.x,
		//			&m_ObjList[iObj]->sclTrack[iTrack].q.y,
		//			&m_ObjList[iObj]->sclTrack[iTrack].q.z,
		//			&m_ObjList[iObj]->sclTrack[iTrack].q.w);
		//	}

		//	_fgetts(m_pBuffer, 256, m_pStream);
		//	_stscanf(m_pBuffer, _T("%s %d %f %f %f %f %f %f %f %f %f"),
		//		m_pString,
		//		&iTemp,
		//		&m_ObjList[iObj]->m_CharBox.vCenter.x,
		//		&m_ObjList[iObj]->m_CharBox.vCenter.y,
		//		&m_ObjList[iObj]->m_CharBox.vCenter.z,

		//		&m_ObjList[iObj]->m_CharBox.vMax.x,
		//		&m_ObjList[iObj]->m_CharBox.vMax.y,
		//		&m_ObjList[iObj]->m_CharBox.vMax.z,

		//		&m_ObjList[iObj]->m_CharBox.vMin.x,
		//		&m_ObjList[iObj]->m_CharBox.vMin.y,
		//		&m_ObjList[iObj]->m_CharBox.vMin.z);
		//}
		//int iBoneCount = 0;
		//_fgetts(m_pBuffer, 256, m_pStream);
		//_stscanf(m_pBuffer, _T("%s %d"), m_pString, &iBoneCount);

		//D3DXMATRIX mat;
		//for (int ibone = 0; ibone < iBoneCount; ibone++)
		//{
		//	_fgetts(m_pBuffer, 256, m_pStream);
		//	_stscanf(m_pBuffer, _T("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"),
		//		&mat._11,
		//		&mat._12,
		//		&mat._13,
		//		&mat._14,
		//		&mat._21,
		//		&mat._22,
		//		&mat._23,
		//		&mat._24,
		//		&mat._31,
		//		&mat._32,
		//		&mat._33,
		//		&mat._34,
		//		&mat._41,
		//		&mat._42,
		//		&mat._43,
		//		&mat._44);

		//	m_matBoneBindPoss.push_back(mat);
		//}
		CloseStream();
		return true;
	}
	BOOL  CBY_SkinParser::CBY_SkinParser::OpenStream(const TCHAR* filename)
	{
		m_pStream = _tfopen(filename, _T("rb"));
		if (m_pStream == nullptr)
		{
			return FALSE;
		}
		return TRUE;
	}

	BOOL  CBY_SkinParser::CloseStream()
	{
		fclose(m_pStream);
		return TRUE;
	}

	bool CBY_SkinParser::Release()
	{
		m_ObjList.clear();
		m_matBoneBindPoss.clear();
		m_MtlList.clear();
		return true;
	}

	CBY_SkinParser::CBY_SkinParser()
	{
		m_pStream = nullptr;
	}
	CBY_SkinParser::~CBY_SkinParser()
	{
	}
}

