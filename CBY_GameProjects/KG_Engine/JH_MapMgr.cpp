#pragma once
#include "JH_MapMgr.h"
#include"JH_ObjMgr.h"






namespace JH {
	JH_MapMgr::JH_MapMgr()
	{
		m_ObjID = 0;
	}


	JH_MapMgr::~JH_MapMgr()
	{
	}

	TCHAR* JH_MapMgr::FixupName(T_STR name)
	{
		memset(m_tmpBuffer, 0, sizeof(TCHAR)*MAX_PATH);

		TCHAR* cPtr;
		_tcscpy(m_tmpBuffer, name.c_str());
		cPtr = m_tmpBuffer;

		while (*cPtr)
		{
			if (*cPtr == '"')		*cPtr = SINGLE_QUOTE;
			else if (*cPtr == ' ' || *cPtr <= CTL_CHARS)
				*cPtr = _T('_');
			cPtr++;
		}
		return m_tmpBuffer;
	}
	void JH_MapMgr::Set(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, KG_Camera* pCamera, KG_Select* select)
	{
		m_pd3dDevice = pDevice;
		m_pContext = pContext;
		m_pCamera = pCamera;
		m_pSelect = select;
	}

	bool JH_MapMgr::SaveMapData(const TCHAR* LoadFile)
	{
	

		m_MapData.Reset();
		std::shared_ptr<JH_Map> pMap = GetMap();
		std::shared_ptr<HQuadTree> pQuadTree = GetCurrentQuadTree();
		m_MapData.m_CharPos = pMap->GetCharPos();
		JH::OBJECT OBJ;
		for (auto Obj : pQuadTree->m_ObjectList)
		{
			OBJ.m_MapObj = std::make_shared<JH::MAP_OBJ_DATA>();
			OBJ.m_MapObj->m_matWorld = Obj.second->GetObj()->m_matWorld;
			OBJ.m_MapObj->m_BoneName = Obj.second->GetBoneName();
			OBJ.m_MapObj->m_SkinName = Obj.second->GetSkinName();
			OBJ.m_MapObj->m_iQuadTreeIndex = Obj.second->GetQuadIndex();
			OBJ.m_MapObj->m_Box = Obj.second->GetObj()->GetCharBox();
			OBJ.m_MapObj->m_Flag = Obj.second->GetFlag();
			m_MapData.m_sQTData.m_ObjList.push_back(OBJ);

			m_MapData.m_pSplattAlphaTextureFile = pMap->m_pSPTAFile;
		}

		FILE* fp;
		TCHAR  szFile[256] = { 0, };
		fp = _tfopen(LoadFile, _T("wb"));

		int		ibuf[6] = {};
		DWORD	Dbuf[2] = {};
		float	fbuf[40] = {};
		TCHAR Temp[256] = {};
		const TCHAR* pT;
		TCHAR cbuf[256] = {};
		ibuf[0] = lstrlenW(pMap->m_TextureFileName.data());
		fwrite((void*)ibuf, sizeof(int), 1, fp);
		fwrite((void*)pMap->m_TextureFileName.data(), sizeof(TCHAR), lstrlenW(pMap->m_TextureFileName.data()), fp);

		ibuf[0] = lstrlenW(pMap->m_pNormMapFileName.data());
		fwrite((void*)ibuf, sizeof(int), 1, fp);
		fwrite((void*)pMap->m_pNormMapFileName.data(), sizeof(TCHAR), lstrlenW(pMap->m_pNormMapFileName.data()), fp);

		ibuf[0] = lstrlenW(pMap->m_HegithFileName.data());
		fwrite((void*)ibuf, sizeof(int), 1, fp);
		fwrite((void*)pMap->m_HegithFileName.data(), sizeof(TCHAR), lstrlenW(pMap->m_HegithFileName.data()), fp);

		ibuf[0] = lstrlenW(pMap->m_ShaderFileName.data());
		fwrite((void*)ibuf, sizeof(int), 1, fp);
		fwrite((void*)pMap->m_ShaderFileName.data(), sizeof(TCHAR), lstrlenW(pMap->m_ShaderFileName.data()), fp);

		ibuf[0] = lstrlenW(pMap->m_LightFileName.data());
		fwrite((void*)ibuf, sizeof(int), 1, fp);
		fwrite((void*)pMap->m_LightFileName.data(), sizeof(TCHAR), lstrlenW(pMap->m_LightFileName.data()), fp);

		ibuf[0] = lstrlenW(m_MapData.m_pSplattAlphaTextureFile.data());
		fwrite((void*)ibuf, sizeof(int), 1, fp);
		fwrite((void*)m_MapData.m_pSplattAlphaTextureFile.data(), sizeof(TCHAR), lstrlenW(m_MapData.m_pSplattAlphaTextureFile.data()), fp);

		for (int i = 0; i < pMap->m_vSplattTextureList.size(); i++)
		{
			m_MapData.m_pSplattTextureFile.push_back(FixupName(pMap->m_vSplattTextureList[i]->m_szPath + pMap->m_vSplattTextureList[i]->m_szName));
		}

		ibuf[0] = m_MapData.m_pSplattTextureFile.size();
		fwrite((void*)ibuf, sizeof(int), 1, fp);

		fbuf[0] = m_MapData.m_CharPos.x;
		fbuf[1] = m_MapData.m_CharPos.y;
		fbuf[2] = m_MapData.m_CharPos.z;

		fwrite((void*)fbuf, sizeof(float), 3, fp);


		for (int i = 0; i < m_MapData.m_pSplattTextureFile.size(); i++)
		{
			ibuf[0] = lstrlenW(m_MapData.m_pSplattTextureFile[i].data());
			fwrite((void*)ibuf, sizeof(int), 1, fp);
			fwrite((void*)m_MapData.m_pSplattTextureFile[i].data(), sizeof(TCHAR), lstrlenW(m_MapData.m_pSplattTextureFile[i].data()), fp);
		}
		int iWidth = pMap->m_iColumNum / pMap->m_iCellCount;
		int iHeight = pMap->m_iRowNum / pMap->m_iCellCount;
		int iCellSize = (int)pMap->m_fCellDistance;

		ibuf[0] = iWidth;
		ibuf[1] = iHeight;
		ibuf[2] = pMap->m_iCellCount;
		ibuf[3] = iCellSize;

		fwrite((void*)ibuf, sizeof(int), 4, fp);

		ibuf[0] = pMap->m_VerTex.size();
		fwrite((void*)ibuf, sizeof(int), 1, fp);
		int layer = 0;
		for (int iVertex = 0; iVertex < pMap->m_VerTex.size(); iVertex++)
		{

			fbuf[0] = pMap->m_VerTex[iVertex].p.x; fbuf[1] = pMap->m_VerTex[iVertex].p.y; fbuf[2] = pMap->m_VerTex[iVertex].p.z;
			fbuf[3] = pMap->m_VerTex[iVertex].n.x; fbuf[4] = pMap->m_VerTex[iVertex].n.y; fbuf[5] = pMap->m_VerTex[iVertex].n.z;
			fbuf[6] = pMap->m_VerTex[iVertex].c.x; fbuf[7] = pMap->m_VerTex[iVertex].c.y; fbuf[8] = pMap->m_VerTex[iVertex].c.z; fbuf[9] = pMap->m_VerTex[iVertex].c.w;
			fbuf[10] = pMap->m_VerTex[iVertex].t.x; fbuf[11] = pMap->m_VerTex[iVertex].t.y;

			fwrite((void*)fbuf, sizeof(float), 12, fp);

		};

		ibuf[0] = pMap->m_IndexData.size();
		fwrite((void*)ibuf, sizeof(int), 1, fp);
		for (int Index = 0; Index < pMap->m_IndexData.size(); Index++)
		{
			ibuf[0] = pMap->m_IndexData[Index];
			fwrite((void*)ibuf, sizeof(int), 1, fp);




		}
		ibuf[0] = m_MapData.m_sQTData.m_ObjList.size();
		fwrite((void*)ibuf, sizeof(int), 1, fp);
		for (int iObj = 0; iObj < m_MapData.m_sQTData.m_ObjList.size(); iObj++)
		{
			JH::OBJECT &OBJ =
				m_MapData.m_sQTData.m_ObjList[iObj];

			ibuf[0] = OBJ.m_MapObj->m_iQuadTreeIndex;
			fwrite((void*)ibuf, sizeof(int), 1, fp);

			Dbuf[0] = OBJ.m_MapObj->m_Flag;
			fwrite((void*)Dbuf, sizeof(DWORD), 1, fp);

			ibuf[0] = lstrlenW(OBJ.m_MapObj->m_SkinName.data());
			fwrite((void*)ibuf, sizeof(int), 1, fp);
			fwrite((void*)OBJ.m_MapObj->m_SkinName.data(), sizeof(TCHAR), lstrlenW(OBJ.m_MapObj->m_SkinName.data()), fp);
			ibuf[0] = lstrlenW(OBJ.m_MapObj->m_BoneName.data());
			fwrite((void*)ibuf, sizeof(int), 1, fp);

			fwrite((void*)OBJ.m_MapObj->m_BoneName.data(), sizeof(TCHAR), lstrlenW(OBJ.m_MapObj->m_SkinName.data()), fp);






			fbuf[0] = OBJ.m_MapObj->m_matWorld._11; fbuf[1] = OBJ.m_MapObj->m_matWorld._12;
			fbuf[2] = OBJ.m_MapObj->m_matWorld._13; fbuf[3] = OBJ.m_MapObj->m_matWorld._14;

			fbuf[4] = OBJ.m_MapObj->m_matWorld._21; fbuf[5] = OBJ.m_MapObj->m_matWorld._22;
			fbuf[6] = OBJ.m_MapObj->m_matWorld._23; fbuf[7] = OBJ.m_MapObj->m_matWorld._24;

			fbuf[8] = OBJ.m_MapObj->m_matWorld._31; fbuf[9] = OBJ.m_MapObj->m_matWorld._32;
			fbuf[10] = OBJ.m_MapObj->m_matWorld._33; fbuf[11] = OBJ.m_MapObj->m_matWorld._34;

			fbuf[12] = OBJ.m_MapObj->m_matWorld._41; fbuf[13] = OBJ.m_MapObj->m_matWorld._42;
			fbuf[14] = OBJ.m_MapObj->m_matWorld._43; fbuf[15] = OBJ.m_MapObj->m_matWorld._44;





			fbuf[16] = OBJ.m_MapObj->m_Box.vCenter.x; fbuf[17] = OBJ.m_MapObj->m_Box.vCenter.y; fbuf[18] = OBJ.m_MapObj->m_Box.vCenter.z;
			fbuf[19] = OBJ.m_MapObj->m_Box.vMin.x; fbuf[20] = OBJ.m_MapObj->m_Box.vMin.y; fbuf[21] = OBJ.m_MapObj->m_Box.vMin.z;
			fbuf[22] = OBJ.m_MapObj->m_Box.vMax.x; fbuf[23] = OBJ.m_MapObj->m_Box.vMin.y; fbuf[24] = OBJ.m_MapObj->m_Box.vMin.z;



			fbuf[25] = OBJ.m_MapObj->m_Box.vAxis[0].x; fbuf[26] = OBJ.m_MapObj->m_Box.vAxis[0].y; fbuf[27] = OBJ.m_MapObj->m_Box.vAxis[0].z;
			fbuf[28] = OBJ.m_MapObj->m_Box.vAxis[1].x; fbuf[29] = OBJ.m_MapObj->m_Box.vAxis[1].y; fbuf[30] = OBJ.m_MapObj->m_Box.vAxis[1].z;



			fbuf[31] = OBJ.m_MapObj->m_Box.vAxis[2].x; fbuf[32] = OBJ.m_MapObj->m_Box.vAxis[2].y; fbuf[33] = OBJ.m_MapObj->m_Box.vAxis[2].z;
			fbuf[34] = OBJ.m_MapObj->m_Box.fExtent[0]; fbuf[35] = OBJ.m_MapObj->m_Box.fExtent[1]; fbuf[36] = OBJ.m_MapObj->m_Box.fExtent[2];

			fwrite((void*)fbuf, sizeof(float), 37, fp);


		}
		/*ibuf[0] = pQuadTree->m_LevelList.size();
		fwrite((void*)ibuf, sizeof(float), 1, fp);
		for (auto Lv : pQuadTree->m_LevelList)
		{
			ibuf[0] = Lv.size();
			fwrite((void*)ibuf, sizeof(float), 1, fp);
			for(auto pNode :Lv)
			{
				if (pNode != nullptr)
				{
					ibuf[0] = 1;
					fwrite((void*)ibuf, sizeof(float), 1, fp);
					SaveToQuadTree(pNode, fp);
				}
				else
				{
					ibuf[0] = 0;
					fwrite((void*)ibuf, sizeof(float), 1, fp);
				}
			}
			
		}*/
	

		fclose(fp);

		return true;
	}
	INT	JH_MapMgr::AddMap(const TCHAR* LoadFile)
	{
		FILE* fp;
		TCHAR  szFile[256] = { 0, };

	/*	for (auto List:m_GMDataList )
		{
			if (List->MapFileName == LoadFile)
			{
				m_CurrentMap = List;
				return 1;
			}
		}*/
		fp = _tfopen(LoadFile, _T("rb"));

		if (fp == nullptr)
		{
			return false;
		}

		//Map Basic Text Data
	
		std::shared_ptr<GAME_MAP> GMData;
		GMData = std::make_shared<GAME_MAP>();
		GMData->m_pMap = std::make_shared<JH_Map>();
		GMData->m_pQuadTree = std::make_shared<HQuadTree>();

		//Map Basic Text Data
		I_ObjMgr.m_ObjectList.clear();


		int		ibuf[6] = {};
		DWORD	Dbuf[2] = {};
		int		ilength[3] = {};
		float	fbuf[16] = {};

		TCHAR   Temp[256];
		TCHAR cbuf[256] = {};

		fread((void*)ibuf, sizeof(int), 1, fp);
		fread((void*)cbuf, sizeof(TCHAR), ibuf[0], fp);
		_tcsncpy_s(Temp, cbuf, sizeof(TCHAR)*ibuf[0]);
		memset(cbuf, 0, sizeof(TCHAR) * 256);
		m_MapData.m_BaseTextureFile = Temp;


		fread((void*)ibuf, sizeof(int), 1, fp);
		fread((void*)cbuf, sizeof(TCHAR), ibuf[0], fp);
		_tcsncpy_s(Temp, cbuf, sizeof(TCHAR)*ibuf[0]);

		memset(cbuf, 0, sizeof(TCHAR) * 256);
		m_MapData.m_NormalMapFile = Temp;

		fread((void*)ibuf, sizeof(int), 1, fp);
		fread((void*)cbuf, sizeof(TCHAR), ibuf[0], fp);
		_tcsncpy_s(Temp, cbuf, sizeof(TCHAR)*ibuf[0]);

		memset(cbuf, 0, sizeof(TCHAR) * 256);
		m_MapData.m_HeightMapFile = Temp;

		fread((void*)ibuf, sizeof(int), 1, fp);
		fread((void*)cbuf, sizeof(TCHAR), ibuf[0], fp);
		_tcsncpy_s(Temp, cbuf, sizeof(TCHAR)*ibuf[0]);

		memset(cbuf, 0, sizeof(TCHAR) * 256);
		m_MapData.m_ShaderFile = Temp;

		fread((void*)ibuf, sizeof(int), 1, fp);
		fread((void*)cbuf, sizeof(TCHAR), ibuf[0], fp);
		_tcsncpy_s(Temp, cbuf, sizeof(TCHAR)*ibuf[0]);

		memset(cbuf, 0, sizeof(TCHAR) * 256);
		m_MapData.m_LightFile = Temp;


		fread((void*)ibuf, sizeof(int), 1, fp);
		fread((void*)cbuf, sizeof(TCHAR), ibuf[0], fp);
		_tcsncpy_s(Temp, cbuf, sizeof(TCHAR)*ibuf[0]);

		memset(cbuf, 0, sizeof(TCHAR) * 256);
		m_MapData.m_pSplattAlphaTextureFile = Temp;

		fread((void*)ibuf, sizeof(int), 1, fp);

		m_MapData.m_pSplattTextureFile.resize(ibuf[0]);

		fread((void*)fbuf, sizeof(float), 3, fp);

		m_MapData.m_CharPos.x = fbuf[0];
		m_MapData.m_CharPos.y = fbuf[1];
		m_MapData.m_CharPos.z = fbuf[2];


		for (int i = 0; i < m_MapData.m_pSplattTextureFile.size(); i++)
		{

			fread((void*)ilength, sizeof(int), 1, fp);
			fread((void*)cbuf, sizeof(TCHAR), ilength[0], fp);
			_tcsncpy_s(Temp, cbuf, sizeof(TCHAR)*ilength[0]);
			m_MapData.m_pSplattTextureFile[i] = Temp;
			memset(cbuf, 0, sizeof(TCHAR) * 256);
		}

		fread((void*)ibuf, sizeof(int), 4, fp);


		m_MapData.iRow = ibuf[0];
		m_MapData.iCol = ibuf[1];
		m_MapData.iCellCount = ibuf[2];
		m_MapData.iCellSize = ibuf[3];


		fread((void*)ibuf, sizeof(int), 1, fp);


		m_MapData.m_VerTex.resize(ibuf[0]);
		int layer = 0;
		for (int iVertex = 0; iVertex < m_MapData.m_VerTex.size(); iVertex++)
		{
			fread((void*)fbuf, sizeof(float), 12, fp);
			m_MapData.m_VerTex[iVertex].p.x = fbuf[0];  m_MapData.m_VerTex[iVertex].p.y = fbuf[1];  m_MapData.m_VerTex[iVertex].p.z = fbuf[2];
			m_MapData.m_VerTex[iVertex].n.x = fbuf[3];  m_MapData.m_VerTex[iVertex].n.y = fbuf[4]; m_MapData.m_VerTex[iVertex].n.z = fbuf[5];
			m_MapData.m_VerTex[iVertex].c.x = fbuf[6];  m_MapData.m_VerTex[iVertex].c.y = fbuf[7]; m_MapData.m_VerTex[iVertex].c.z = fbuf[8]; m_MapData.m_VerTex[iVertex].c.w = fbuf[9];
			m_MapData.m_VerTex[iVertex].t.x = fbuf[10]; m_MapData.m_VerTex[iVertex].t.y = fbuf[11];



		};


		fread((void*)ibuf, sizeof(int), 1, fp);
		m_MapData.m_IndexData.resize(ibuf[0]);
		for (int Index = 0; Index < m_MapData.m_IndexData.size(); Index++)
		{

			fread((void*)ibuf, sizeof(int), 1, fp);

			m_MapData.m_IndexData[Index] = ibuf[0];


		}

		fread((void*)ibuf, sizeof(int), 1, fp);
		m_MapData.m_sQTData.m_ObjList.resize(ibuf[0]);
		for (int iObj = 0; iObj < m_MapData.m_sQTData.m_ObjList.size(); iObj++)
		{
			m_MapData.m_sQTData.m_ObjList[iObj].m_MapObj = std::make_shared<MAP_OBJ_DATA>();
			OBJECT &OBJ =
				m_MapData.m_sQTData.m_ObjList[iObj];



			fread((void*)ibuf, sizeof(int), 1, fp);
			OBJ.m_MapObj->m_iQuadTreeIndex = ibuf[0];

			fread((void*)Dbuf, sizeof(DWORD), 1, fp);
			OBJ.m_MapObj->m_Flag = Dbuf[0];

			fread((void*)ilength, sizeof(int), 1, fp);
			fread((void*)cbuf, sizeof(TCHAR), ilength[0], fp);
			_tcsncpy_s(Temp, cbuf, sizeof(TCHAR)*ilength[0]);

			memset(cbuf, 0, sizeof(TCHAR) * 256);
			OBJ.m_MapObj->m_SkinName = Temp;

			fread((void*)ilength, sizeof(int), 1, fp);
			fread((void*)cbuf, sizeof(TCHAR), ilength[0], fp);
			_tcsncpy_s(Temp, cbuf, sizeof(TCHAR)*ilength[0]);
			memset(cbuf, 0, sizeof(TCHAR) * 256);
			OBJ.m_MapObj->m_BoneName = Temp;




			fread((void*)fbuf, sizeof(float), 16, fp);


			OBJ.m_MapObj->m_matWorld._11 = fbuf[0];  OBJ.m_MapObj->m_matWorld._12 = fbuf[1];
			OBJ.m_MapObj->m_matWorld._13 = fbuf[2]; OBJ.m_MapObj->m_matWorld._14 = fbuf[3];

			OBJ.m_MapObj->m_matWorld._21 = fbuf[4]; OBJ.m_MapObj->m_matWorld._22 = fbuf[5];
			OBJ.m_MapObj->m_matWorld._23 = fbuf[6]; OBJ.m_MapObj->m_matWorld._24 = fbuf[7];

			OBJ.m_MapObj->m_matWorld._31 = fbuf[8]; OBJ.m_MapObj->m_matWorld._32 = fbuf[9];
			OBJ.m_MapObj->m_matWorld._33 = fbuf[10]; OBJ.m_MapObj->m_matWorld._34 = fbuf[11];

			OBJ.m_MapObj->m_matWorld._41 = fbuf[12];  OBJ.m_MapObj->m_matWorld._42 = fbuf[13];
			OBJ.m_MapObj->m_matWorld._43 = fbuf[14]; OBJ.m_MapObj->m_matWorld._44 = fbuf[15];




			fread((void*)fbuf, sizeof(float), 15, fp);

			OBJ.m_MapObj->m_Box.vCenter.x = fbuf[0];  OBJ.m_MapObj->m_Box.vCenter.y = fbuf[1];  OBJ.m_MapObj->m_Box.vCenter.z = fbuf[2];
			OBJ.m_MapObj->m_Box.vMin.x = fbuf[3]; OBJ.m_MapObj->m_Box.vMin.y = fbuf[4];  OBJ.m_MapObj->m_Box.vMin.z = fbuf[5];
			OBJ.m_MapObj->m_Box.vMax.x = fbuf[6];  OBJ.m_MapObj->m_Box.vMin.y = fbuf[7]; OBJ.m_MapObj->m_Box.vMin.z = fbuf[8];



			OBJ.m_MapObj->m_Box.vAxis[0].x = fbuf[9]; OBJ.m_MapObj->m_Box.vAxis[0].y = fbuf[10]; OBJ.m_MapObj->m_Box.vAxis[0].z = fbuf[11];
			OBJ.m_MapObj->m_Box.vAxis[1].x = fbuf[12]; OBJ.m_MapObj->m_Box.vAxis[1].y = fbuf[13]; OBJ.m_MapObj->m_Box.vAxis[1].z = fbuf[14];


			fread((void*)fbuf, sizeof(float), 6, fp);
			OBJ.m_MapObj->m_Box.vAxis[2].x = fbuf[0]; OBJ.m_MapObj->m_Box.vAxis[2].y = fbuf[1]; OBJ.m_MapObj->m_Box.vAxis[2].z = fbuf[2];
			OBJ.m_MapObj->m_Box.fExtent[0] = fbuf[3]; OBJ.m_MapObj->m_Box.fExtent[1] = fbuf[4];  OBJ.m_MapObj->m_Box.fExtent[2] = fbuf[5];

			

		}



		const TCHAR* HeightFile;
	
		TCHAR Tempm[256] = L"(null)";
		
		if (m_MapData.m_HeightMapFile== Tempm)
		{
			HeightFile = nullptr;
		}
		else
		{
			HeightFile = m_MapData.m_HeightMapFile.data();
		}

		LoadMap(GMData->m_pMap.get(), GMData->m_pQuadTree.get(), m_MapData.iRow, m_MapData.iCol, m_MapData.iCellCount, m_MapData.iCellSize,
			m_MapData.m_BaseTextureFile.c_str(), m_MapData.m_ShaderFile.c_str(), m_MapData.m_LightFile.c_str(),m_MapData.m_NormalMapFile.c_str(), HeightFile);
		//CreateMap(GMData.m_pMap.get(), GMData.m_pQuadTree.get(),
		//	m_MapData.iRow, m_MapData.iCol, m_MapData.iCellCount, m_MapData.iCellSize,
		//	m_MapData.m_BaseTextureFile.c_str(), m_MapData.m_NormalMapFile.c_str(), HeightFile);
		for (int iTex = 0; iTex < m_MapData.m_pSplattTextureFile.size(); iTex++)
		{
			GMData->m_pMap->AddSplattTexture(m_MapData.m_pSplattTextureFile[iTex].data(), iTex + 1);
		}
		HRESULT hr;

		GMData->m_pMap->m_pCopySrv.Reset();
		GMData->m_pMap->SetCharPos(m_MapData.m_CharPos);

		hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice,
			(m_MapData.m_pSplattAlphaTextureFile.data()), NULL, NULL, GMData->m_pMap->m_pCopySrv.GetAddressOf(), NULL);

