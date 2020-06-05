#pragma once
#include "KG_DxState.h"
#include "KG_Std.h"

class KG_DxRT
{
public:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;				//새로 만들 랜더타켓뷰
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;				//랜더타켓뷰를 새로 만들때 같이 뎁스도 만들어 져야한다
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture;								//랜더타켓 뷰에 그려질 텍스쳐
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pDsvSRV;
	D3D11_DEPTH_STENCIL_VIEW_DESC			m_DepthStencilDesc;
	DXGI_FORMAT								m_DSFormat;
	D3D11_VIEWPORT			m_vp;													//쓰일 뷰포트
	D3D11_VIEWPORT			m_vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];		//D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX는 뷰포트를 최대 가질수 있는 수다.
	UINT					m_iViewPorts;
	ID3D11RenderTargetView* m_pOldRTV;
	ID3D11DepthStencilView* m_pOldDSV;

public:
	virtual HRESULT Create(ID3D11Device* pd3dDevice, float fWidth = 1024, float fHeight = 1024);
	HRESULT UpdateDepthStencilView(ID3D11Device* pDevice, UINT Width, UINT Height);
	virtual bool Begin(ID3D11DeviceContext* pContext, D3DXVECTOR4 vColor = { 0, 0, 0, 1 });
	virtual bool End(ID3D11DeviceContext* pContext);
	virtual bool Release();
	virtual HRESULT SaveFile(ID3D11DeviceContext* pContext, T_STR name);

public:
	KG_DxRT();
	virtual ~KG_DxRT();
};
