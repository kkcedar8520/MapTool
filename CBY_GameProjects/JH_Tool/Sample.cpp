#pragma once
#include "pch.h"
#include "Sample.h"
#include"KG_DxState.h"
#include"KG_Input.h"
#include"KG_ShapeMap.h"
#include"LightMgr.h"


	

#pragma warning( disable:4005 )
//void Sample::ResetComponent()
//{
//	m_Object.reset();
//	m_Object = nullptr;
//	m_se
//}
bool Sample::ObjectDelete()
{
	if (!m_pSelectMapObj)  return false;
	JH::KG_Node* pNode=(JH::KG_Node*)m_pSelectMapObj->GetNode();
	pNode->m_ObjList.erase(m_pSelectMapObj->GetID());
	m_QuadTree->m_ObjectList.erase(m_pSelectMapObj->GetID());

	m_pSelectMapObj = nullptr;
	return true;
}
void Sample::ResetSRTValue()
{
	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;
	m_fScaleZ = 1.0f;
	m_fPitch = 0.0f;
	m_fYaw = 0.0f;
	m_fRoll = 0.0f;
	D3DXMatrixIdentity(&m_matMove);

}
void Sample::ObjRotation()
{
	D3DXQUATERNION qRot;
	D3DXVECTOR3	vScale, vTrans;
	
	D3DXMATRIX matRot, matScale, matTrans, matDecomposRot, matWorld;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matWorld);
	if (!m_pSelectMapObj)  return;

	float fYaw = m_fYaw * D3DX_PI;
	float fPit = m_fPitch * D3DX_PI;
	float fRoll = m_fRoll * D3DX_PI;


	D3DXQuaternionRotationYawPitchRoll(&qRot, fYaw, fPit, fRoll);
	D3DXVECTOR3 vCenter= m_pSelectMapObj->GetObj()->GetCharBox().vCenter;
	D3DXMatrixAffineTransformation(&matRot, 1.0f, NULL, &qRot, &vCenter);
	D3DXMatrixDecompose(&vScale, &qRot, &vTrans, &m_pSelectMapObj->GetObj()->m_matWorld);
	
	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z); 
	D3DXMatrixRotationQuaternion(&matDecomposRot, &qRot);

	
	matWorld = matScale* matRot* matDecomposRot;
	matWorld._41 = vTrans.x;
	matWorld._42 = vTrans.y;
	matWorld._43 = vTrans.z;
	m_pSelectMapObj->GetObj()->m_matWorld = matWorld;
}
void Sample::ObjTranslation()
{
	D3DXQUATERNION qRot;
	D3DXMATRIX matRot;
	if (!m_pSelectMapObj)  return; 

	m_pSelectMapObj->GetObj()->m_matWorld ._41= m_matMove._41;
	m_pSelectMapObj->GetObj()->m_matWorld._42 = m_matMove._42;
	m_pSelectMapObj->GetObj()->m_matWorld._43 = m_matMove._43;



}
void Sample::ObjScale()
{
	D3DXQUATERNION qRot;
	D3DXVECTOR3	vScale, vTrans;

	D3DXMATRIX matRot, matScale, matTrans, matDecomposRot, matWorld, matDeSacle;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matDeSacle);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matWorld);	
	if (!m_pSelectMapObj)  return;


	D3DXMatrixDecompose(&vScale, &qRot, &vTrans, &m_pSelectMapObj->GetObj()->m_matWorld);
	D3DXMatrixScaling(&matDeSacle, vScale.x, vScale.y, vScale.z);
	D3DXMatrixRotationQuaternion(&matDecomposRot, &qRot);
	D3DXMatrixScaling(&matScale, m_fScaleX, m_fScaleY, m_fScaleZ);

	matWorld = matScale*matDeSacle*matDecomposRot;
	matWorld._41 = vTrans.x;
	matWorld._42 = vTrans.y;
	matWorld._43 = vTrans.z;	
	m_pSelectMapObj->GetObj()->m_matWorld = matWorld;
}
TCHAR* Sample::FixupName(T_STR name)
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
TCHAR* Sample::SaveFileDlg(TCHAR* szExt, TCHAR* szTitle)
{
	OPENFILENAME    ofn;
	TCHAR            szFile[256], szFileTitle[256];
	static TCHAR     *szFilter;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	_tcscpy_s(szFile, _T("*."));
	_tcscat_s(szFile, szExt);
	_tcscat_s(szFile, _T("\0"));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFilter = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0L;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = szTitle;
	ofn.Flags = 0L;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = szExt;

	if (!GetSaveFileName(&ofn)) //GetOpenFileName
	{
		return NULL;
	}

	return szFile;
}
bool	Sample::SaveDataReset()
{

	m_sMapData.m_pSplattAlphaTextureFile.clear();
	m_sMapData.m_pSplattTextureFile.clear();
	return true;
}
bool  Sample::SaveMapData(const TCHAR* LoadFile)
{
	m_sMapData.Reset();


	
		OBJECT OBJ;
		for (auto Obj :  m_QuadTree->m_ObjectList)
		{
			OBJ.m_MapObj = std::make_shared<MAP_OBJ_DATA>();
			 OBJ.m_MapObj->m_matWorld = Obj.second->GetObj()->m_matWorld;
			 OBJ.m_MapObj->m_BoneName = Obj.second->GetBoneName();
			 OBJ.m_MapObj->m_SkinName = Obj.second->GetSkinName();
			 OBJ.m_MapObj->m_iQuadTreeIndex = Obj.second->GetQuadIndex();
			 OBJ.m_MapObj->m_Box = Obj.second->GetObj()->GetCharBox();
			m_sMapData.m_sQTData.m_ObjList.push_back(OBJ);
		}
	

	m_sMapData.m_pSplattAlphaTextureFile=m_pSPTAFile;
	FILE* fp;
	TCHAR  szFile[256] = { 0, };
	fp = _tfopen(LoadFile, _T("wt"));


	_ftprintf(fp, _T("%s %s\n"), L"Map",
		m_Map->m_TextureFileName);
	_ftprintf(fp, _T("%s %s\n "), L"NorMalMap",
		m_Map->m_pNormMapFileName);
	_ftprintf(fp, _T("%s %s\n "), L"HeightMap",
		m_Map->m_HegithFileName);

	_ftprintf(fp, _T("%s %s\n"), L"Shader",
		m_Map->m_ShaderFileName);

	


	_ftprintf(fp, _T("%s %s\n"), L"ALPHATEX",
		m_sMapData.m_pSplattAlphaTextureFile.data());


	for (int i = 0; i < m_Map->m_vSplattTextureList.size(); i++)
	{
		m_sMapData.m_pSplattTextureFile.push_back(FixupName(m_Map->m_vSplattTextureList[i]->m_szPath+ m_Map->m_vSplattTextureList[i]->m_szName));
	}

	_ftprintf(fp, _T("%s %d\n"), L"SPT_TEX_NUM", m_sMapData.m_pSplattTextureFile.size());
	for (int i = 0; i < m_sMapData.m_pSplattTextureFile.size(); i++)
	{
		_ftprintf(fp, _T("%d %s\n"), i, m_sMapData.m_pSplattTextureFile[i].data());
	}

	int iWidth = m_Map->m_iColumNum / m_Map->m_iCellCount;
	int iHeight = m_Map->m_iRowNum / m_Map->m_iCellCount;
	int iCellSize = (int)m_Map->m_fCellDistance;
	_ftprintf(fp, _T("%s %d %d %d %d\n"), L"MapSizeData", iWidth, iHeight, m_Map->m_iCellCount, iCellSize);

	_ftprintf(fp, _T("%s %d\n %s\n"), L"VertexNum", m_Map->m_VerTex.size(), L"VertexHegiht");
	int layer = 0;
	for (int iVertex = 0; iVertex < m_Map->m_VerTex.size(); iVertex++)
	{

		_ftprintf(fp, _T("%10.4f \n"), m_Map->m_VerTex[iVertex].p.y);


	}
	_ftprintf(fp, _T("%s %d \n"), L"QUAD_OBJECT_NUM ", m_sMapData.m_sQTData.m_ObjList.size());
	for (int  iObj = 0; iObj < m_sMapData.m_sQTData.m_ObjList.size(); iObj++)
	{
		OBJECT &OBJ=
			m_sMapData.m_sQTData.m_ObjList[iObj];
		_ftprintf(fp, _T("%d \n"),  OBJ.m_MapObj->m_iQuadTreeIndex);
		_ftprintf(fp, _T("%s \n"),  OBJ.m_MapObj->m_SkinName.data());
		_ftprintf(fp, _T("%s \n"),  OBJ.m_MapObj->m_BoneName.data());

		_ftprintf(fp, _T("\t%s\n"), _T("WORLD_MATRIX"));
		_ftprintf(fp, _T("\t%10.4f %10.4f %10.4f %10.4f\n"),
			 OBJ.m_MapObj->m_matWorld._11,  OBJ.m_MapObj->m_matWorld._12,  OBJ.m_MapObj->m_matWorld._13,  OBJ.m_MapObj->m_matWorld._14);
		_ftprintf(fp, _T("\t%10.4f %10.4f %10.4f %10.4f\n"),
			 OBJ.m_MapObj->m_matWorld._21,  OBJ.m_MapObj->m_matWorld._22,  OBJ.m_MapObj->m_matWorld._23,  OBJ.m_MapObj->m_matWorld._24);
		_ftprintf(fp, _T("\t%10.4f %10.4f %10.4f %10.4f\n"),
			 OBJ.m_MapObj->m_matWorld._31,  OBJ.m_MapObj->m_matWorld._32,  OBJ.m_MapObj->m_matWorld._33,  OBJ.m_MapObj->m_matWorld._34);
		_ftprintf(fp, _T("\t%10.4f %10.4f %10.4f %10.4f\n"),
			 OBJ.m_MapObj->m_matWorld._41,  OBJ.m_MapObj->m_matWorld._42,  OBJ.m_MapObj->m_matWorld._43,  OBJ.m_MapObj->m_matWorld._44);

		_ftprintf(fp, _T("\t%10.4f %10.4f %10.4f\n"),
			 OBJ.m_MapObj->m_Box.vCenter.x,  OBJ.m_MapObj->m_Box.vCenter.y,  OBJ.m_MapObj->m_Box.vCenter.z);
		_ftprintf(fp, _T("\t%10.4f %10.4f %10.4f\n"),
			 OBJ.m_MapObj->m_Box.vMin.x,  OBJ.m_MapObj->m_Box.vMin.y,  OBJ.m_MapObj->m_Box.vMin.z);
		_ftprintf(fp, _T("\t%10.4f %10.4f %10.4f\n"),
			 OBJ.m_MapObj->m_Box.vMax.x,  OBJ.m_MapObj->m_Box.vMax.y,  OBJ.m_MapObj->m_Box.vMax.z);


		_ftprintf(fp, _T("\t%10.4f %10.4f %10.4f\n"),
			 OBJ.m_MapObj->m_Box.vAxis[0].x,  OBJ.m_MapObj->m_Box.vAxis[0].y,  OBJ.m_MapObj->m_Box.vAxis[0].z);
		_ftprintf(fp, _T("\t%10.4f %10.4f %10.4f\n"),									   
			 OBJ.m_MapObj->m_Box.vAxis[1].x,  OBJ.m_MapObj->m_Box.vAxis[1].y,  OBJ.m_MapObj->m_Box.vAxis[1].z);
		_ftprintf(fp, _T("\t%10.4f %10.4f %10.4f\n"),									   
			 OBJ.m_MapObj->m_Box.vAxis[2].x,  OBJ.m_MapObj->m_Box.vAxis[2].y,  OBJ.m_MapObj->m_Box.vAxis[2].z);

		_ftprintf(fp, _T("\t%10.4f %10.4f %10.4f\n"),
			 OBJ.m_MapObj->m_Box.fExtent[0],  OBJ.m_MapObj->m_Box.fExtent[1],  OBJ.m_MapObj->m_Box.fExtent[2]);
	}



	fclose(fp);
	m_sMapData.Reset();
	return true;
}
bool  Sample::LoadMapData(const TCHAR* LoadFile)
{
	FILE* fp;
	TCHAR  szFile[256] = { 0, };

	fp = _tfopen(LoadFile, _T("rt"));

	if (fp == nullptr)
	{
		return false;
	}

	//Map Basic Text Data
	m_sMapData.Reset();

	TCHAR   Temp[256];
	_fgetts(m_pBuffer, 256, fp);
	_stscanf(m_pBuffer, _T("%s %s\n"), m_pString,
		Temp);
	m_sMapData.m_BaseTextureFile = Temp;

	_fgetts(m_pBuffer, 256, fp);
	_stscanf(m_pBuffer, _T("%s %s\n"), m_pString, Temp);
	m_sMapData.m_NormalMapFile = Temp;

	_fgetts(m_pBuffer, 256, fp);
	_stscanf(m_pBuffer, _T("%s %s\n "), m_pString, Temp);
	if(Temp!=L"(null)")
	m_sMapData.m_HeightMapFile = Temp;

	_fgetts(m_pBuffer, 256, fp);
	_stscanf(m_pBuffer, _T("%s %s\n "), m_pString, Temp);

	m_sMapData.m_ShaderFile = Temp;


	
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%s %s\n"), m_pString,
			Temp);
		m_sMapData.m_pSplattAlphaTextureFile = Temp;
	



	//Splatt Texture
	_fgetts(m_pBuffer, 256, fp);
	_stscanf(m_pBuffer, _T("%s %d\n"), m_pString, &m_iTemp);

	m_sMapData.m_pSplattTextureFile.resize(m_iTemp);
	for (int i = 0; i < m_sMapData.m_pSplattTextureFile.size(); i++)
	{

		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%d %s"), &m_iTemp, Temp);
		m_sMapData.m_pSplattTextureFile[i] = Temp;
	}
	_fgetts(m_pBuffer, 256, fp);
	_stscanf(m_pBuffer, _T("%s %d %d %d %d\n"), m_pString, &m_sMapData.iRow, &m_sMapData.iCol, &m_sMapData.iCellCount, &m_sMapData.iCellSize);
	// Vertex Height Data
	_fgetts(m_pBuffer, 256, fp);
	_stscanf(m_pBuffer, _T("%s %d\n "), m_pString, &m_iTemp, m_pString);

	_fgetts(m_pBuffer, 256, fp);
	_stscanf(m_pBuffer, _T(" %s\n"), m_pString);

	m_sMapData.m_fHegihtList.resize(m_iTemp);
	int layer = 0;
	for (int iVertex = 0; iVertex < m_iTemp; iVertex++)
	{

		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%f "), &m_sMapData.m_fHegihtList[iVertex]);


	}

	_fgetts(m_pBuffer, 256, fp);
	_stscanf(m_pBuffer, _T("%s %d \n"), m_pString, &m_iTemp);

	m_sMapData.m_sQTData.m_ObjList.resize(m_iTemp);
	TCHAR  Buf[255];
	for (int iObj = 0; iObj < m_sMapData.m_sQTData.m_ObjList.size(); iObj++)
	{
		m_sMapData.m_sQTData.m_ObjList[iObj].m_MapObj = std::make_shared<MAP_OBJ_DATA>();
		OBJECT &OBJ =
			m_sMapData.m_sQTData.m_ObjList[iObj];
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%d \n"),&m_iTemp);

		 OBJ.m_MapObj->m_iQuadTreeIndex=m_iTemp;

		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%s \n"), Buf);
		 OBJ.m_MapObj->m_SkinName=Buf;
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("%s \n"), Buf);
		 OBJ.m_MapObj->m_BoneName=Buf;

		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("\t%s\n"), Buf);

		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("\t%f %f %f %f\n"),
			& OBJ.m_MapObj->m_matWorld._11, & OBJ.m_MapObj->m_matWorld._12, & OBJ.m_MapObj->m_matWorld._13,& OBJ.m_MapObj->m_matWorld._14);
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("\t%f %f %f %f\n"),
			& OBJ.m_MapObj->m_matWorld._21, & OBJ.m_MapObj->m_matWorld._22, & OBJ.m_MapObj->m_matWorld._23, & OBJ.m_MapObj->m_matWorld._24);
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("\t%f %f %f %f\n"),
			& OBJ.m_MapObj->m_matWorld._31, & OBJ.m_MapObj->m_matWorld._32, & OBJ.m_MapObj->m_matWorld._33, & OBJ.m_MapObj->m_matWorld._34);
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("\t%f %f %f %f\n"),
			& OBJ.m_MapObj->m_matWorld._41, & OBJ.m_MapObj->m_matWorld._42, & OBJ.m_MapObj->m_matWorld._43, & OBJ.m_MapObj->m_matWorld._44);


		KG_Box Box = OBJ.m_MapObj->m_Box;
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("\t%f %f %f\n"),
			&Box.vCenter.x, &Box.vCenter.y, &Box.vCenter.z);
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("\t%f %f %f\n"),
			&Box.vMin.x, &Box.vMin.y, &Box.vMin.z);
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("\t%f %f %f\n"),
			&Box.vMax.x, &Box.vMax.y, &Box.vMax.z);


		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("\t%f %f %f\n"),
			&Box.vAxis[0].x, &Box.vAxis[0].y, &Box.vAxis[0].z);
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("\t%f %f %f\n"),
			&Box.vAxis[1].x, &Box.vAxis[1].y, &Box.vAxis[1].z);
		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("\t%f %f %f\n"),
			&Box.vAxis[2].x, &Box.vAxis[2].y, &Box.vAxis[2].z);

		_fgetts(m_pBuffer, 256, fp);
		_stscanf(m_pBuffer, _T("\t%f %f %f\n"),
			&Box.fExtent[0], &Box.fExtent[1], &Box.fExtent[2]);
	}
	const TCHAR* HeightFile;
	if (StrStrW(m_sMapData.m_HeightMapFile.c_str(),L"(null)"))
	{
		 HeightFile = nullptr;
	}
	else
	{
		HeightFile = m_sMapData.m_HeightMapFile.data();
	}
	CreateMap(m_sMapData.iRow, m_sMapData.iCol, m_sMapData.iCellCount,m_sMapData.iCellSize, 
		m_sMapData.m_BaseTextureFile.c_str(), m_sMapData.m_NormalMapFile.c_str(), HeightFile);
	for (int iTex = 0; iTex < m_sMapData.m_pSplattTextureFile.size(); iTex++)
	{
		m_Map->AddSplattTexture(m_sMapData.m_pSplattTextureFile[iTex].data(), iTex+1);
	}
	HRESULT hr;





	if (SUCCEEDED(hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice,
		m_sMapData.m_pSplattAlphaTextureFile.data(), NULL, NULL, m_Map->m_pCopySrv.GetAddressOf(), NULL)))



	for (int i = 0; i < m_sMapData.m_sQTData.m_ObjList.size(); i++)
	{
		OBJECT& obj = m_sMapData.m_sQTData.m_ObjList[i];
		AddObject(obj);
	}

	return true;
}
INT Sample::AddObject(OBJECT OBJ)
{
	std::shared_ptr<CBY::CBY_Object> Object;
	Object = std::make_shared<CBY::CBY_Object>();
	Object->Create(m_pd3dDevice, m_pContext, L"../../data/shader/ObjectShader.txt", nullptr, "VSOBJECT", "PS");
	Object->SkinLoad( OBJ.m_MapObj->m_SkinName);
	Object->BoneLoad( OBJ.m_MapObj->m_BoneName);

	Object->m_matWorld =  OBJ.m_MapObj->m_matWorld;
	
	//OBJ.m_MapObj->SetObj(Object);
	//OBJ.m_MapObj->SetID(m_ObjID++);

	std::shared_ptr < JH::JH_MapObj> MapObj = std::make_shared<JH::JH_MapObj>();
	MapObj->SetBoneName(OBJ.m_MapObj->m_BoneName);
	MapObj->SetSkinName(OBJ.m_MapObj->m_SkinName);
	MapObj->SetID(m_ObjID++);
	MapObj->SetObj(Object);
	MapObj->SetQuadIndex(OBJ.m_MapObj->m_iQuadTreeIndex);
	m_QuadTree->GetObjectAddNode(MapObj);
	return 1;
}
void 	Sample::RunComputeShaderSplatting(ID3D11ComputeShader* pCS, ID3D11ShaderResourceView* pShaderResourceView,
	ID3D11ShaderResourceView* pBufSrv,
	ID3D11UnorderedAccessView* pUnorderedAccessView, UINT X, UINT Y, UINT Z)
{

	ID3D11ShaderResourceView* ppSRVNULL[2] = { NULL, NULL };
	//m_pContext->CSSetShaderResources(0, 2, ppSRVNULL);
	//m_pContext->CSSetShaderResources(1, 2, ppSRVNULL);
	ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
	//m_pContext->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL, NULL);
	//m_pContext->CSSetShader(NULL, NULL, 0);

	m_pContext->CSSetShader(pCS, NULL, 0);


	m_pContext->CSSetShaderResources(0, 1, &pShaderResourceView);
	m_pContext->CSSetShaderResources(1, 1, &pBufSrv);
	


	m_pContext->CSSetUnorderedAccessViews(0, 1, &pUnorderedAccessView, NULL);
	ID3D11Buffer* ppCBNULL[1] = { NULL };
	m_pContext->CSSetConstantBuffers(0, 1, ppCBNULL);

	m_pContext->Dispatch(X, Y, Z);

	//m_pContext->CopyResource(pReadTexture.Get(), pUAVTexture.Get());



	m_pContext->CSSetShader(NULL, NULL, 0);


	// CS 세팅 해제

	m_pContext->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL, NULL);


	m_pContext->CSSetShaderResources(0, 2, ppSRVNULL);
	m_pContext->CSSetShaderResources(1, 2, ppSRVNULL);


	m_pContext->CSSetConstantBuffers(0, 1, ppCBNULL);


	m_pContext->CopyResource((ID3D11Resource*)pReadTexture.Get(), (ID3D11Resource*)pUAVTexture.Get());



}
HRESULT	Sample::CreateSplattingTexture(JH::KG_Node* pNode)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC td;

	pDestTexture.Reset();
	pSrcTexture.Reset();
	ZeroMemory(&td, sizeof(td));

	UINT WIDTH = pNode->m_dwCorner[1] - pNode->m_dwCorner[0];
	UINT HEIGHT = (UINT)((pNode->m_dwCorner[2] - pNode->m_dwCorner[0]) / WIDTH) - 1;
	td.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.Width = WIDTH * 8;
	td.Height = HEIGHT * 8;
	td.Usage = D3D11_USAGE_STAGING;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.MiscFlags = 0;

	if (FAILED(hr = m_pd3dDevice->CreateTexture2D(&td, NULL, &pDestTexture)))
	{
		return hr;
	}



	td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	td.Usage = D3D11_USAGE_DEFAULT;
	if (FAILED(hr = m_pd3dDevice->CreateTexture2D(&td, NULL, &pSrcTexture)))
	{
		return hr;
	}
}
HRESULT	Sample::CreateSplattingTexture()
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC td;



	ZeroMemory(&td, sizeof(td));

	td.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.Width = (m_Map->m_iRowNum - 1) * 16;
	td.Height = (m_Map->m_iColumNum - 1) * 16;
	td.Usage = D3D11_USAGE_STAGING;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.MiscFlags = 0;

	if (FAILED(hr = m_pd3dDevice->CreateTexture2D(&td, NULL, &pDestTexture)))
	{
		return hr;
	}



	td.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	td.Usage = D3D11_USAGE_DEFAULT;
	if (FAILED(hr = m_pd3dDevice->CreateTexture2D(&td, NULL, &pSrcTexture)))
	{
		return hr;
	}




	return hr;
}
HRESULT Sample::CreateCSTexture()
{


	HRESULT hr = S_OK;
	//before dispatch
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	m_vBuf0[0].iRow = (m_Map->m_iRowNum - 1);
	m_vBuf0[0].iCol = (m_Map->m_iRowNum - 1);
	td.Width = m_vBuf0[0].iRow;
	td.Height = m_vBuf0[0].iCol;
	td.MipLevels = 1;
	td.MiscFlags = 0;
	td.ArraySize = 1;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	hr = m_pd3dDevice->CreateTexture2D(&td, NULL, pUAVTexture.GetAddressOf());


	hr = m_pd3dDevice->CreateTexture2D(&td, NULL, pReadTexture.GetAddressOf());



	D3D11_UNORDERED_ACCESS_VIEW_DESC viewDescUAV;
	ZeroMemory(&viewDescUAV, sizeof(viewDescUAV));
	viewDescUAV.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	viewDescUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	viewDescUAV.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateUnorderedAccessView(pUAVTexture.Get(), &viewDescUAV, m_pUAV.GetAddressOf());

	//the getSRV function after dispatch.



	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	hr = m_pd3dDevice->CreateShaderResourceView(pUAVTexture.Get(), &srvDesc, m_pReadSrv.GetAddressOf());
	hr = m_pd3dDevice->CreateShaderResourceView(pReadTexture.Get(), &srvDesc, m_Map->m_pCopySrv.GetAddressOf());




	m_vBuf0[0].vPickPos = D3DXVECTOR3(0, 0, 0);
	m_vBuf0[0].fRadius = 20;
	

	(CDXH::CreateStructuredBuffer(m_pd3dDevice, sizeof(CSBUFF), 1, &m_vBuf0[0], m_pStructureBF.GetAddressOf()));
	(CDXH::CreateBufferSRV(m_pd3dDevice, m_pStructureBF.Get(), m_pBufSrv.GetAddressOf()));
	//(CDXH::CreateStructuredBuffer(m_pd3dDevice, sizeof(CSPOSBF), m_Map->m_VerTex.size(), &m_vBuf1[0], m_pStructureEditBF.GetAddressOf()));
	//(CDXH::CreateBufferSRV(m_pd3dDevice, m_pStructureBF.Get(), m_pEditBufSrv.GetAddressOf()));


	return hr;
}
//HRESULT Sample::MapSplatting(SPHERE Sphere)
//{
//
//
//
//
//	HRESULT hr = S_OK;
//
//
//	D3D11_MAPPED_SUBRESOURCE MapSrc;
//
//	static int i = 0;
//
//	D3D11_TEXTURE2D_DESC td;
//	pDestTexture->GetDesc(&td);
//	if (SUCCEEDED(m_pContext->Map((ID3D11Resource*)pDestTexture.Get(), NULL, D3D11_MAP_READ_WRITE, NULL, &MapSrc)))
//	{
//
//		UCHAR* pTexcell = (UCHAR*)MapSrc.pData;
//		for (int iRow = 0; iRow < m_Map->m_iRowNum * 16; iRow++)
//		{
//			int irowStart = MapSrc.RowPitch*iRow;
//			for (int iCol = 0; iCol < m_Map->m_iColumNum * 16; iCol++)
//			{
//				float fDistance = sqrt((Sphere.vCenter.x - iCol)*(Sphere.vCenter.x - iCol) +
//					(Sphere.vCenter.z - iRow)*(Sphere.vCenter.z - iRow));
//				if (fDistance <= Sphere.Radius)
//				{
//					pTexcell[irowStart + iCol * 4] = m_Map->m_vSplattTextureList[0]->GetAlpha();
//					pTexcell[irowStart + iCol * 4 + 1] = m_Map->m_vSplattTextureList[1]->GetAlpha();
//					pTexcell[irowStart + iCol * 4 + 2] = m_Map->m_vSplattTextureList[2]->GetAlpha();
//					pTexcell[irowStart + iCol * 4 + 3] = m_Map->m_vSplattTextureList[3]->GetAlpha();
//
//				}
//
//
//			}
//
//		}
//
//		m_pContext->Unmap((ID3D11Resource*)pDestTexture.Get(), D3D11CalcSubresource(0, 0, 1));
//
//	}
//
//
//
//
//
//	m_pContext->CopyResource((ID3D11Resource*)pSrcTexture.Get(), (ID3D11Resource*)pDestTexture.Get());
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC svd;
//	ZeroMemory(&svd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
//	svd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	svd.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
//	svd.Texture2D.MipLevels = 1;
//	svd.Texture2D.MostDetailedMip = 0;
//
//
//	m_pSplSrv.Reset();
//	m_pd3dDevice->CreateShaderResourceView((ID3D11Resource*)pSrcTexture.Get(), &svd, m_pSplSrv.GetAddressOf());
//	m_pContext->PSSetShaderResources(2, 1, m_pSplSrv.GetAddressOf());
//	return hr;
//}
void Sample::GetNearPoint()
{
	
	D3DXVECTOR3 v0, v1, v2, vIntersection;
	m_NearPoint = m_Select.m_Ray.m_Dir*m_pMainCamera->m_fFar;
	m_fTimer += g_SecondTime;

	m_fTimer = 0.0f;
	m_Interval = 999999.0f;

	m_QuadTree->m_SelectNodeList.clear();
	m_QuadTree->GetSelectNode(m_QuadTree->m_pRootNode);

	for (int iNode = 0; iNode <
		m_QuadTree->m_SelectNodeList.size(); iNode++)
	{
		DWORD dwFace = m_QuadTree->m_SelectNodeList[iNode]->m_IndexList.size() / 3;
		for (int iFace = 0; iFace < dwFace; iFace++)
		{
			DWORD i0 = m_QuadTree->m_SelectNodeList[iNode]->m_IndexList[iFace * 3 + 0];
			DWORD i1 = m_QuadTree->m_SelectNodeList[iNode]->m_IndexList[iFace * 3 + 1];
			DWORD i2 = m_QuadTree->m_SelectNodeList[iNode]->m_IndexList[iFace * 3 + 2];
			v0 = m_Map->m_VerTex[i0].p;
			v1 = m_Map->m_VerTex[i1].p;
			v2 = m_Map->m_VerTex[i2].p;

			D3DXVECTOR3 vDIR = m_Select.m_Ray.m_Dir;
			D3DXVECTOR3 vEnd = m_Select.m_Ray.m_Orig + vDIR * m_pMainCamera->m_fFar;

			D3DXVECTOR3 vNormal;
			D3DXVECTOR3 e0 = v1 - v0;
			D3DXVECTOR3 e1 = v2 - v0;
			D3DXVec3Cross(&vNormal, &e0, &e1);


			m_Select.PickCheck(&m_Interval, v0, v1, v2);


		}
	}

		
		m_NearPoint = m_Select.GetIntersection(m_Interval);
		
	
}
void Sample::SelectObject()
{


	m_Interval = 999999.0f;
	bool bSel = false;;

	m_QuadTree->GetSelectObj(m_QuadTree->m_pRootNode);

	if (m_QuadTree->m_pSelectObj)
	{
		m_pSelectMapObj = m_QuadTree->m_pSelectObj;
		m_SelectObjID=m_pSelectMapObj->GetID();
	}


}
void Sample::MapUpDown(JH::SPHERE sphere)
{
	D3DXVECTOR3 v0, v1, v2;

	


	for (size_t iNode = 0; iNode < m_QuadTree->m_SelectNodeList.size(); iNode++)
	{
		D3DXVECTOR3 vCenter = m_QuadTree->m_SelectNodeList[iNode]->m_Box.vCenter;
		D3DXVECTOR3 vMin = m_QuadTree->m_SelectNodeList[iNode]->m_Box.vMin;
		float fDistance = D3DXVec3Length(&(vCenter - sphere.vCenter));
		float fMinMax = D3DXVec3Length(&(vCenter - vMin));
		//float fDistance = sqrt((vCenter.x - sphere.vCenter.x)*
		//	(vCenter.x - sphere.vCenter.x) +
		//	(vCenter.z - sphere.vCenter.z)*
		//	(vCenter.z - sphere.vCenter.z));
		//float fMinMax = sqrt((vCenter.x - sphere.vCenter.x)*
		//	(vCenter.x - vMin.x) +
		//	(vCenter.z - vMin.z)*
		//	(vCenter.z - vMin.z));
		DWORD dwFace = m_QuadTree->m_SelectNodeList[iNode]->m_IndexList.size() / 3;
		if (sphere.Radius + fMinMax >= fDistance)
		{
			for (int iFace = 0; iFace < dwFace; iFace++)
			{
				for (int iV = 0; iV < 3; iV++)
				{
					DWORD i0 = m_QuadTree->m_SelectNodeList[iNode]->m_IndexList[iFace * 3 + iV];


					fDistance = sqrt((m_Map->m_VerTex[i0].p.x - sphere.vCenter.x)*
						(m_Map->m_VerTex[i0].p.x - sphere.vCenter.x) +
						(m_Map->m_VerTex[i0].p.z - sphere.vCenter.z)*
						(m_Map->m_VerTex[i0].p.z - sphere.vCenter.z));

					float  fDet = (fDistance / sphere.Radius)*D3DX_PI / 2.0;

					float value = cos(fDet)*g_SecondTime;
					if (sphere.Radius > fDistance)
					{
						m_Map->m_VerTex[i0].p.y += value * m_HeightVlaue;
					}


				}
				DWORD i0 = m_QuadTree->m_SelectNodeList[iNode]->m_IndexList[iFace * 3 + 0];
				DWORD i1 = m_QuadTree->m_SelectNodeList[iNode]->m_IndexList[iFace * 3 + 1];
				DWORD i2 = m_QuadTree->m_SelectNodeList[iNode]->m_IndexList[iFace * 3 + 2];

				D3DXVECTOR3 vFaceNormal, E0, E1;
				E0 = m_Map->m_VerTex[i1].p - m_Map->m_VerTex[i0].p;
				E1 = m_Map->m_VerTex[i2].p - m_Map->m_VerTex[i0].p;

				D3DXVec3Cross(&vFaceNormal, &E0, &E1);
				D3DXVec3Normalize(&vFaceNormal, &vFaceNormal);

				m_Map->m_VerTex[i0].n = vFaceNormal;
				m_Map->m_VerTex[i1].n = vFaceNormal;
				m_Map->m_VerTex[i2].n = vFaceNormal;


			}
		}
	

		for (size_t iNeighbor = 0; iNeighbor < m_QuadTree->m_SelectNodeList[iNode]->m_NeighborNodeList.size(); iNeighbor++)
		{
			if (m_QuadTree->m_SelectNodeList[iNode]->m_NeighborNodeList[iNeighbor] == nullptr)
				continue;

			D3DXVECTOR3 vCenter = m_QuadTree->m_SelectNodeList[iNode]->m_NeighborNodeList[iNeighbor]->m_Box.vCenter;
			D3DXVECTOR3 vMin = m_QuadTree->m_SelectNodeList[iNode]->m_NeighborNodeList[iNeighbor]->m_Box.vMin;
			float fDistance = D3DXVec3Length(&(vCenter - sphere.vCenter));
			float fMinMax = D3DXVec3Length(&(vCenter - vMin));
	/*		float fDistance = sqrt((vCenter.x - sphere.vCenter.x)*
				(vCenter.x - sphere.vCenter.x) +
				(vCenter.z - sphere.vCenter.z)*
				(vCenter.z - sphere.vCenter.z));
			float fMinMax = sqrt((vCenter.x - sphere.vCenter.x)*
				(vCenter.x - vMin.x) +
				(vCenter.z - vMin.z)*
				(vCenter.z - vMin.z));*/
			DWORD dwFace = m_QuadTree->m_SelectNodeList[iNode]->m_NeighborNodeList[iNeighbor]->m_IndexList.size() / 3;
			if (sphere.Radius + fMinMax >= fDistance)
			{
				for (int iFace = 0; iFace < dwFace; iFace++)
				{
					for (int iV = 0; iV < 3; iV++)
					{
						DWORD i0 = m_QuadTree->m_SelectNodeList[iNode]->m_NeighborNodeList[iNeighbor]->m_IndexList[iFace * 3 + iV];


						fDistance = sqrt((m_Map->m_VerTex[i0].p.x - sphere.vCenter.x)*
							(m_Map->m_VerTex[i0].p.x - sphere.vCenter.x) +
							(m_Map->m_VerTex[i0].p.z - sphere.vCenter.z)*
							(m_Map->m_VerTex[i0].p.z - sphere.vCenter.z));

						float  fDet = (fDistance / sphere.Radius)*D3DX_PI / 2.0;

						float value = cos(fDet)*g_SecondTime;
						if (sphere.Radius > fDistance)
						{
							m_Map->m_VerTex[i0].p.y += value * m_HeightVlaue;
						}


					}
					DWORD i0 = m_QuadTree->m_SelectNodeList[iNode]->m_NeighborNodeList[iNeighbor]->m_IndexList[iFace * 3 + 0];
					DWORD i1 = m_QuadTree->m_SelectNodeList[iNode]->m_NeighborNodeList[iNeighbor]->m_IndexList[iFace * 3 + 1];
					DWORD i2 = m_QuadTree->m_SelectNodeList[iNode]->m_NeighborNodeList[iNeighbor]->m_IndexList[iFace * 3 + 2];

					D3DXVECTOR3 vFaceNormal, E0, E1;
					E0 = m_Map->m_VerTex[i1].p - m_Map->m_VerTex[i0].p;
					E1 = m_Map->m_VerTex[i2].p - m_Map->m_VerTex[i0].p;

					D3DXVec3Cross(&vFaceNormal, &E0, &E1);
					D3DXVec3Normalize(&vFaceNormal, &vFaceNormal);

					m_Map->m_VerTex[i0].n = vFaceNormal;
					m_Map->m_VerTex[i1].n = vFaceNormal;
					m_Map->m_VerTex[i2].n = vFaceNormal;


				}
			}
		}

	}
	


}

