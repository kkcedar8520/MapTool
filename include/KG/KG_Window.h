#pragma once
#include "KG_Std.h"
#include "KG_Math.h"

class KG_Window
{
public:
	WNDCLASSEX wn;				//윈도우 클래스 확장버전을 이용할 변수
	HWND m_Hwnd;				//생성된 윈도우의 핸들(현재 한개의 윈도우만 이용하므로 전체에서 사용될 한개의 핸들만 선언)
	HINSTANCE hInstance;		//윈도우에 사용될 인스턴스 윈도우의 고유넘버
	//RECT m_ClientRt;

public:
	bool InitWindow(const TCHAR* WindowClass, const TCHAR* WindowName);
	virtual bool ResizeDevice(UINT width, UINT height);
	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND m_hEdit;
	HWND m_hButton;

public:
	KG_Window();
	virtual ~KG_Window();
};
