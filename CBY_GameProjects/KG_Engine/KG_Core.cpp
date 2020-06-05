#include "KG_Core.h"

LRESULT KG_Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera != nullptr)
	{
		m_pMainCamera->MsgProc(hWnd, message, wParam, lParam);
	}
	return KG_Window::WndProc(hWnd, message, wParam, lParam);
}

HRESULT KG_Core::CreateDXrc()
{
	HRESULT hr = S_OK;


	///TEXT////
	IDXGISurface*  pSurface;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&pSurface);
	m_Font->Set(g_hwnd, 0, 0, pSurface);
	pSurface->Release();
	//////////
	return hr;
}

HRESULT	KG_Core::DeleteDXrc()
{
	HRESULT hr = S_OK;

	float fAspect = (float)Winrt.right / (float)Winrt.bottom;

	m_pMainCamera->CreateProjMatrix(
		m_pMainCamera->m_fNear,
		m_pMainCamera->m_fFar,
		m_pMainCamera->m_fFovY,
		fAspect);

	///TEXT////
	m_Font->Release();
	//////////


	return hr;
}

bool KG_Core::Init()
{
	return true;
}
bool KG_Core::Frame()
{
	return true;
}
bool KG_Core::Render()
{
	return true;
}
bool KG_Core::Release()
{
	return true;
}


bool KG_Core::GameRun()
{
	GetGameRun = CoreInit();											//런함수 실행시 Init실행하고 Frame과 Render함수를 실행 어떠한 이벤트로 종료시 Release로 메모리를 비워주고 종료한다.
	if (GetGameRun == false)
	{
		return false;
	}

	MSG msg;															//메세지를 받을 MSG 선언
	ZeroMemory(&msg, sizeof(msg));
	while (GetGameRun)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))				//PeekMessage는 메세지가 오는지 대기상태없이 계속 돌아가며 발생시1 없을시 0을 반환
		{															//그러므로 메세지큐에 메세지가 쌓여 있으면 계속 메세지 처리하다가 없을때 Frame과 Render가 돌아가 1프레임이 돌아가는 것

			TranslateMessage(&msg);									//메세지큐에 있는 메세지를 받아서 키보드의 눌림이 발생시 WN_CHAR로 가공해서 메세지큐에 넣는다
			DispatchMessage(&msg);									//WndProc로 메세지큐에있는 메세지를 넘겨준다.
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			if (CoreFrame() == false)
			{
				return false;
			}
			if (CoreRender() == false)
			{
				return false;
			}
		}
	}
	if (CoreRelease() == false)
	{
		return false;
	}
	return true;
};

bool  KG_Core::CoreInit()
{
	GetGameRun = true;
	m_Timer.Init();
	I_Input.Init();
	if (FAILED(DeviceInit()))
	{
		return false;
	}
	CDX::KG_DxState::SetState(m_pd3dDevice);
	m_DebugCamera.SetViewProj();
	m_pMainCamera = &m_DebugCamera;

	///TEXT////
	IDXGISurface*  pSurface;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&pSurface);
	m_Font = std::make_shared<KG_DXWrite>();
	m_Font->Set(g_hwnd, 0, 0, pSurface);
	pSurface->Release();
	//////////
	m_DebugLine = std::make_shared<KG_ShapeLine>();
	m_DebugLine->Create(m_pd3dDevice, m_pContext, L"../../data/shader/LineShader.txt", nullptr, "VS", "PS");

	return Init();

};
bool  KG_Core::CoreFrame()
{
	m_Timer.Frame();
	m_Font->Frame();
	m_DebugLine->Frame();
	m_pMainCamera->Frame();
	I_Input.Frame();
	if (I_Input.KeyCheck(VK_F6))
	{
		CDX::ApplyRS(m_pContext, CDX::KG_DxState::g_pRSWireFrame);
	}
	if (I_Input.KeyCheck(VK_F5))
	{
		CDX::ApplyRS(m_pContext, CDX::KG_DxState::g_pRSSold);
	}

	if (I_Input.KeyCheck(VK_F3))
	{
		bDebug = true;
	}
	if (I_Input.KeyCheck(VK_F4))
	{
		bDebug = false;
	}

	if (Frame())
	{
		return true;
	}
	else
		return false;
};