void Sample::MapFlatting(JH::SPHERE sphere)
{
	D3DXVECTOR3 v0, v1, v2;



	for (size_t iNode = 0; iNode < m_QuadTree->m_SelectNodeList.size(); iNode++)
	{
		D3DXVECTOR3 vCenter = m_QuadTree->m_SelectNodeList[iNode]->m_Box.vCenter;
		D3DXVECTOR3 vMin = m_QuadTree->m_SelectNodeList[iNode]->m_Box.vMin;
		float fDistance = sqrt((vCenter.x - sphere.vCenter.x)*
			(vCenter.x - sphere.vCenter.x) +
			(vCenter.z - sphere.vCenter.z)*
			(vCenter.z - sphere.vCenter.z));
		float fMinMax = sqrt((vCenter.x - sphere.vCenter.x)*
			(vCenter.x - vMin.x) +
			(vCenter.z - vMin.z)*
			(vCenter.z - vMin.z));
		DWORD dwFace = m_QuadTree->m_SelectNodeList[iNode]->m_IndexList.size() / 3;
		if (sphere.Radius + fMinMax >= fDistance)
		{
			for (int iFace = 0; iFace < dwFace; iFace++)
			{
				for (int iV = 0; iV < 3; iV++)
				{
					DWORD i0 = m_QuadTree->m_SelectNodeList[iNode]->m_IndexList[iFace * 3 + iV];


					fDistance = sqrt((m_Map->m_VerTex[i0].p.x - sphere.vCenter.x)*
						(m_Map->m_VerTex[i0].p.x - sphere.vCenter.x) +
						(m_Map->m_VerTex[i0].p.z - sphere.vCenter.z)*
						(m_Map->m_VerTex[i0].p.z - sphere.vCenter.z));

					float  fDet = (fDistance / sphere.Radius)*D3DX_PI / 2.0;
					if (sphere.Radius > fDistance)
					{
						float value = cos(fDet)*g_SecondTime;

						m_Map->m_VerTex[i0].p.y = 0;

					}
				}
				DWORD i0 = m_QuadTree->m_SelectNodeList[iNode]->m_IndexList[iFace * 3 + 0];
				DWORD i1 = m_QuadTree->m_SelectNodeList[iNode]->m_IndexList[iFace * 3 + 1];
				DWORD i2 = m_QuadTree->m_SelectNodeList[iNode]->m_IndexList[iFace * 3 + 2];

				D3DXVECTOR3 vFaceNormal, E0, E1;
				E0 = m_Map->m_VerTex[i1].p - m_Map->m_VerTex[i0].p;
				E1 = m_Map->m_VerTex[i2].p - m_Map->m_VerTex[i0].p;

				D3DXVec3Cross(&vFaceNormal, &E0, &E1);
				D3DXVec3Normalize(&vFaceNormal, &vFaceNormal);

				m_Map->m_VerTex[i0].n = vFaceNormal;
				m_Map->m_VerTex[i1].n = vFaceNormal;
				m_Map->m_VerTex[i2].n = vFaceNormal;


			}
		}

	}
	m_pContext->UpdateSubresource(m_Map->m_obj.m_pVertexBuffer.Get(), 0, 0, &m_Map->m_VerTex.at(0), 0, 0);


}
bool Sample::CreateMap(int iWidth,
	int iHeight,
	int iCellCount,
	int iCellSize,
	const TCHAR* pTexturFileName,
	const TCHAR* pNormalMapFileName,
	const TCHAR* pHeightMapFileName)
{


	if (m_Map != nullptr)
	{
		m_Map->Release();
		m_QuadTree->Release();
		m_ObjectList.clear();
	}


	m_Map = std::make_shared<JH::JH_Map>();
	m_Map->m_vHeightList.resize(m_sMapData.m_fHegihtList.size());

	if (pHeightMapFileName)
	{
		m_Map->CreateHeightMap(m_pd3dDevice, m_pContext, pHeightMapFileName);
	}
	else
	{
		m_Map->m_iCellCount = iCellCount;
		m_Map->m_iRowNum = iCellCount * iWidth + 1;
		m_Map->m_iColumNum = iCellCount * iHeight + 1;
	
	}
	if (m_sMapData.m_fHegihtList.size() > 0)
	{
		
		std::copy(m_sMapData.m_fHegihtList.begin(),
			m_sMapData.m_fHegihtList.end(),
			m_Map->m_vHeightList.begin());
	}
	//	CreateSplattingTexture();



	m_Map->SetMapDesc(pTexturFileName, L"../../data/Shader/JHMapShader.txt", m_Map->m_iRowNum, m_Map->m_iColumNum, iCellSize, 1.0f);


	m_Map->m_pNormMapFileName = pNormalMapFileName;
	m_Map->m_HegithFileName = pHeightMapFileName;
	m_Map->Load(m_pd3dDevice, m_pContext);
	CreateCSTexture();

	m_QuadTree = std::make_shared<JH::HQuadTree>();
	m_QuadTree->Build(m_Map.get(), m_pMainCamera);
	m_QuadTree->m_pSelect = &m_Select;


	return true;
}

