#pragma once
#include "KG_Window.h"


class KG_Device :public KG_Window
{
public:
	D3D11_VIEWPORT m_ViewPort;
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	IDXGIFactory* m_pGIFactory;
	DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;
	ID3D11Texture2D* m_pDepthTex;

public:
	HRESULT CreateDevice();
	HRESULT CreateGIFactory();
	HRESULT CreateSwapChain();
	HRESULT SetRenderTargetView();
	void SetViewPort();
	bool ResizeDevice(UINT width, UINT height);		//윈도우 화면 크기가 바뀔때마다 백버퍼도 같이 크기 변경해줄 수 있게 해주는 함수
	virtual HRESULT CreateDXrc();
	virtual HRESULT	DeleteDXrc();
	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


public:
	bool DeviceInit();
	bool DeviceRelease();

public:
	KG_Device();
	~KG_Device();
};
