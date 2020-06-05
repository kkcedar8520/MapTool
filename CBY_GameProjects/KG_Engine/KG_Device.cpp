#include "KG_Device.h"

LRESULT KG_Device::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return KG_Window::WndProc(hWnd, message, wParam, lParam);
}

bool KG_Device::DeviceInit()
{
	HRESULT hr;
	if (FAILED(hr = CreateDevice()))						//백버퍼가 생성 되었는지 확인 FAILED 매크로를 이용한 이유는 실패했을때의 결과값이 음수이고 직접적으로 쓰기보다 매크로를 이용함이 좋다
	{
		return false;
	}

	if (FAILED(hr = CreateGIFactory()))
	{
		return false;
	}

	if (FAILED(hr = CreateSwapChain()))
	{
		return false;
	}

	if (FAILED(hr = SetRenderTargetView()))
	{
		return false;
	}

	SetViewPort();



	return true;
}

HRESULT KG_Device::CreateDevice()
{
	HRESULT hr = S_OK;


	D3D_FEATURE_LEVEL retFeatureLevel;															//하드웨어와 호환되는 FeatureLevel의 결과값을 저장
	D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_HARDWARE;										//드라이버 타입을 어떻게 할지, 하드웨어,소프트, 등등 있지만 게임 속도와 프레임을 위해 하드웨어
	UINT Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	D3D_FEATURE_LEVEL pFeatureLevel[] = { D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_10_0 };		//하드웨어가 처음 11_1을 호환하는지 안될시에 11_0을 하라는 뜻
	UINT FeatureLevels = 2;																		//FeatureLevel 갯수

	hr = D3D11CreateDevice(NULL, DriverType, NULL, Flags, pFeatureLevel, FeatureLevels,
		D3D11_SDK_VERSION, &m_pd3dDevice, &retFeatureLevel, &m_pContext);			//백버퍼 정보 입력


	return hr;
}

HRESULT KG_Device::CreateGIFactory()
{
	HRESULT hr = S_OK;

	IDXGIDevice * pDXGIDevice;
	hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);

	IDXGIAdapter * pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);			//디바이스로부터 어뎁터 설정이되어 팩토리에 넘겨줘서 GI팩토리를 만든다

	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_pGIFactory);

	pDXGIDevice->Release();
	pDXGIAdapter->Release();

	return hr;
}

HRESULT KG_Device::CreateSwapChain()
{
	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC ch;								//더블 버퍼링을 위한 정보 생성
	ZeroMemory(&ch, sizeof(ch));
	ch.BufferCount = 1;										//백버퍼의 갯수
	ch.BufferDesc.Width = Winrt.right;						//백버퍼의 폭
	ch.BufferDesc.Height = Winrt.bottom;					//백버퍼의 높이
	ch.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//백버퍼의 포맷상태, 32비트 언사인 노말로 만든다 선언한 모습이다.
	ch.BufferDesc.RefreshRate.Numerator = 60;				//RefreshRate는 주사율
	ch.BufferDesc.RefreshRate.Denominator = 1;				// 60/1(?)
	ch.SampleDesc.Count = 1;								//백버퍼의 샘플링(계단효과를 사라지게 해주는데 Count는 최소 1은 지정되어있어야 함
	ch.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//백버퍼의 사용 용도 그리기 연산을 백버퍼에 저장하기 위해 이와 같이 선언
	ch.OutputWindow = g_hwnd;								//백버퍼를 나타낼 윈도우핸들
	ch.Windowed = TRUE;										//창모드로 실행할지에 대한 여부
	ch.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = m_pGIFactory->CreateSwapChain(m_pd3dDevice, &ch, &m_pSwapChain);			//백버퍼 정보 입력

	if (FAILED(hr))
	{
		return false;
	}
	hr = m_pGIFactory->MakeWindowAssociation(g_hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);		//윈도우창의 조건을 걸어주는것으로 윈도우 변경과 알트 엔터를 막았다
	if (FAILED(hr))
	{
		return false;
	}


	hr = m_pSwapChain->GetDesc(&m_SwapChainDesc);

	if (FAILED(hr))
	{
		return false;
	}



	return hr;
}

HRESULT KG_Device::SetRenderTargetView()
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* pBackBuffer;							//백버퍼

	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);			//백버퍼를 생성, 1번째 인자는 백버퍼의 인덱스이며 지금 만든 버퍼의 갯수는 1개이니 0,
																								//2번째 인자는 백버퍼의 반환되는 인터페이스 타입
	if (FAILED(hr))						//생성 확인
	{
		return false;
	}

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);			//RenderTargetView를 생성하는 작업
	if (FAILED(hr))						//생성 확인
	{
		return false;
	}
	pBackBuffer->Release();

	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);								//렌더타겟을 사용할수 있게 해주는 함수로 1번째 인자는 렌더타겟 갯수, 2번째는 주소

#pragma region dss용 텍스처 생성
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
	td.Width = m_SwapChainDesc.BufferDesc.Width;
	td.Height = m_SwapChainDesc.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = m_pd3dDevice->CreateTexture2D(&td, NULL, &m_pDepthTex);
#pragma endregion

#pragma region 깊이스텐실버퍼 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv;
	ZeroMemory(&dsv, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsv.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(
		m_pDepthTex,
		&dsv,
		&m_pDepthStencilView);
#pragma endregion
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//pDepthTex->Release();

	return hr;
}

void KG_Device::SetViewPort()
{
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;									//어디에서부터 뿌릴건지에 대한 좌표
	m_ViewPort.Width = Winrt.right;
	m_ViewPort.Height = Winrt.bottom;							//얼마만큼 뿌릴지
	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;									//z값으로 무조건 0~1이다.

	m_pContext->RSSetViewports(1, &m_ViewPort);					//뷰포트 세팅
}

bool KG_Device::ResizeDevice(UINT width, UINT height)
{
	HRESULT hr = S_OK;

	if (m_pd3dDevice == NULL || m_pRenderTargetView == NULL)			//디바이스와 렌더타겟뷰가 널이면 돌릴 생성 되기 전이거나 끝난거니 할 필요가 없다
	{
		return true;
	}

	hr = DeleteDXrc();
	if (FAILED(hr))
	{
		return hr;
	}

	m_pContext->OMSetRenderTargets(0, NULL, NULL);						//렌덤타겟뷰를 초기화 시켜준다.
	m_pRenderTargetView->Release();										//삭제
	m_pRenderTargetView = nullptr;

	hr = m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, width, height, m_SwapChainDesc.BufferDesc.Format, m_SwapChainDesc.Flags);	//백버퍼를 다시 수정해준다
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pSwapChain->GetDesc(&m_SwapChainDesc);		//수정된 소스를 다시 저장한다
	if (FAILED(hr))
	{
		return hr;
	}

	hr = SetRenderTargetView();						//랜타뷰 생성

	if (FAILED(hr))
	{
		return false;
	}

	SetViewPort();									//뷰포트 다시

	hr = CreateDXrc();
	if (FAILED(hr))
	{
		return hr;
	}


	return true;
}

HRESULT KG_Device::CreateDXrc()
{
	HRESULT hr = S_OK;
	return hr;
}

HRESULT KG_Device::DeleteDXrc()
{
	HRESULT hr = S_OK;
	return hr;
}


bool KG_Device::DeviceRelease()
{
	m_pRenderTargetView->Release();
	m_pSwapChain->Release();
	m_pd3dDevice->Release();
	m_pContext->Release();
	return true;
}

KG_Device::KG_Device()
{
}


KG_Device::~KG_Device()
{
}