int Sample::CreateObj(const TCHAR* pSkinFileName, const TCHAR* pBoneFileName, D3DXMATRIX& matWorld)

{
	if (!m_Map) { return -1; }
	if (m_Object) 
	{ 
		//m_Object.reset(); 
		//m_Object = nullptr;

		return -1;
	}

	if (pSkinFileName == nullptr || pBoneFileName == nullptr) { return -1; }
		m_pSelectMapObj = nullptr;
		m_QuadTree->m_pFindNode = nullptr;
		m_Object=std::make_shared<CBY::CBY_Object>();
		m_Object->Create(m_pd3dDevice, m_pContext, L"../../data/shader/ObjectShader.txt", nullptr, "VSOBJECT", "PS");
		m_Object->SkinLoad(pSkinFileName);
		m_Object->BoneLoad(pBoneFileName);
		

		bObjectAdd = true;

		assert(true); // false

		
		m_CurrentMapObj = std::make_shared<JH::JH_MapObj>();
		//m_QuadTree->= std::make_shared<JH::JH_MapObj>();
		m_CurrentMapObj->SetObj(m_Object);
		m_CurrentMapObj->SetID(m_ObjID++);
		m_CurrentMapObj->SetSkinName(pSkinFileName);
		m_CurrentMapObj->SetBoneName(pBoneFileName);
	
		m_CurrentMapObj->GetObj()->m_matWorld=m_Object->m_ObjList[0]->m_ObjList[0]->m_matWorld;
	
		m_CurrentMapObj->GetObj()->GetCharBox() = m_Object->GetCharBox();


		m_CurrentMapObj->GetObj()->SetMatrix(&m_CurrentMapObj->GetObj()->m_matWorld, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
		

		



	return 1;

}
bool Sample::Init()
{



	m_ModelViewCamera = std::make_shared<KG_ModelViewCamera>();
	m_ModelViewCamera->Init();
	m_ModelViewCamera->SetViewProj();
	m_ModelViewCamera->CreateViewMatrix(D3DXVECTOR3(0, 0, -50.0f), D3DXVECTOR3(0, 0, 0));

	m_BackViewCamera = std::make_shared<KG_BackViewCamera>();
	m_BackViewCamera->Init();
	m_BackViewCamera->SetViewProj();
	m_BackViewCamera->CreateViewMatrix(D3DXVECTOR3(0, 200, -50.0f), D3DXVECTOR3(0, 0, 0));

	m_DebugCamera= std::make_shared<KG_DebugCamera>();
	m_DebugCamera->Init();
	m_DebugCamera->SetViewProj();

	m_pMainCamera = m_DebugCamera.get();
	m_pMainCamera->CreateViewMatrix(D3DXVECTOR3(0, 400, -50.0f), D3DXVECTOR3(0, 0, 0));
	m_pMainCamera->UpdateVector();


	float fAspect = (float)Winrt.right / Winrt.bottom;
	m_pMainCamera->CreateProjMatrix(0.1F, 2000.0F, D3DX_PI*0.5F, fAspect);


	JH::I_LIGHT_MGR.GetDevice(m_pd3dDevice);
	JH::I_LIGHT_MGR.GetContext(m_pContext);
	JH::I_LIGHT_MGR.SetCamera(m_pMainCamera);
	JH::I_LIGHT_MGR.Create(L"../../data/Shader/JHMapShader.txt", L"../../data/LightSrc/LightInfo.txt");

	//ComPuteShader
	(CDXH::CreateComputeShader( L"ComputeShader.HLSL", "CSMAIN", m_pd3dDevice, m_pCS.GetAddressOf()));
	//(CDXH::CreateComputeShader(L"ComputeShader.HLSL", "CSEDITER", m_pd3dDevice, m_pEditCS.GetAddressOf()));

	m_MiniMap.Create(m_pd3dDevice, m_pContext);
	m_MiniMap.m_RT->Create(m_pd3dDevice,150,150);

	return true;
}
bool Sample::Frame()
{
	if (m_Map == nullptr)return true;
	
	//test/adsfadsf
	m_QuadTree->m_pSelect->SetMarix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
	D3DXVECTOR3 v0, v1, v2, vIntersection;
	

	

	
	switch (m_ToolState)
	{
	case NORMAL:
		break;
	case HEIGHT_EDIT:
		GetNearPoint();
		if (I_Input.KeyCheck(VK_LBUTTON))
		{
			JH::SPHERE sphere;
			sphere.vCenter = m_NearPoint;
			sphere.Radius = m_vBuf0[0].fRadius;
			MapUpDown(sphere);
			m_pContext->UpdateSubresource(m_Map->m_obj.m_pVertexBuffer.Get(), 0, 0, &m_Map->m_VerTex.at(0), 0, 0);
		}
		break;
	case FLATTING:
		GetNearPoint();
		if (I_Input.KeyCheck(VK_LBUTTON))
		{
			JH::SPHERE sphere;
			sphere.vCenter = m_NearPoint;
			sphere.Radius = m_vBuf0[0].fRadius;
			MapFlatting(sphere);
		}
		break;
	case SPLATTING:
		GetNearPoint();
		if (I_Input.KeyCheck(VK_LBUTTON))//&& m_fTimer >=0.5)
		{
			m_vBuf0[0].vPickPos = D3DXVECTOR3(m_NearPoint.x + ((m_vBuf0[0].iRow) / 2.0f),
				0, -(m_NearPoint.z) + ((m_vBuf0[0].iCol) / 2.0f));
			m_pContext->UpdateSubresource((ID3D11Resource*)m_pStructureBF.Get(), NULL, nullptr, &m_vBuf0, NULL, NULL);
			RunComputeShaderSplatting(m_pCS.Get(), m_Map->m_pCopySrv.Get(), m_pBufSrv.Get(), m_pUAV.Get(), m_vBuf0[0].iRow / 16, m_vBuf0[0].iCol / 16, 1);
		}
		break;
	case ADDOBJECT:
		KG_Box;
		
		if (!m_Object)break;
		if (I_Input.KeyCheck(VK_LBUTTON))
		{
			GetNearPoint();


			m_CurrentMapObj->GetObj()-> m_matWorld._41 = m_NearPoint.x;
			m_CurrentMapObj->GetObj()->m_matWorld._42 = m_NearPoint.y;
			m_CurrentMapObj->GetObj()->m_matWorld._43 = m_NearPoint.z;

			if (m_QuadTree->GetObjectAddNode(m_CurrentMapObj))
			{
				m_CurrentMapObj = nullptr;
				m_Object = nullptr;
				m_ToolState = NORMAL;
				bObjectAdd = false;
			}
		}
		break;
	case SETTING:
		/*if (!m_pSelectMapObj)break;
		{
			if (m_QuadTree->ChangeObjectNode(m_pSelectMapObj))
			{
			
				m_pSelectMapObj = nullptr;
				m_SelectObj = nullptr;
				m_ToolState = NORMAL;
				bSet = false;
				bObjectAdd = false;
			}
			
		}*/
		break;
	case SELECT:
		if(!m_pSelectMapObj)
		if (I_Input.KeyCheck(VK_RBUTTON))
		{
				SelectObject();
		}
		break;
	case MOVE:
		if (!m_pSelectMapObj)break;
		if (I_Input.KeyCheck(VK_LBUTTON))
		{
			GetNearPoint();
			D3DXVECTOR3 before = m_pSelectMapObj->GetObj()->GetCharBox().vCenter;
			m_pSelectMapObj->GetObj()-> m_matWorld._41 = m_NearPoint.x;
			m_pSelectMapObj->GetObj()->m_matWorld._42 = m_NearPoint.y;
			m_pSelectMapObj->GetObj()->m_matWorld._43 = m_NearPoint.z;
			if (m_QuadTree->ChangeObjectNode(m_pSelectMapObj))
			{
		
				m_pSelectMapObj = nullptr;
				m_SelectObj = nullptr;
				m_ToolState = NORMAL;
				bSet = false;
				bObjectAdd = false;
			}

		}

	
		break;



	default:
		break;
	}

	m_QuadTree->Frame();







	return true;
}
bool Sample::Render()
{



	
	ID3D11ShaderResourceView* ppSRVNULL[1] = { NULL };
	ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
	ID3D11Buffer* ppCBNULL[1] = { NULL };
	
	if (m_Map == nullptr)return true;
	else
	{
	
			m_QuadTree->Render();
			D3DXMATRIX World;
			D3DXMatrixIdentity(&World);

			
			for (auto  Obj : m_QuadTree->m_DrawObjectList)
			{

				

					Obj->GetObj()->SetMatrix(&Obj->GetObj()->m_matWorld,
						&m_pMainCamera->m_View,
						&m_pMainCamera->m_Proj);
					Obj->GetObj()->Frame();
					Obj->GetObj()->Render();
			
				

			}

	}
	

	
	//m_MiniMap.SetMatrix(NULL, NULL, NULL);
	//m_pContext->RSSetViewports(1, &m_MiniMap.m_RT->m_vp);
	//m_MiniMap.Render();




	m_pContext->RSSetViewports(1, &m_ViewPort);

	//m_DebugLine->SetMatrix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
	//m_DebugLine->Draw(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(100, 0, 0), D3DXVECTOR4(1, 0, 0, 1));
	//m_DebugLine->Draw(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 100, 0), D3DXVECTOR4(0, 1, 0, 1));
	//m_DebugLine->Draw(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 100), D3DXVECTOR4(0, 0, 1, 1));







	

	m_QuadTree->DrawObjectBoxLine();





	m_pContext->PSSetShaderResources(2, 1, ppSRVNULL);

	return true;
}
bool Sample::Release()
{
	/*map< int, ComPtr<CBY::CBY_Object>>::iterator iter;

	for (iter = m_ObjectList.begin();iter!=m_ObjectList.end(); iter++)
	{
		iter->second->Release();
	
	
	}

	m_ObjectList.clear();*/


	return true;
}
Sample::Sample()
{
	m_fTimer		= 0.0f;
	bAttach			= false;
	bSplatting		= false;
	bMapFlatting	= false;
	bObjectAdd		= false;
	bSelect			= false;
	bSet			= false;

	m_fYaw			= 0.0f;
	m_fPitch		= 0.0f;
	m_fRoll			= 0.0f;
	m_Interval		= 0.0f;

	m_fScaleX		= 1.0f;
	m_fScaleY		= 1.0f;
	m_fScaleZ		= 1.0f;
	m_fValue		= 0.0f;

	m_pSPTAFile		= nullptr;
	m_ObjID = 0;
	CurrentObjIndex = -1;
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matMove);
	D3DXMatrixIdentity(&m_matMove);
}


Sample::~Sample()
{
	m_ToolState = 0;
}