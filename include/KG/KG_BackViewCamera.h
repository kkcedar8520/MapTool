#pragma once
#include "KG_Camera.h"
class KG_BackViewCamera :public KG_Camera
{
public:
	virtual bool Frame() override;
	//void Side(float fValue);
	//void Front(float fValue);

public:
	void Update(D3DXVECTOR4 rotvalue);
	void Front(float Speed);
	void Side(float Speed);
	KG_BackViewCamera();
	virtual ~KG_BackViewCamera();
};

