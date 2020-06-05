#pragma once
#include "KG_Camera.h"
class KG_DebugCamera :public KG_Camera
{
public:
	bool  m_bDrag;
	POINT m_LastPos;

public:
	virtual bool Frame() override;
	virtual void MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

public:
	void Update(D3DXVECTOR4 rotvalue);

public:
	KG_DebugCamera();
	~KG_DebugCamera();
};

