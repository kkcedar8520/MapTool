#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <d3dx11.h>
#include <D3DX10Math.h>
#include "DirectXTex.h"

#include <wrl.h>
#include <memory>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#if defined(_DEBUG)
#pragma comment(lib, "d3dx11d.lib")
#pragma comment(lib, "d3dx10d.lib")
#pragma comment(lib, "DirectXTex.lib")
#else
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "DirectXTex_R.lib")
#endif

namespace CDXH
{
	ID3D11InputLayout* CreateInputlayout(ID3D11Device*  pd3dDevice, DWORD dwSize, LPCVOID lpData, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements);
	ID3D11Buffer* CreateVertexBuffer(ID3D11Device*  pd3dDevice, void *vertices, UINT iNumVertex, UINT iVertexSize, bool bDynamic = false);
	ID3D11Buffer* CreateIndexBuffer(ID3D11Device*  pd3dDevice, void *indices, UINT iNumIndex, UINT iSize, bool bDynamic = false);
	ID3D11Buffer* CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNum, UINT iSize, bool dynamic = false);

	HRESULT CreateComputeShader(LPCWSTR pSrcFile, LPCSTR pFunctionName, ID3D11Device* pDevice, ID3D11ComputeShader** ppShaderOut);
	HRESULT CreateStructuredBuffer(ID3D11Device* pd3dDevice, UINT uElementSize, UINT uCount, VOID* pInitData, ID3D11Buffer** ppBufOut);
	HRESULT CreateBufferSRV(ID3D11Device* pd3dDevice, ID3D11Buffer* buf, ID3D11ShaderResourceView** srv);
	HRESULT CreateTextureUAV(ID3D11Device* pd3dDevice, ID3D11Texture2D* pBuffer, ID3D11UnorderedAccessView** ppSRVOut);
	HRESULT CreateBufferUAV(ID3D11Device* pd3dDevice, ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppSRVOut);
	HRESULT RunComputeShader(ID3D11DeviceContext* pd3dContext, ID3D11ComputeShader* pComputeShader, UINT nNumViews, ID3D11ShaderResourceView** pShaderResourceView,
		ID3D11ShaderResourceView** m_pTextureResultSRV,
		ID3D11UnorderedAccessView* pUnorderedAccessView, UINT X, UINT Y, UINT Z);
}

class KG_DxHeper
{
public:
	KG_DxHeper();
	~KG_DxHeper();
};


