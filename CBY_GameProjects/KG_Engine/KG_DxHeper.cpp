#include "KG_DxHeper.h"

namespace CDXH
{
	ID3D11InputLayout* CreateInputlayout(ID3D11Device*  pd3dDevice, DWORD dwSize, LPCVOID lpData, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements)
	{
		HRESULT hr = S_OK;
		ID3D11InputLayout* pInutLayout = nullptr;
		if (FAILED(hr = pd3dDevice->CreateInputLayout(layout, numElements, lpData, dwSize, &pInutLayout)))
		{
			return nullptr;
		}
		return pInutLayout;
	}
	ID3D11Buffer* CreateVertexBuffer(ID3D11Device*  pd3dDevice, void *vertices, UINT iNumVertex, UINT iVertexSize, bool bDynamic)
	{
		HRESULT hr = S_OK;
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		if (bDynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}
		bd.ByteWidth = iVertexSize * iNumVertex;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;
		if (vertices != NULL)
		{
			if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer)))
			{
				return nullptr;
			}
		}
		else
		{
			if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer)))
			{
				return nullptr;
			}
		}
		return pBuffer;
	}

	ID3D11Buffer* CreateIndexBuffer(ID3D11Device*  pd3dDevice, void *indices, UINT iNumIndex, UINT iSize, bool bDynamic)
	{
		HRESULT hr = S_OK;
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		if (bDynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}
		bd.ByteWidth = iSize * iNumIndex;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		InitData.pSysMem = indices;
		if (indices != NULL)
		{
			if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer)))
			{
				return nullptr;
			}
		}
		else
		{
			if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer)))
			{
				return nullptr;
			}
		}
		return pBuffer;
	}

	ID3D11Buffer* CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNum, UINT iSize, bool dynamic)
	{
		HRESULT hr = S_OK;
		ID3D11Buffer* conbuffer = nullptr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = iNum * iSize;								//버퍼의 크기
		if (dynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;								//버퍼의 읽기 쓰기 방법, Usage를 DYNAMIC으로 설정을 해야 cpu가 접근 가능하다
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;					//cpu가 접근하여 쓸수 있다.
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;								//버퍼의 읽기 쓰기 방법, Usage를 DYNAMIC으로 설정을 해야 cpu가 접근 가능하다
			bd.CPUAccessFlags = 0;					//cpu가 접근하여 쓸수 있다.
		}
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
		InitData.pSysMem = data;							//초기화 데이터에 대한 포인터

		if (data != nullptr)
		{
			hr = pd3dDevice->CreateBuffer(&bd, &InitData, &conbuffer);			//버퍼생성
			if (FAILED(hr))
			{
				return nullptr;
			}
		}
		else
		{
			hr = pd3dDevice->CreateBuffer(&bd, NULL, &conbuffer);			//버퍼생성
			if (FAILED(hr))
			{
				return nullptr;
			}
		}

		return conbuffer;
	}

	HRESULT CreateComputeShader(LPCWSTR pSrcFile, LPCSTR pFunctionName, ID3D11Device* pDevice, ID3D11ComputeShader** ppShaderOut)
	{
		HRESULT hr;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG ) 
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

		const D3D_SHADER_MACRO defines[] =
		{
	#ifdef USE_STRUCTURED_BUFFERS
			"USE_STRUCTURED_BUFFERS", "1",
	#endif
			NULL, NULL
		};

		// We generally prefer to use the higher CS shader profile when possible as CS 5.0 is better performance on 11-class hardware
		LPCSTR pProfile = (pDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "cs_5_0" : "cs_4_0";

		ID3DBlob* pErrorBlob = NULL;
		ID3DBlob* pBlob = NULL;
		hr = D3DX11CompileFromFile(pSrcFile, defines, NULL, pFunctionName, pProfile,
			dwShaderFlags, NULL, NULL, &pBlob, &pErrorBlob, NULL);
		if (FAILED(hr))
		{
			if (pErrorBlob)
				OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

			if (pErrorBlob)
			{
				pErrorBlob->Release();
				pErrorBlob = NULL;
			}
			if (pBlob)
			{
				pBlob->Release();
				pBlob = NULL;
			}

			return hr;
		}

		hr = pDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShaderOut);

		if (pErrorBlob)
		{
			pErrorBlob->Release();
			pErrorBlob = NULL;
		}
		if (pBlob)
		{
			pBlob->Release();
			pBlob = NULL;
		}

		return hr;
	}

	HRESULT CreateStructuredBuffer(ID3D11Device* pd3dDevice, UINT uElementSize, UINT uCount, VOID* pInitData, ID3D11Buffer** ppBufOut)
	{
		HRESULT hr = S_OK;
		*ppBufOut = NULL;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		desc.ByteWidth = uElementSize * uCount;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = uElementSize;

		if (pInitData)
		{
			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
			InitData.pSysMem = pInitData;
			hr = pd3dDevice->CreateBuffer(&desc, &InitData, ppBufOut);
			return hr;
		}
		else
		{
			hr = pd3dDevice->CreateBuffer(&desc, NULL, ppBufOut);
			return hr;
		}
	}


	HRESULT CreateBufferSRV(ID3D11Device* pd3dDevice, ID3D11Buffer* buf, ID3D11ShaderResourceView** srv)
	{
		HRESULT hr = S_OK;
		D3D11_BUFFER_DESC descBuf;
		ZeroMemory(&descBuf, sizeof(descBuf));
		buf->GetDesc(&descBuf);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		desc.BufferEx.FirstElement = 0;

		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

		hr = pd3dDevice->CreateShaderResourceView(buf, &desc, srv);
		return hr;
	}

	HRESULT CreateBufferUAV(ID3D11Device* pd3dDevice, ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppSRVOut)
	{
		HRESULT hr = S_OK;
		D3D11_BUFFER_DESC descBuf;
		ZeroMemory(&descBuf, sizeof(descBuf));
		pBuffer->GetDesc(&descBuf);

		D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		desc.Buffer.FirstElement = 0;

		if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
		{
			// This is a Raw Buffer

			desc.Format = DXGI_FORMAT_R32_TYPELESS; // Format must be DXGI_FORMAT_R32_TYPELESS, when creating Raw Unordered Access View
			desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
			desc.Buffer.NumElements = descBuf.ByteWidth / 4;
		}
		else
			if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
			{
				// This is a Structured Buffer

				desc.Format = DXGI_FORMAT_UNKNOWN;      // Format must be must be DXGI_FORMAT_UNKNOWN, when creating a View of a Structured Buffer
				desc.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
			}
			else
			{
				return E_INVALIDARG;
			}
		hr = pd3dDevice->CreateUnorderedAccessView(pBuffer, &desc, ppSRVOut);
		return hr;
	}

	HRESULT CreateTextureUAV(ID3D11Device* pd3dDevice, ID3D11Texture2D* pBuffer, ID3D11UnorderedAccessView** ppSRVOut)
	{
		HRESULT hr = S_OK;
		D3D11_TEXTURE2D_DESC descBuf;
		ZeroMemory(&descBuf, sizeof(descBuf));
		pBuffer->GetDesc(&descBuf);

		D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Texture2D.MipSlice = 0;
		desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		desc.Format = DXGI_FORMAT_UNKNOWN;

		hr = pd3dDevice->CreateUnorderedAccessView(pBuffer, &desc, ppSRVOut);
		return hr;
	}


	HRESULT RunComputeShader(ID3D11DeviceContext* pd3dContext, ID3D11ComputeShader* pComputeShader, UINT nNumViews, ID3D11ShaderResourceView** pShaderResourceView,
		ID3D11ShaderResourceView** m_pTextureResultSRV,
		ID3D11UnorderedAccessView* pUnorderedAccessView, UINT X, UINT Y, UINT Z)
	{
		HRESULT hr = S_OK;
		pd3dContext->CSSetShader(pComputeShader, NULL, 0);
		pd3dContext->CSSetShaderResources(0, 1, m_pTextureResultSRV);
		pd3dContext->CSSetShaderResources(1, nNumViews, pShaderResourceView);
		pd3dContext->CSSetUnorderedAccessViews(0, 1, &pUnorderedAccessView, NULL);
		pd3dContext->Dispatch(X, Y, Z);



		//해제 작업
		pd3dContext->CSSetShader(NULL, NULL, 0);

		ID3D11UnorderedAccessView* pUAViewNULL[1] = { NULL };
		pd3dContext->CSSetUnorderedAccessViews(0, 1, pUAViewNULL, NULL);

		ID3D11ShaderResourceView* pTexSRVNULL[1] = { NULL };
		ID3D11ShaderResourceView* pSRVNULL[1] = { NULL };
		pd3dContext->CSSetShaderResources(0, 1, pTexSRVNULL);
		pd3dContext->CSSetShaderResources(1, nNumViews, pSRVNULL);

		ID3D11Buffer* pCBNULL[1] = { NULL };
		pd3dContext->CSSetConstantBuffers(0, 1, pCBNULL);

		return S_OK;
	}

}


KG_DxHeper::KG_DxHeper()
{
}


KG_DxHeper::~KG_DxHeper()
{
}
