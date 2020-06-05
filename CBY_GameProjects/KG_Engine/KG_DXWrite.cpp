#include "KG_DXWrite.h"

bool KG_DXWrite::Set(HWND hWnd, int Width, int Height, IDXGISurface* pSurface)
{
	HRESULT hr = S_OK;

	hr = CreateDeviceIndependentResources();										//��ü ����
	if (FAILED(hr))
	{
		return false;
	}

	hr = CreateDeviceResources(pSurface);										//����̽� ���� ��ü ����
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

HRESULT KG_DXWrite::CreateDeviceResources(IDXGISurface* pSurface)
{
	HRESULT hr = S_OK;

	D2D1_RENDER_TARGET_PROPERTIES sr;
	ZeroMemory(&sr, sizeof(D2D1_RENDER_TARGET_PROPERTIES));		//���� Ÿ�ٿ� �Էµ� ����
	sr.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	sr.pixelFormat.format = DXGI_FORMAT_UNKNOWN;
	sr.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	sr.dpiX = m_fdpiX;
	sr.dpiY = m_fdpiY;
	sr.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	sr.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	hr = m_pD2DFactory->CreateDxgiSurfaceRenderTarget(pSurface, &sr, &m_pRT);
	if (FAILED(hr))
	{
		return hr;
	}
	m_pRT->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 1), &m_pBlackBrush);			//������ ����Ʈ ���� �귯���� ���� �Է»���

	return hr;

}

HRESULT KG_DXWrite::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_pD2DFactory);		//D2D���丮�� ��Ƽ������ �������� ����
	if (FAILED(hr))
	{
		return hr;
	}

	m_pD2DFactory->GetDesktopDpi(&m_fdpiX, &m_fdpiY);								//���� �ý����� DPI�� ���� �޴´�
	m_fDPIScaleX = m_fdpiX / 96.0f;													//
	m_fDPIScaleY = m_fdpiY / 96.0f;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pWriteFactory);	//
	if (FAILED(hr))
	{
		return hr;
	}

	//���� ���� ����
	hr = m_pWriteFactory->CreateTextFormat(L"�ü�", NULL, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30, L"ko-kr", &m_pTextFormat);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

void KG_DXWrite::DiscardDeviceResources()
{
	if (m_pBlackBrush != nullptr)
	{
		m_pBlackBrush->Release();
	}

	if (m_pRT != nullptr)
	{
		m_pRT->Release();
	}
}

void KG_DXWrite::DiscardDeviceIndependentResources()
{
	if (m_pTextFormat != nullptr)
	{
		m_pTextFormat->Release();
	}
	if (m_pWriteFactory != nullptr)
	{
		m_pWriteFactory->Release();
	}
	if (m_pD2DFactory != nullptr)
	{
		m_pD2DFactory->Release();
	}
}


bool KG_DXWrite::Begin()
{
	if (m_pRT)				//���µ� ����Ǹ� �̻��ϴ� ���� �˻�
	{
		m_pRT->BeginDraw();
		D2D1::Matrix3x2F matWorld = D2D1::Matrix3x2F::Identity();
		m_pRT->SetTransform(matWorld);
	}
	return true;
}

bool KG_DXWrite::End()
{
	m_pRT->EndDraw();
	return true;
}

bool KG_DXWrite::TextDraw(RECT rt, const TCHAR* Text, D2D1::ColorF color)
{
	Begin();

	D2D_RECT_F textRt;
	textRt.left = (float)rt.left / m_fDPIScaleX;
	textRt.right = (float)rt.right / m_fDPIScaleX;
	textRt.top = (float)rt.top / m_fDPIScaleY;
	textRt.bottom = (float)rt.bottom / m_fDPIScaleY;

	m_pBlackBrush->SetColor(color);

	m_pRT->DrawText(Text, wcslen(Text), m_pTextFormat, textRt, m_pBlackBrush);

	End();
	return true;
}


bool KG_DXWrite::Init()
{
	return true;
}

bool KG_DXWrite::Frame()
{
	return true;
}

bool KG_DXWrite::Render()
{
	return true;
}

bool KG_DXWrite::Release()
{
	DiscardDeviceResources();
	DiscardDeviceIndependentResources();

	return true;
}


KG_DXWrite::KG_DXWrite()
{
}


KG_DXWrite::~KG_DXWrite()
{
}

