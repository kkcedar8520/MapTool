#pragma once
#include "KG_Device.h"
#include "KG_Camera.h"
#include "KG_Model.h"
#include "KG_Input.h"
#include "KG_DXWrite.h"
#include "KG_ShapeLine.h"
#include "KG_DebugCamera.h"
#include "KG_Select.h"


class KG_Core :public KG_Device
{
protected:
	bool GetGameRun;
	KG_Time	m_Timer;
	std::shared_ptr<KG_DXWrite> m_Font;
	std::shared_ptr<KG_ShapeLine> m_DebugLine;
	KG_Camera m_DebugCamera;
	KG_Camera* m_pMainCamera;
	bool bDebug;
	//KG_Input m_Input;

public:
	KG_Select m_Select;

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	void Debug();
	bool CoreInit();
	bool CoreFrame();
	bool PreRender();
	bool CoreRender();
	bool PostRender();
	bool CoreRelease();
	bool GameRun();
	virtual HRESULT CreateDXrc();		//��������� CDXH�ҽ��� ������ ����
	virtual HRESULT	DeleteDXrc();		//��������� CDXH�ҽ� ������ ���� ����
	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void ClearContext();

public:
	KG_Core();
	virtual ~KG_Core();
};


