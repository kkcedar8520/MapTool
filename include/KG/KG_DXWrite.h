#pragma once
#include "KG_DxState.h"
#include <d2d1.h>
#include <dwrite.h>
#include <queue>
#pragma comment( lib, "d2d1.lib")
#pragma comment( lib, "dwrite.lib")

class KG_DXWrite
{
public:
	ID2D1RenderTarget* m_pRT;
	ID2D1Factory* m_pD2DFactory;
	IDWriteFactory* m_pWriteFactory;
	ID2D1SolidColorBrush* m_pBlackBrush;
	IDWriteTextFormat* m_pTextFormat;

public:
	float m_fDPIScaleX;
	float m_fDPIScaleY;
	FLOAT m_fdpiX;
	FLOAT m_fdpiY;

public:
	bool Set(HWND hWnd, int Width, int Height, IDXGISurface* pSurface);
	bool Begin();
	bool End();
	HRESULT CreateDeviceResources(IDXGISurface* pSurface);
	HRESULT CreateDeviceIndependentResources();			//	辆加等 按眉 积己
	void DiscardDeviceResources();
	void DiscardDeviceIndependentResources();
	bool TextDraw(RECT rt, const TCHAR* Text, D2D1::ColorF color);

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	KG_DXWrite();
	~KG_DXWrite();
};

