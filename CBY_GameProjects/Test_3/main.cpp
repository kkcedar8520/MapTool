#pragma once
#include "main.h"
#include"KG_DxState.h"
#include"KG_Input.h"
#include"KG_ShapeMap.h"
#include"LightMgr.h"

#pragma warning( disable:4005 )

TCHAR* main::FixupName(T_STR name)
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


bool main::Init()
{
	

	//m_SkyBox = make_shared<KG_SkyBox>();
	//m_SkyBox->CreateSkyBox(m_pd3dDevice, m_pContext, L"SkyObj.hlsl");
	//m_SkyBox->CreateTexuture(m_pd3dDevice, m_pContext, L"../../data/sky/StarFiled2.dds");


	//m_pMainCamera=&m_BackViewCamera;
	m_pMainCamera = &m_DebugCamera;
	m_pMainCamera->CreateViewMatrix(D3DXVECTOR3(0, 100, -50.0f), D3DXVECTOR3(0, 0, 0));
	m_pMainCamera->UpdateVector();


	JH::I_LIGHT_MGR.GetDevice(m_pd3dDevice);
	JH::I_LIGHT_MGR.GetContext(m_pContext);
	JH::I_LIGHT_MGR.SetCamera(m_pMainCamera);
	JH::I_LIGHT_MGR.Create(L"../../data/Shader/JHMapShader.txt", L"../../data/LightSrc/LightInfo.txt");

	JH::I_MapMgr.Set(m_pd3dDevice, m_pContext, m_pMainCamera, m_Select);
	JH::I_MapMgr.AddMap(L"../../MapSave/aa.Map");
	float fAspect = (float)Winrt.right / Winrt.bottom;
	m_pMainCamera->CreateProjMatrix(0.1F, 1000.0F, D3DX_PI*0.4F, fAspect);
	


	//ComPuteShader



	return true;
}
bool main::Frame()
{
	//m_SkyBox->Frame();


	if (JH::I_MapMgr.GetCurrentMap().m_pMap == nullptr)return true;
	JH::I_MapMgr.GetCurrentMap().m_pQuadTree->Frame();
	for (auto Obj : JH::I_MapMgr.GetCurrentMap().m_ObjectList)
	{
		Obj.second->Frame();
	}
	//for (auto LightObj : I_LIGHT_MGR.m_LightObjList)
	//{
	//	LightObj.m_Shadow.Begin(m_pContext);
	//	I_MapMgr.GetCurrentMap().m_pQuadTree->Render();

	//	for (int iNode = 0; iNode < I_MapMgr.GetCurrentMap().m_pQuadTree->m_DrawObjNodeList.size(); iNode++)
	//	{
	//		KG_Node* pNode = I_MapMgr.GetCurrentMap().m_pQuadTree->m_DrawObjNodeList[iNode];
	//		std::map<int, JH_MapObj>::iterator iter;
	//		for (iter = pNode->m_ObjList.begin();
	//			iter != pNode->m_ObjList.end();
	//			iter++)
	//		{
	//			int pos = m_pMainCamera->CheckOBBInPlane(iter->second.m_Box);
	//			if (pos == P_BACK)  continue;


	//			I_MapMgr.GetCurrentMap().m_ObjectList[iter->second.GetID()]->SetMatrix(&I_MapMgr.GetCurrentMap().m_ObjectList[iter->second.GetID()]->m_matWorld,
	//				&m_pMainCamera->m_View,
	//				&m_pMainCamera->m_Proj);

	//			I_MapMgr.GetCurrentMap().m_ObjectList[iter->second.GetID()]->Render();
	//		}
	//	}
	//	LightObj.m_Shadow.Begin(m_pContext);
	//}
	return true;
}
bool main::Render()
{

	//m_SkyBox->SetMatrix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
	//m_SkyBox->Render();



	if (JH::I_MapMgr.GetCurrentMap().m_pMap == nullptr)return true;
	else
	{
		JH::I_MapMgr.GetCurrentMap().m_pQuadTree->Render();
	/*	for (auto LightObj :JH::I_LIGHT_MGR.m_LightObjList)
		{*/
			/*m_pContext->PSSetShaderResources(11, 1, &LightObj.m_Shadow.m_Rt->m_pDsvSRV);*/
	
				for (auto Obj : JH::I_MapMgr.GetCurrentMap().m_pQuadTree->m_DrawObjectList)
				{



					Obj->GetObj()->SetMatrix(&Obj->GetObj()->m_matWorld,
						&m_pMainCamera->m_View,
						&m_pMainCamera->m_Proj);
					Obj->GetObj()->Frame();
					Obj->GetObj()->Render();



				}
			
		//}
	}


	return true;
}
bool main::Release()
{



	return true;
}
main::main()
{
	m_pSPTAFile = nullptr;

}


main::~main()
{
}