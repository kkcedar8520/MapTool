#pragma once
#include "KG_DxRT.h"

class KG_Shadow
{
private:
	D3DXMATRIX m_matLTM;
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXMATRIX m_matTexture;


public:
	DXGI_FORMAT m_DSFormat;
	ID3D11VertexShader* m_pVS;					//¡§¿˚ Ω¶¿Ã¥ı
	ID3D11PixelShader* m_pPS;					//«»ºø Ω¶¿Ã¥ı
	ID3DBlob* m_pVertexCode;
	ID3DBlob* m_pPixelCode;
	std::shared_ptr<KG_DxRT> m_Rt;

public:
	virtual HRESULT Create(ID3D11Device* pd3dDevice, float fWidth = 1024, float fHeight = 1024, const TCHAR* ShaderFileName = L"../../data/shader/ShadowShader.txt",
		const CHAR* VSName = "VSDS", const CHAR* PSName = "PSDS");
	virtual bool Begin(ID3D11DeviceContext* pContext, D3DXVECTOR4 vColor = { 0, 0, 0, 1 });
	virtual void ShaderSet(ID3D11DeviceContext* pContext);
	virtual bool End(ID3D11DeviceContext* pContext);
	virtual bool Release();
	//void SetDSFormat(DXGI_FORMAT Format);

public:
	HRESULT LoadShader(ID3D11Device* pd3dDevice, const TCHAR* ShaderFileName, const CHAR* VSName, const CHAR* PSName);
	//D3DXMATRIX SetWorld();
	//D3DXMATRIX SetView();
	//D3DXMATRIX SetProj();
	void SetLTM(D3DXMATRIX* world, D3DXVECTOR3 vPos, D3DXVECTOR3 vAt, float fDistance, float fFar = 100.0f);
	void GetLTM(D3DXMATRIX& world, D3DXMATRIX& View, D3DXMATRIX& Proj, D3DXMATRIX& LTM);
	//ID3D11ShaderResourceView GetDSV();

public:
	KG_Shadow();
	virtual ~KG_Shadow();
};