		//SAFE_NEW(GMData->m_pQuadTree->m_pRootNode, KG_Node);

		//fread((void*)ibuf, sizeof(int), 1, fp);
		//GMData->m_pQuadTree->m_LevelList.resize(ibuf[0]);
		//for (auto Lv: GMData->m_pQuadTree->m_LevelList)
		//{
		//	fread((void*)ibuf, sizeof(int), 1, fp);
		//	Lv.resize(ibuf[0]);
		//	for ( auto pNode :Lv)
		//	{
	
		//		fread((void*)ibuf, sizeof(int), 1, fp);
		//		if (ibuf[0])
		//		{
		//			SAFE_NEW(pNode, KG_Node);
		//			LoadToQuadTree(pNode, fp);
		//		}


		//	}

		//}
		//GMData->m_pQuadTree->SetNeighborNode(GMData->m_pQuadTree->m_pRootNode);

		fclose(fp);
		m_CurrentMap = GMData;
		for (int i = 0; i < m_MapData.m_sQTData.m_ObjList.size(); i++)
		{
			OBJECT& obj = m_MapData.m_sQTData.m_ObjList[i];
			AddObject(obj);
		}
		m_GMDataList.push_back(m_CurrentMap);

	}
	INT JH_MapMgr::AddGameMap(std::shared_ptr<JH_Map> pMap, std::shared_ptr<HQuadTree> pQuadTree)
	{
		std::shared_ptr <GAME_MAP>  GM;
		GM = std::make_shared<GAME_MAP>();
		GM->m_pMap = pMap;
		GM->m_pQuadTree = pQuadTree;

		m_CurrentMap = GM;
		
		m_GMDataList.push_back(GM);
		return 1;

	}
	//오브젝트 바운딩 박스 추가해야함
	INT JH_MapMgr::AddObject(OBJECT Obj)
	{

		std::shared_ptr<CBY::CBY_Object> Object;
		Object =std::make_shared<CBY::CBY_Object>();
		Object->Create(m_pd3dDevice, m_pContext, L"../../data/shader/ObjectShader.txt", nullptr, "VSOBJECT", "PS");
		Object->SkinLoad(Obj.m_MapObj->m_SkinName);
		Object->BoneLoad(Obj.m_MapObj->m_BoneName);

		Object->m_matWorld = Obj.m_MapObj->m_matWorld;
		std::shared_ptr<JH_MapObj> MapObj= std::make_shared< JH_MapObj>();
		MapObj->SetBoneName(Obj.m_MapObj->m_BoneName);
		MapObj->SetSkinName(Obj.m_MapObj->m_SkinName);
		MapObj->SetQuadIndex(Obj.m_MapObj->m_iQuadTreeIndex);
		MapObj->SetObj(Object);
		MapObj->SetFlag(Obj.m_MapObj->m_Flag);
		MapObj->SetID(m_ObjID++);
		
		MapObj->GetObj()->SetMatrix(&MapObj->GetObj()->m_matWorld, &m_pCamera->m_View, &m_pCamera->m_Proj);
		
		int ind=MapObj->GetQuadIndex();
		m_CurrentMap->m_pQuadTree->m_NodeList[ind]->m_ObjList.insert(std::make_pair(MapObj->GetID(), MapObj));
		m_CurrentMap->m_pQuadTree->m_ObjectList.insert(std::make_pair(MapObj->GetID(), MapObj));
		I_ObjMgr.AddObject(MapObj);
		//m_CurrentMap->m_pQuadTree->GetObjectAddNode(MapObj);
		return 1;

	}

	bool JH_MapMgr::CreateMap(JH_Map* pMap, HQuadTree* pQuad, int iWidth,
		int iHeight,
		int iCellCount,
		int iCellSize,
		const TCHAR* pTexturFileName,
		const TCHAR* pNormalMapFileName,
		const TCHAR* pHeightMapFileName)
	{


		if (pMap != nullptr)
		{
			pMap->Release();
			pQuad->Release();
			
		}

		pMap->m_vHeightList.resize(m_MapData.m_fHegihtList.size());
		if (pHeightMapFileName)
		{
			pMap->CreateHeightMap(m_pd3dDevice, m_pContext, pHeightMapFileName);
		}
		else
		{
			pMap->m_iCellCount = iCellCount;
			pMap->m_iRowNum = iCellCount * iWidth + 1;
			pMap->m_iColumNum = iCellCount * iHeight + 1;
		}
		if (m_MapData.m_fHegihtList.size() > 0)
		{
			std::copy(m_MapData.m_fHegihtList.begin(),
				m_MapData.m_fHegihtList.end(),
				pMap->m_vHeightList.begin());
		}



		//	CreateSplattingTexture();

		pMap->SetMapDesc(pTexturFileName, L"../../data/Shader/JHMapShader.txt", pMap->m_iRowNum, pMap->m_iColumNum, iCellSize, 1.0f);


		pMap->m_pNormMapFileName = pNormalMapFileName;
		pMap->Load(m_pd3dDevice, m_pContext);


		pQuad->Build(pMap, m_pCamera);
		pQuad->m_pSelect = m_pSelect;


		return true;
	}
	bool JH_MapMgr::LoadMap(JH_Map* pMap, HQuadTree* pQuad, int iWidth,
		int iHeight,
		int iCellCount,
		int iCellSize,
		const TCHAR* pTexturFileName,
		const TCHAR* pShaderFileName,
		const TCHAR* pLightShaderName,
		const TCHAR* pNormalMapFileName,
		const TCHAR* pHeightMapFileName)
	{


		if (pMap != nullptr)
		{
			pMap->Release();
			pQuad->Release();

		}

		pMap->m_iCellCount = iCellCount;
		pMap->m_iRowNum = iCellCount * iWidth + 1;
		pMap->m_iColumNum = iCellCount * iHeight + 1;
		
		pMap->m_TextureFileName = pTexturFileName;
		pMap->m_pNormMapFileName = pNormalMapFileName;
		pMap->m_HegithFileName = pHeightMapFileName;
		pMap->m_ShaderFileName = pShaderFileName;
		pMap->m_LightFileName = pLightShaderName;


		pMap->m_VerTex.resize(m_MapData.m_VerTex.size());
		std::copy(m_MapData.m_VerTex.begin(), m_MapData.m_VerTex.end(), pMap->m_VerTex.begin());
		pMap->m_IndexData.resize(m_MapData.m_IndexData.size());
		std::copy(m_MapData.m_IndexData.begin(), m_MapData.m_IndexData.end(), pMap->m_IndexData.begin());
		pMap->LoadMap(m_pd3dDevice, m_pContext, pShaderFileName,pTexturFileName);

		

		pQuad->Build(pMap, m_pCamera);
		pQuad->m_pSelect = m_pSelect;


		return true;
	}

	void JH_MapMgr::SaveToQuadTree(KG_Node* pNode, FILE* fp)
	{
		if (pNode == nullptr)return;

		BOOL	BL[1];
		int		ibuf[6] = {};
		DWORD	Dbuf[5] = {};
		float	fbuf[40] = {};
		TCHAR Temp[256] = {};
		TCHAR cbuf[256] = {};

		ibuf[0] = pNode->m_iQuadTreeIndex;
		fwrite((void*)ibuf, sizeof(int), 1, fp);

		Dbuf[0] = pNode->m_dwDepth;
		fwrite((void*)Dbuf, sizeof(DWORD), 1, fp);

		Dbuf[0] = pNode->m_dwCorner[0];
		Dbuf[1] = pNode->m_dwCorner[1];
		Dbuf[2] = pNode->m_dwCorner[2];
		Dbuf[3] = pNode->m_dwCorner[3];
		fwrite((void*)Dbuf, sizeof(DWORD), 4, fp);
		
		Dbuf[0] = pNode->m_dwPositionIndex[0];
		Dbuf[1] = pNode->m_dwPositionIndex[1];
		fwrite((void*)Dbuf, sizeof(DWORD), 2, fp);


		BL[0] = pNode->m_isLeaf;
		fwrite((void*)BL, sizeof(BOOL), 1, fp);

		fbuf[0] = pNode->m_BoxPlane[0].fA; fbuf[1] = pNode->m_BoxPlane[0].fB; fbuf[2] = pNode->m_BoxPlane[0].fC; fbuf[3] = pNode->m_BoxPlane[0].fD;
		fbuf[4] = pNode->m_BoxPlane[1].fA; fbuf[5] = pNode->m_BoxPlane[1].fB; fbuf[6] = pNode->m_BoxPlane[1].fC; fbuf[7] = pNode->m_BoxPlane[1].fD;
		fbuf[8] = pNode->m_BoxPlane[2].fA; fbuf[9] = pNode->m_BoxPlane[2].fB; fbuf[10] = pNode->m_BoxPlane[2].fC; fbuf[11] = pNode->m_BoxPlane[2].fD;
		fbuf[12] = pNode->m_BoxPlane[3].fA; fbuf[13] = pNode->m_BoxPlane[3].fB; fbuf[14] = pNode->m_BoxPlane[3].fC; fbuf[15] = pNode->m_BoxPlane[3].fD;


		fbuf[16] = pNode->m_Box.vCenter.x;	fbuf[17] = pNode->m_Box.vCenter.y;		fbuf[18] = pNode->m_Box.vCenter.z;
		fbuf[19] = pNode->m_Box.vMin.x;		fbuf[20] = pNode->m_Box.vMin.y;			fbuf[21] = pNode->m_Box.vMin.z;
		fbuf[22] = pNode->m_Box.vMax.x;		fbuf[23] = pNode->m_Box.vMin.y;			fbuf[24] = pNode->m_Box.vMin.z;



		fbuf[25] = pNode->m_Box.vAxis[0].x; fbuf[26] = pNode->m_Box.vAxis[0].y;		fbuf[27] = pNode->m_Box.vAxis[0].z;
		fbuf[28] = pNode->m_Box.vAxis[1].x; fbuf[29] = pNode->m_Box.vAxis[1].y;		fbuf[30] = pNode->m_Box.vAxis[1].z;



		fbuf[31] = pNode->m_Box.vAxis[2].x; fbuf[32] = pNode->m_Box.vAxis[2].y;		fbuf[33] = pNode->m_Box.vAxis[2].z;
		fbuf[34] = pNode->m_Box.fExtent[0]; fbuf[35] = pNode->m_Box.fExtent[1]; fbuf[36] = pNode->m_Box.fExtent[2];


		fwrite((void*)fbuf, sizeof(float), 37, fp);

		/*ibuf[0] = pNode->m_NeighborNodeList.size();
		fwrite((void*)ibuf, sizeof(int),1, fp);
		for (auto Node : pNode->m_NeighborNodeList)
		{
			if (Node)
			{
				ibuf[0] = Node->m_iQuadTreeIndex;
				fwrite((void*)ibuf, sizeof(int), 1, fp);
			}
		}*/
		ibuf[0] = pNode->m_IndexList.size();
		fwrite((void*)ibuf, sizeof(int), 1, fp);
		for (auto Index : pNode->m_IndexList)
		{
			Dbuf[0] = Index;
			fwrite((void*)Dbuf, sizeof(DWORD), 1, fp);
		}


		/*for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->m_pChild[iNode] != nullptr)
			{
				SaveToQuadTree(pNode->m_pChild[iNode], fp);

			}
		}*/


	}
	void JH_MapMgr::LoadToQuadTree(KG_Node* pNode, FILE* fp)
	{
		if (pNode == nullptr)return;

		int		ibuf[6] = {};
		DWORD	Dbuf[4] = {};
		float	fbuf[40] = {};
	
		TCHAR Temp[256] = {};
		TCHAR cbuf[256] = {};
		BOOL  Bf [1] = { };

		

		fread((void*)ibuf, sizeof(int), 1, fp);
		pNode->m_iQuadTreeIndex = ibuf[0];
		fread((void*)Dbuf, sizeof(DWORD), 1, fp);
		pNode->m_dwDepth = Dbuf[0];
		fread((void*)Dbuf, sizeof(DWORD), 4, fp);
		pNode->m_dwCorner[0] = Dbuf[0];
		pNode->m_dwCorner[1] = Dbuf[1];
		pNode->m_dwCorner[2] = Dbuf[2];
		pNode->m_dwCorner[3] = Dbuf[3];

		fread((void*)Dbuf, sizeof(DWORD), 2, fp);
		pNode->m_dwPositionIndex[0]	=	Dbuf[0];
		pNode->m_dwPositionIndex[1] =	Dbuf[1];
		

		fread((void*)Bf, sizeof(BOOL), 1, fp);
		pNode->m_isLeaf = Bf[0];

	/*	if (!pNode->m_isLeaf)
		{
			for (int iNode = 0; iNode < 4; iNode++)
			{
				SAFE_NEW(pNode->m_pChild[iNode], KG_Node);
			}
		}*/


		fread((void*)fbuf, sizeof(float), 37, fp);
		pNode->m_BoxPlane[0].fA = fbuf[0];  pNode->m_BoxPlane[0].fB =  fbuf[1];  pNode->m_BoxPlane[0].fC = fbuf[2];   pNode->m_BoxPlane[0].fD =  fbuf[3];
		pNode->m_BoxPlane[1].fA = fbuf[4];  pNode->m_BoxPlane[1].fB = fbuf[5];   pNode->m_BoxPlane[1].fC = fbuf[6];   pNode->m_BoxPlane[1].fD = fbuf[7];
		pNode->m_BoxPlane[2].fA = fbuf[8];  pNode->m_BoxPlane[2].fB = fbuf[9];   pNode->m_BoxPlane[2].fC = fbuf[10];  pNode->m_BoxPlane[2].fD = fbuf[11];
		pNode->m_BoxPlane[3].fA = fbuf[12]; pNode->m_BoxPlane[3].fB = fbuf[13];  pNode->m_BoxPlane[3].fC = fbuf[14];  pNode->m_BoxPlane[3].fD = fbuf[15];


		 pNode->m_Box.vCenter.x=fbuf[16];		fbuf[17] = pNode->m_Box.vCenter.y		= fbuf[17];			fbuf[18] = pNode->m_Box.vCenter.z	=	fbuf[18];
		 pNode->m_Box.vMin.x = fbuf[19];		fbuf[20] = pNode->m_Box.vMin.y			= fbuf[20];			fbuf[21] = pNode->m_Box.vMin.z		=	fbuf[21];
		 pNode->m_Box.vMax.x = fbuf[22];		fbuf[23] = pNode->m_Box.vMin.y			= fbuf[23];			fbuf[24] = pNode->m_Box.vMin.z		=	fbuf[24];



		pNode->m_Box.vAxis[0].x = fbuf[25]; pNode->m_Box.vAxis[0].y	=	fbuf[26];			pNode->m_Box.vAxis[0].z=fbuf[27];
		pNode->m_Box.vAxis[1].x = fbuf[28]; pNode->m_Box.vAxis[1].y = fbuf[29];				pNode->m_Box.vAxis[1].z=fbuf[30];



		pNode->m_Box.vAxis[2].x= fbuf[31]; pNode->m_Box.vAxis[2].y= fbuf[32];		pNode->m_Box.vAxis[2].z= fbuf[33];
		pNode->m_Box.fExtent[0]= fbuf[34]; pNode->m_Box.fExtent[1]= fbuf[35];		pNode->m_Box.fExtent[2]= fbuf[36];


		
		///*fread((void*)ibuf, sizeof(int), 1, fp);
		//pNode->m_NeighborNodeList.resize(ibuf[0]);

		//for (auto Node : pNode->m_NeighborNodeList)
		//{
		//	if (Node)
		//	{
		//		fread((void*)ibuf, sizeof(int), 1, fp);
		//		Node->m_iQuadTreeIndex = ibuf[0];
		//	}
		//}*/

		fread((void*)ibuf, sizeof(int), 1, fp);
		pNode->m_IndexList.resize(ibuf[0]);
		for (auto Index : pNode->m_IndexList)
		{
			fread((void*)Dbuf, sizeof(DWORD), 1, fp);
			Index = Dbuf[0];
		}


		/*for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->m_pChild[iNode] != nullptr)
			{
				LoadToQuadTree(pNode->m_pChild[iNode], fp);

			}
		}*/


	}
	bool JH_MapMgr::Release()
	{
		return true;
	}
}