#pragma once
#include "KG_DxHeper.h"
namespace CDX
{
	class KG_DxState
	{
	public:
		static ID3D11SamplerState*  g_pSampler;
		static ID3D11SamplerState*  g_pSamplShadow;
		static ID3D11SamplerState* g_pSSClampPoint;
		static ID3D11SamplerState* g_pSSClampLinear;

		static ID3D11BlendState* g_pAlpahBlend;
		static ID3D11BlendState* g_pAlpahTesting;
		static ID3D11BlendState* g_pAlpahBlendDisable;

		static ID3D11RasterizerState* g_pRSSold;
		static ID3D11RasterizerState* g_pRSWireFrame;
		static ID3D11RasterizerState* g_pRSDepth;

		static ID3D11DepthStencilState* g_pDSS;
		static ID3D11DepthStencilState* g_pDSSDisable;

	public:
		static HRESULT SetState(ID3D11Device* pd3dDevice);
		static bool Release();

		// tool & edit
	public:
		static Microsoft::WRL::ComPtr<ID3D11SamplerState> g_pSSEdit;
		static Microsoft::WRL::ComPtr<ID3D11BlendState>	g_pBSEdit;
		static Microsoft::WRL::ComPtr<ID3D11RasterizerState> g_pRSEdit;
		static Microsoft::WRL::ComPtr<ID3D11DepthStencilState> g_pDSSEdit;

		static D3D11_SAMPLER_DESC g_SamplerDesc;			//基敲矾 积己 单捞磐
		static D3D11_BLEND_DESC g_BlendDesc;				//喉坊爹 积己 单捞磐
		static D3D11_RASTERIZER_DESC g_RasterizerDesc;		//RS积己 单捞磐
		static D3D11_DEPTH_STENCIL_DESC g_DepthDesc;

		static HRESULT SetSamplerState(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, D3D11_SAMPLER_DESC* pDesc, UINT StartSlot = 0, UINT NumSamplers = 1);
		static HRESULT SetBlendState(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, D3D11_BLEND_DESC* pDesc, FLOAT BlendFactor[] = 0, UINT SampleMask = 0xff);
		static HRESULT SetRasterizerState(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, D3D11_RASTERIZER_DESC* pDesc);
		static HRESULT SetDepthStencilState(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, D3D11_DEPTH_STENCIL_DESC* pDesc, UINT StencilRef = 0x01);

	public:
		KG_DxState();
		~KG_DxState();
	};

	static void ApplyRS(ID3D11DeviceContext* pContext, ID3D11RasterizerState*	pRS)
	{
		pContext->RSSetState(pRS);
	}
	static void ApplyDSS(ID3D11DeviceContext* pContext, ID3D11DepthStencilState*	pDSS, UINT StencilRef = 0x01)
	{
		pContext->OMSetDepthStencilState(pDSS, StencilRef);
	}
	static void ApplyBS(ID3D11DeviceContext* pContext, ID3D11BlendState*	pBS, FLOAT BlendFactor[] = 0, UINT SampleMask = 0xff)
	{
		pContext->OMSetBlendState(pBS, BlendFactor, SampleMask);
	}
	static void ApplySS(ID3D11DeviceContext* pContext, ID3D11SamplerState*	pSS, UINT StartSlot = 0, UINT NumSamplers = 1)
	{
		pContext->PSSetSamplers(StartSlot, NumSamplers, &pSS);
	}
}

