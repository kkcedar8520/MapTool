#pragma once
#include "JH_MapMgr.h"
#include"JH_ObjMgr.h"






namespace JH {
	JH_MapMgr::JH_MapMgr()
	{
	}


	JH_MapMgr::~JH_MapMgr()
	{
	}
	void JH_MapMgr::Set(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, KG_Camera* pCamera, KG_Select& select)
	{
		m_pd3dDevice = pDevice;
		m_pContext = pContext;
		m_pCamera = pCamera;
		m_Select = select;
	}
	INT	JH_MapMgr::AddMap(const TCHAR* LoadFile)
	{
		FILE* fp;
		TCHAR  szFile[256] = { 0, };

		fp = _tfopen(LoadFile, _T("rt"));

		if (fp == nullptr)
		{
			return false;
		}

		//Map Basic Text Data

		GAME_MAP GMData;
		GMData.m_pMap = std::make_shared<JH_Map>();
		GMData.m_pQuadTree = std::make_shared<HQuadTree>();

		//Map Basic Text Data
		I_ObjMgr.m_ObjectList.clear();

		TCHAR   Temp[256];
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%s %s\n"), m_pString,
			Temp);
		m_MapData.m_BaseTextureFile = Temp;

		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%s %s\n"), m_pString, Temp);
		m_MapData.m_NormalMapFile = Temp;

		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%s %s\n "), m_pString, Temp);
		m_MapData.m_HeightMapFile = Temp;

		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%s %s\n "), m_pString, Temp);