bool KG_Core::PreRender()													//렌더가 진행된후 다음 렌더로 넘어갈때 화면을 깨끗하게 배경으로 클리어해주는 역할
{
	if (m_pContext != nullptr)
	{
		if (m_pRenderTargetView != nullptr)
		{
			float ClearColor[4];
			ClearColor[0] = 0.0f;
			ClearColor[1] = 0.7f;
			ClearColor[2] = 1.0f;
			ClearColor[3] = 1.0f;
			m_pContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);							//더블 버퍼링에서 preRender에 해당하는 부분
		}

		if (m_pDepthStencilView != nullptr)
		{
			m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		}

		m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
		m_pContext->RSSetViewports(1, &m_ViewPort);		
		//뷰포트 세팅
		CDX::ApplyDSS(m_pContext, CDX::KG_DxState::g_pDSS);
		CDX::ApplyBS(m_pContext, CDX::KG_DxState::g_pAlpahBlend);
		CDX::ApplySS(m_pContext, CDX::KG_DxState::g_pSampler);
		m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	return true;
};

bool  KG_Core::CoreRender()
{
	PreRender();
	Render();
	if (bDebug)
	{
		Debug();
	}
	PostRender();
	//ClearContext();
	return true;

};

void KG_Core::Debug()
{
	RECT rt;
	TCHAR text[100];

	rt = Winrt;
	swprintf_s(text, L"FPS : %d, intersection : (%0.2f,%0.2f,%0.2f)", g_FPS, g_fPickpos.x, g_fPickpos.y, g_fPickpos.z);
	m_Font->TextDraw(rt, text, D2D1::ColorF(0.0f, 0.0f, 1.0f, 1.0f));

	D3DXVECTOR3 vS = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 vE = D3DXVECTOR3(1000, 0, 0);
	m_DebugLine->SetMatrix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
	m_DebugLine->Draw(vS, vE, D3DXVECTOR4(1, 0, 0, 1));

	vE = D3DXVECTOR3(0, 1000, 0);
	m_DebugLine->Draw(vS, vE, D3DXVECTOR4(0, 1, 0, 1));

	vE = D3DXVECTOR3(0, 0, 1000);
	m_DebugLine->Draw(vS, vE, D3DXVECTOR4(0, 0, 1, 1));

}

bool KG_Core::PostRender()
{
	m_pSwapChain->Present(0, 0);																//최종적으로 윈도우의 영역에 그려주는 함수이다.
	return true;
};

void KG_Core::ClearContext()
{
	ID3D11ShaderResourceView* pSRVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11RenderTargetView* pRTVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11DepthStencilView* pDSV = NULL;
	ID3D11Buffer* pBuffers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11SamplerState* pSamplers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	UINT StrideOffset[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	m_pContext->VSSetShader(NULL, NULL, 0);
	m_pContext->PSSetShader(NULL, NULL, 0);

	m_pContext->IASetVertexBuffers(0, 16, pBuffers, StrideOffset, StrideOffset);
	m_pContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
	m_pContext->IASetInputLayout(NULL);

	m_pContext->VSSetConstantBuffers(0, 14, pBuffers);
	m_pContext->PSSetConstantBuffers(0, 14, pBuffers);

	m_pContext->VSSetShaderResources(0, 16, pSRVs);
	m_pContext->VSSetShaderResources(0, 16, pSRVs);

	m_pContext->VSSetSamplers(0, 16, pSamplers);
	m_pContext->PSSetSamplers(0, 16, pSamplers);

	m_pContext->OMSetRenderTargets(8, pRTVs, pDSV);
	FLOAT blendFactor[4] = { 0,0,0,0 };
	m_pContext->OMSetBlendState(NULL, blendFactor, 0xFFFFFFFF);
	m_pContext->OMSetDepthStencilState(NULL, 0);
	m_pContext->RSSetState(NULL);
}

bool  KG_Core::CoreRelease()
{
	m_DebugLine->Release();
	m_pMainCamera->Release();
	m_Font->Release();
	m_Timer.Release();
	Release();
	DeviceRelease();
	return true;
};

KG_Core::KG_Core()
{
	GetGameRun = false;
	bDebug = false;
}


KG_Core::~KG_Core()
{
}

