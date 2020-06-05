#pragma once
#include "KG_Camera.h"

class KG_ModelViewCamera :public KG_Camera
{
public:
	D3DXMATRIX m_mModelLastRot;
	D3DXMATRIX m_mModelRot;
public:
	virtual bool Frame() override;

public:
	void Update(D3DXVECTOR4 rotvalue);

public:
	KG_ModelViewCamera();
	virtual ~KG_ModelViewCamera();
};


