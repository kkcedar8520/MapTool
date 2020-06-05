#include "KG_DxState.h"

namespace CDX
{
	ID3D11SamplerState*  KG_DxState::g_pSampler = nullptr;
	ID3D11SamplerState*  KG_DxState::g_pSamplShadow = nullptr;
	ID3D11SamplerState*  KG_DxState::g_pSSClampPoint = nullptr;
	ID3D11SamplerState*  KG_DxState::g_pSSClampLinear = nullptr;

	ID3D11BlendState* KG_DxState::g_pAlpahBlend = nullptr;
	ID3D11BlendState* KG_DxState::g_pAlpahBlendDisable = nullptr;

	ID3D11RasterizerState* KG_DxState::g_pRSSold = nullptr;
	ID3D11RasterizerState* KG_DxState::g_pRSWireFrame = nullptr;
	ID3D11RasterizerState* KG_DxState::g_pRSDepth = nullptr;

	ID3D11DepthStencilState* KG_DxState::g_pDSS = nullptr;
	ID3D11DepthStencilState* KG_DxState::g_pDSSDisable = nullptr;


	//Tool
	Microsoft::WRL::ComPtr<ID3D11SamplerState> KG_DxState::g_pSSEdit = 0;
	Microsoft::WRL::ComPtr<ID3D11BlendState>	KG_DxState::g_pBSEdit = 0;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> KG_DxState::g_pRSEdit = 0;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> KG_DxState::g_pDSSEdit = 0;

	D3D11_SAMPLER_DESC KG_DxState::g_SamplerDesc;
	D3D11_BLEND_DESC KG_DxState::g_BlendDesc = { 0, };
	D3D11_RASTERIZER_DESC KG_DxState::g_RasterizerDesc;
	D3D11_DEPTH_STENCIL_DESC KG_DxState::g_DepthDesc = { 0, };


	HRESULT KG_DxState::SetState(ID3D11Device* pd3dDevice)
	{
		HRESULT hr;
		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(rd));
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_NONE;
		pd3dDevice->CreateRasterizerState(&rd, &g_pRSSold);

		rd.FillMode = D3D11_FILL_WIREFRAME;
		rd.CullMode = D3D11_CULL_NONE;
		pd3dDevice->CreateRasterizerState(&rd, &g_pRSWireFrame);

		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		rd.DepthBias = 10000;
		rd.DepthBiasClamp = 0.0f;
		rd.SlopeScaledDepthBias = 1.0f;
		pd3dDevice->CreateRasterizerState(&rd, &g_pRSDepth);

		D3D11_SAMPLER_DESC Samp;
		ZeroMemory(&Samp, sizeof(D3D11_SAMPLER_DESC));
		Samp.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		Samp.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		Samp.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		Samp.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		pd3dDevice->CreateSamplerState(&Samp, &g_pSampler);


		D3D11_BLEND_DESC bs;
		ZeroMemory(&bs, sizeof(D3D11_BLEND_DESC));

		bs.IndependentBlendEnable = TRUE;						//���������� �ΰڴ�
		bs.RenderTarget[0].BlendEnable = TRUE;					//���� ������ �ϰڴ� ���ϰڴ�

		bs.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;				//�ҽ� ����
		bs.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;		//����ũ ����
		bs.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;					//���� ���� ��� (+�� ����)

		bs.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;				//�ҽ� ���� ��
		bs.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;			//����ũ ���İ�
		bs.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;			//���İ� ����
		//SRCAlpha*1+DEST*0

