#pragma once
#include "KG_Camera.h"
#include "KG_ShapeObject.h"
#include "KG_DxRT.h"


class KG_Minimap :public KG_Camera
{
public:
	std::shared_ptr<KG_ShapeObject> m_pMiniMap;
	std::shared_ptr<KG_DxRT> m_RT;
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_Context;
	D3DXMATRIX	m_matTopView;
	D3DXMATRIX	m_matTopProj;


public:
	HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);

public:
	bool Render();
	bool Release();

public:
	KG_Minimap();
	virtual ~KG_Minimap();
};