		m_MapData.m_ShaderFile = Temp;



		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%s %s\n"), m_pString,
			Temp);
		m_MapData.m_pSplattAlphaTextureFile = Temp;




		//Splatt Texture
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%s %d\n"), m_pString, &m_iTemp);

		m_MapData.m_pSplattTextureFile.resize(m_iTemp);
		for (int i = 0; i < m_MapData.m_pSplattTextureFile.size(); i++)
		{

			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("%d %s"), &m_iTemp, Temp);
			m_MapData.m_pSplattTextureFile[i] = Temp;
		}
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%s %d %d %d %d\n"), m_pString, &m_MapData.iRow, &m_MapData.iCol, &m_MapData.iCellCount, &m_MapData.iCellSize);
		// Vertex Height Data
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%s %d\n "), m_pString, &m_iTemp);

		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T(" %s\n"), m_pString);

		m_MapData.m_fHegihtList.resize(m_iTemp);
		int layer = 0;
		for (int iVertex = 0; iVertex < m_iTemp; iVertex++)
		{

			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("%f "), &m_MapData.m_fHegihtList[iVertex]);


		}

		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%s %d \n"), m_pString, &m_iTemp);

		m_MapData.m_sQTData.m_ObjList.resize(m_iTemp);
		TCHAR  Buf[255];
		for (int iObj = 0; iObj < m_MapData.m_sQTData.m_ObjList.size(); iObj++)
		{
			OBJECT &OBJ =
				m_MapData.m_sQTData.m_ObjList[iObj];
			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("%d \n"), &m_iTemp);

			OBJ.m_MapObj.SetQuadIndex(m_iTemp);

			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("%s \n"), Buf);
			OBJ.m_MapObj.SetSkinName(Buf);
			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("%s \n"), Buf);
			OBJ.m_MapObj.SetBoneName(Buf);

			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("\t%s\n"), Buf);

			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("\t%f %f %f %f\n"),
				&OBJ.m_MapObj.m_matWorld._11, &OBJ.m_MapObj.m_matWorld._12, &OBJ.m_MapObj.m_matWorld._13, &OBJ.m_MapObj.m_matWorld._14);
			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("\t%f %f %f %f\n"),
				&OBJ.m_MapObj.m_matWorld._21, &OBJ.m_MapObj.m_matWorld._22, &OBJ.m_MapObj.m_matWorld._23, &OBJ.m_MapObj.m_matWorld._24);
			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("\t%f %f %f %f\n"),
				&OBJ.m_MapObj.m_matWorld._31, &OBJ.m_MapObj.m_matWorld._32, &OBJ.m_MapObj.m_matWorld._33, &OBJ.m_MapObj.m_matWorld._34);
			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("\t%f %f %f %f\n"),
				&OBJ.m_MapObj.m_matWorld._41, &OBJ.m_MapObj.m_matWorld._42, &OBJ.m_MapObj.m_matWorld._43, &OBJ.m_MapObj.m_matWorld._44);



			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("\t%f %f %f\n"),
				&OBJ.m_MapObj.m_Box.vCenter.x, &OBJ.m_MapObj.m_Box.vCenter.y, &OBJ.m_MapObj.m_Box.vCenter.z);
			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("\t%f %f %f\n"),
				&OBJ.m_MapObj.m_Box.vMin.x, &OBJ.m_MapObj.m_Box.vMin.y, &OBJ.m_MapObj.m_Box.vMin.z);
			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("\t%f %f %f\n"),
				&OBJ.m_MapObj.m_Box.vMax.x, &OBJ.m_MapObj.m_Box.vMax.y, &OBJ.m_MapObj.m_Box.vMax.z);


			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("\t%f %f %f\n"),
				&OBJ.m_MapObj.m_Box.vAxis[0].x, &OBJ.m_MapObj.m_Box.vAxis[0].y, &OBJ.m_MapObj.m_Box.vAxis[0].z);
			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("\t%f %f %f\n"),
				&OBJ.m_MapObj.m_Box.vAxis[1].x, &OBJ.m_MapObj.m_Box.vAxis[1].y, &OBJ.m_MapObj.m_Box.vAxis[1].z);
			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("\t%f %f %f\n"),
				&OBJ.m_MapObj.m_Box.vAxis[2].x, &OBJ.m_MapObj.m_Box.vAxis[2].y, &OBJ.m_MapObj.m_Box.vAxis[2].z);

			_fgetts(m_pBuffer, 256, fp);
			_stscanf(m_pBuffer, _T("\t%f %f %f\n"),
				&OBJ.m_MapObj.m_Box.fExtent[0], &OBJ.m_MapObj.m_Box.fExtent[1], &OBJ.m_MapObj.m_Box.fExtent[2]);
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
		CreateMap(GMData.m_pMap.get(), GMData.m_pQuadTree.get(),
			m_MapData.iRow, m_MapData.iCol, m_MapData.iCellCount, m_MapData.iCellSize,
			m_MapData.m_BaseTextureFile.c_str(), m_MapData.m_NormalMapFile.c_str(), HeightFile);
		for (int iTex = 0; iTex < m_MapData.m_pSplattTextureFile.size(); iTex++)
		{
			GMData.m_pMap->AddSplattTexture(m_MapData.m_pSplattTextureFile[iTex].data(), iTex + 1);
		}
		HRESULT hr;

		GMData.m_pMap->m_pCopySrv.Reset();


		hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice,
			(m_MapData.m_pSplattAlphaTextureFile.data()), NULL, NULL, GMData.m_pMap->m_pCopySrv.GetAddressOf(), NULL);


		m_CurrentMap = GMData;
		for (int i = 0; i < m_MapData.m_sQTData.m_ObjList.size(); i++)
		{
			OBJECT& obj = m_MapData.m_sQTData.m_ObjList[i];
			AddObject(obj);
		}
		m_GMDataList.push_back(m_CurrentMap);

	}
	//오브젝트 바운딩 박스 추가해야함
	INT JH_MapMgr::AddObject(OBJECT Obj)
	{
		static int ID;
		std::shared_ptr<CBY::CBY_Object> Object;
		Object =std::make_shared<CBY::CBY_Object>();
		Object->Create(m_pd3dDevice, m_pContext, L"../../data/shader/ObjectShader.txt", nullptr, "VSOBJECT", "PS");
		Object->SkinLoad(Obj.m_MapObj.GetSkinName());
		Object->BoneLoad(Obj.m_MapObj.GetBoneName());

		Object->m_matWorld = Obj.m_MapObj.m_matWorld;
		std::shared_ptr<JH_MapObj> MapObj;
		MapObj = std::make_shared< JH_MapObj>();
		*MapObj = Obj.m_MapObj;

		MapObj->SetID(ID++);

		I_ObjMgr.AddObject(MapObj);
		m_CurrentMap.m_pQuadTree->FindObjectNode(m_CurrentMap.m_pQuadTree->m_pRootNode, MapObj);
		m_CurrentMap.m_pQuadTree->m_pFindNode->m_ObjList.insert(make_pair(MapObj->GetID(), MapObj));
		m_CurrentMap.m_pQuadTree->CreateBB(m_CurrentMap.m_pQuadTree->m_pFindNode);
		m_CurrentMap.m_ObjectList.insert(make_pair(MapObj->GetID(), Object));
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
		pQuad->m_pSelect = &m_Select;


		return true;
	}

	bool JH_MapMgr::Release()
	{
		return true;
	}
}