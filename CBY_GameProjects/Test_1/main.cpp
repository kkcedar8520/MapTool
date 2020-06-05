#include "main.h"

bool main::Init()
{
	m_ModelViewCamera = std::make_shared<KG_ModelViewCamera>();
	m_ModelViewCamera->Init();
	m_ModelViewCamera->SetViewProj();

	m_BackViewCamera = std::make_shared<KG_BackViewCamera>();
	m_BackViewCamera->Init();
	m_BackViewCamera->SetViewProj();

	m_DebugCamera = std::make_shared<KG_DebugCamera>();
	m_DebugCamera->Init();
	m_DebugCamera->SetViewProj();
	

	m_pMainCamera = m_DebugCamera.get();

	//m_Box = std::make_shared<KG_ShapeBox>();
	//m_Box->Create(m_pd3dDevice, m_pContext, L"TestShader.txt", L"../../data/obj/Weapon.tga", "VSmat", "PS");

	//L"../../data/obj/Weapon.tga"

	m_Object = std::make_shared<CBY::CBY_Object>();
	m_Object->Create(m_pd3dDevice, m_pContext, L"../../data/shader/ObjectShader.txt", nullptr, "VSOBJECT", "PS");
	m_Object->ObjLoad(L"../../data/obj/Pilar.cby");

	return true;
}

bool main::Frame()
{
	//m_Box->SetMatrix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
	m_Object->Frame();
	return true;
}

bool main::Render()
{
	CDX::ApplyBS(m_pContext, CDX::KG_DxState::g_pAlpahBlend);
	CDX::ApplySS(m_pContext, CDX::KG_DxState::g_pSampler);
	//CDX::ApplyRS(m_pContext, CDX::KG_DxState::g_pRSSold);
	//m_Box->Render();
	m_Object->SetMatrix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
	m_Object->Render();
	return true;
}

bool main::Relese()
{
	m_Object->Release();
	return true;
}


main::main()
{
}


main::~main()
{
}
