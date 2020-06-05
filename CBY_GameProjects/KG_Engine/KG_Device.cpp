#include "KG_Device.h"

LRESULT KG_Device::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return KG_Window::WndProc(hWnd, message, wParam, lParam);
}

bool KG_Device::DeviceInit()
{
	HRESULT hr;
	if (FAILED(hr = CreateDevice()))						//����۰� ���� �Ǿ����� Ȯ�� FAILED ��ũ�θ� �̿��� ������ ������������ ������� �����̰� ���������� ���⺸�� ��ũ�θ� �̿����� ����
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


	D3D_FEATURE_LEVEL retFeatureLevel;															//�ϵ����� ȣȯ�Ǵ� FeatureLevel�� ������� ����
	D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_HARDWARE;										//����̹� Ÿ���� ��� ����, �ϵ����,����Ʈ, ��� ������ ���� �ӵ��� �������� ���� �ϵ����
	UINT Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	D3D_FEATURE_LEVEL pFeatureLevel[] = { D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_10_0 };		//�ϵ��� ó�� 11_1�� ȣȯ�ϴ��� �ȵɽÿ� 11_0�� �϶�� ��
	UINT FeatureLevels = 2;																		//FeatureLevel ����

	hr = D3D11CreateDevice(NULL, DriverType, NULL, Flags, pFeatureLevel, FeatureLevels,
		D3D11_SDK_VERSION, &m_pd3dDevice, &retFeatureLevel, &m_pContext);			//����� ���� �Է�


	return hr;
}

HRESULT KG_Device::CreateGIFactory()
{
	HRESULT hr = S_OK;

	IDXGIDevice * pDXGIDevice;
	hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);

	IDXGIAdapter * pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);			//����̽��κ��� ��� �����̵Ǿ� ���丮�� �Ѱ��༭ GI���丮�� �����

	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_pGIFactory);

	pDXGIDevice->Release();
	pDXGIAdapter->Release();

	return hr;
}

HRESULT KG_Device::CreateSwapChain()
{
	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC ch;								//���� ���۸��� ���� ���� ����
	ZeroMemory(&ch, sizeof(ch));
	ch.BufferCount = 1;										//������� ����
	ch.BufferDesc.Width = Winrt.right;						//������� ��
	ch.BufferDesc.Height = Winrt.bottom;					//������� ����
	ch.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//������� ���˻���, 32��Ʈ ����� �븻�� ����� ������ ����̴�.
	ch.BufferDesc.RefreshRate.Numerator = 60;				//RefreshRate�� �ֻ���
	ch.BufferDesc.RefreshRate.Denominator = 1;				// 60/1(?)
	ch.SampleDesc.Count = 1;								//������� ���ø�(���ȿ���� ������� ���ִµ� Count�� �ּ� 1�� �����Ǿ��־�� ��
	ch.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//������� ��� �뵵 �׸��� ������ ����ۿ� �����ϱ� ���� �̿� ���� ����
	ch.OutputWindow = g_hwnd;								//����۸� ��Ÿ�� �������ڵ�
	ch.Windowed = TRUE;										//â���� ���������� ���� ����
	ch.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = m_pGIFactory->CreateSwapChain(m_pd3dDevice, &ch, &m_pSwapChain);			//����� ���� �Է�

	if (FAILED(hr))
	{
		return false;
	}
	hr = m_pGIFactory->MakeWindowAssociation(g_hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);		//������â�� ������ �ɾ��ִ°����� ������ ����� ��Ʈ ���͸� ���Ҵ�
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

	ID3D11Texture2D* pBackBuffer;							//�����

	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);			//����۸� ����, 1��° ���ڴ� ������� �ε����̸� ���� ���� ������ ������ 1���̴� 0,
																								//2��° ���ڴ� ������� ��ȯ�Ǵ� �������̽� Ÿ��
	if (FAILED(hr))						//���� Ȯ��
	{
		return false;
	}

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);			//RenderTargetView�� �����ϴ� �۾�
	if (FAILED(hr))						//���� Ȯ��
	{
		return false;
	}
	pBackBuffer->Release();

	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);								//����Ÿ���� ����Ҽ� �ְ� ���ִ� �Լ��� 1��° ���ڴ� ����Ÿ�� ����, 2��°�� �ּ�

#pragma region dss�� �ؽ�ó ����
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

#pragma region ���̽��ٽǹ��� ����
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
	m_ViewPort.TopLeftY = 0;									//��𿡼����� �Ѹ������� ���� ��ǥ
	m_ViewPort.Width = Winrt.right;
	m_ViewPort.Height = Winrt.bottom;							//�󸶸�ŭ �Ѹ���
	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;									//z������ ������ 0~1�̴�.

	m_pContext->RSSetViewports(1, &m_ViewPort);					//����Ʈ ����
}

bool KG_Device::ResizeDevice(UINT width, UINT height)
{
	HRESULT hr = S_OK;

	if (m_pd3dDevice == NULL || m_pRenderTargetView == NULL)			//����̽��� ����Ÿ�ٺ䰡 ���̸� ���� ���� �Ǳ� ���̰ų� �����Ŵ� �� �ʿ䰡 ����
	{
		return true;
	}

	hr = DeleteDXrc();
	if (FAILED(hr))
	{
		return hr;
	}

	m_pContext->OMSetRenderTargets(0, NULL, NULL);						//����Ÿ�ٺ並 �ʱ�ȭ �����ش�.
	m_pRenderTargetView->Release();										//����
	m_pRenderTargetView = nullptr;

	hr = m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, width, height, m_SwapChainDesc.BufferDesc.Format, m_SwapChainDesc.Flags);	//����۸� �ٽ� �������ش�
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pSwapChain->GetDesc(&m_SwapChainDesc);		//������ �ҽ��� �ٽ� �����Ѵ�
	if (FAILED(hr))
	{
		return hr;
	}

	hr = SetRenderTargetView();						//��Ÿ�� ����

	if (FAILED(hr))
	{
		return false;
	}

	SetViewPort();									//����Ʈ �ٽ�

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