		D3D11_SAMPLER_DESC SamDescShad =
		{
			D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,// D3D11_FILTER Filter;
			D3D11_TEXTURE_ADDRESS_BORDER, //D3D11_TEXTURE_ADDRESS_MODE AddressU;
			D3D11_TEXTURE_ADDRESS_BORDER, //D3D11_TEXTURE_ADDRESS_MODE AddressV;
			D3D11_TEXTURE_ADDRESS_BORDER, //D3D11_TEXTURE_ADDRESS_MODE AddressW;
			0,//FLOAT MipLODBias;
			0,//UINT MaxAnisotropy;
			D3D11_COMPARISON_LESS , //D3D11_COMPARISON_FUNC ComparisonFunc;
			0.0,0.0,0.0,0.0,//FLOAT BorderColor[ 4 ];
			0,//FLOAT MinLOD;
			0//FLOAT MaxLOD;   
		};
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&SamDescShad, &g_pSamplShadow)))
		{
			return hr;
		}

		SamDescShad.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamDescShad.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamDescShad.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamDescShad.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&SamDescShad, &g_pSSClampLinear)))
		{
			return hr;
		}

		SamDescShad.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamDescShad.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamDescShad.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamDescShad.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&SamDescShad, &g_pSSClampPoint)))
		{
			return hr;
		}


		bs.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;			//�� ����

		hr = pd3dDevice->CreateBlendState(&bs, &g_pAlpahBlend);						//���� ������ ������ ���� ������Ʈ ����
		if (FAILED(hr))
		{
			return false;
		}

		bs.RenderTarget[0].BlendEnable = FALSE;
		hr = pd3dDevice->CreateBlendState(&bs, &g_pAlpahBlendDisable);
		if (FAILED(hr))
		{
			return false;
		}

		D3D11_DEPTH_STENCIL_DESC dsd;
		ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
		// Z ���� �� Ȱ��ȭ
		dsd.DepthEnable = TRUE;
		// z�� ���ۿ� ����
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// �� ����
		dsd.DepthFunc = D3D11_COMPARISON_LESS;
		pd3dDevice->CreateDepthStencilState(&dsd, &g_pDSS);
		dsd.DepthEnable = FALSE;
		pd3dDevice->CreateDepthStencilState(&dsd, &g_pDSSDisable);

		return hr;

	}

	HRESULT KG_DxState::SetSamplerState(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, D3D11_SAMPLER_DESC* pDesc, UINT StartSlot, UINT NumSamplers)
	{
		HRESULT hr = S_OK;

		g_pSSEdit.Reset();		//�ʱ�ȭ

		D3D11_SAMPLER_DESC* desc = &g_SamplerDesc;
		if (pDesc != nullptr)
		{
			desc = pDesc;
		}
		g_SamplerDesc = *desc;

		hr = pd3dDevice->CreateSamplerState(desc, g_pSSEdit.GetAddressOf());
		if (FAILED(hr))
		{
			return hr;
		}
		ApplySS(pContext, g_pSSEdit.Get(), StartSlot, NumSamplers);

		return hr;
	}

	HRESULT KG_DxState::SetBlendState(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, D3D11_BLEND_DESC* pDesc, FLOAT BlendFactor[], UINT SampleMask)
	{
		HRESULT hr = S_OK;

		g_pBSEdit.Reset();

		D3D11_BLEND_DESC* Desc = &g_BlendDesc;
		if (pDesc != nullptr)
		{
			Desc = pDesc;
		}
		g_BlendDesc = *Desc;
		hr = pd3dDevice->CreateBlendState(Desc,
			g_pBSEdit.GetAddressOf());
		if (FAILED(hr))
		{
			return hr;
		}
		ApplyBS(pContext, g_pBSEdit.Get(), BlendFactor, SampleMask);

		return hr;
	}

	HRESULT KG_DxState::SetRasterizerState(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, D3D11_RASTERIZER_DESC* pDesc)
	{
		HRESULT hr = S_OK;

		D3D11_RASTERIZER_DESC* Desc = &g_RasterizerDesc;
		if (pDesc != nullptr)
		{
			Desc = pDesc;
		}
		g_RasterizerDesc = *Desc;

		hr = pd3dDevice->CreateRasterizerState(Desc,
			g_pRSEdit.GetAddressOf());
		if (FAILED(hr))
		{
			return hr;
		}
		ApplyRS(pContext, g_pRSEdit.Get());

		return hr;
	}

	HRESULT KG_DxState::SetDepthStencilState(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, D3D11_DEPTH_STENCIL_DESC* pDesc, UINT StencilRef)
	{
		HRESULT hr = S_OK;

		g_pDSSEdit.Reset();

		D3D11_DEPTH_STENCIL_DESC* Desc = &g_DepthDesc;
		if (pDesc != nullptr)
		{
			Desc = pDesc;
		}
		g_DepthDesc = *Desc;
		hr = pd3dDevice->CreateDepthStencilState(Desc,
			g_pDSSEdit.GetAddressOf());
		if (FAILED(hr))
		{
			return hr;
		}
		ApplyDSS(pContext, g_pDSSEdit.Get(), StencilRef);

		return hr;
	}

	bool KG_DxState::Release()
	{
		g_pDSS->Release();
		g_pSampler->Release();
		g_pAlpahBlend->Release();
		g_pAlpahBlendDisable->Release();
		g_pRSWireFrame->Release();
		g_pRSSold->Release();
		g_pDSSDisable->Release();

		return true;
	}


	KG_DxState::KG_DxState()
	{
	}


	KG_DxState::~KG_DxState()
	{
	}
}
