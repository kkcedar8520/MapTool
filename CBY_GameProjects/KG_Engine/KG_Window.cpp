#include "KG_Window.h"

HWND g_hwnd;
HINSTANCE g_hInstance;
RECT Winrt;
KG_Window* g_KGWindow = nullptr;

LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)				//���� �Լ��� �����Ͽ� WndProc�����ϴ� ������� �� ���̺귯���� �ҷ����� �ٸ�������
{																									//���ϴ� ������� �޼����� ó���� �� �ְ� �ϱ� ���� �̷��� ���δ�.
	return g_KGWindow->WndProc(hWnd, message, wParam, lParam);
}

LRESULT KG_Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		if (SIZE_MINIMIZED != wParam)
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			GetClientRect(g_hwnd, &Winrt);
			g_KGWindow->ResizeDevice(width, height);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool KG_Window::ResizeDevice(UINT width, UINT height)
{
	return true;
}

bool KG_Window::InitWindow(const TCHAR* WindowClass, const TCHAR* WindowName)
{
	wn.cbSize = sizeof(WNDCLASSEX);				//������ ũ��(������ Ȯ������ ������̹Ƿ� �� ũ�⸸ŭ �����ָ� �ȴ� ���� ����)
	wn.style = CS_HREDRAW | CS_VREDRAW;				//������ ���� ��Ÿ��(���� �Ʒ��� �ѷ��ش� ���ߴ�)
	wn.lpfnWndProc = &StaticWndProc;			//�����찡 �����ǰ� �̺�Ʈ �߻��� �޼����� ������ ó���� �� ����
	wn.cbClsExtra = 0;
	wn.cbWndExtra = 0;
	wn.hInstance = hInstance;					//������ �ν��Ͻ� ���� �ν��Ͻ��� Ŭ���� ������ �޴´�
	wn.hIcon = NULL;								//������ ������, NULL������ ����Ʈ������ ������ �������� �������ɷ� �޴´�
	wn.hCursor = LoadCursor(NULL, IDC_ARROW);							//������ ���콺 Ŀ��, �����ܰ� ����
	wn.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//������ ����										//������, Ŀ��, ��� ���� ���μ��� ���忡���� ���� �ϳ��ϳ�
	wn.lpszMenuName = NULL;																			//������� ���⿡ �ڵ��� �ʿ��ϴ�.
	wn.lpszClassName = WindowClass;				//������ Ŭ���� �̸�

	g_hInstance = hInstance;					//�����쿡 �ο��� �ν��Ͻ��� �������� �ΰ� ����� Ŭ�����鿡�� �̿�

	if (!RegisterClassEx(&wn))					//RegisterClassEx�Լ��� ���� ������ ������ ����ü�� ����ϴ� �Լ��̴�
	{
		return false;
	}
	Winrt = { 0,0,1280,720 };					//����������� ���� Ŭ���̾�Ʈ ũ�⸦ �����ϴ� RECT�̴�, �ٸ� ���� ������ ��ü�� ũ�⸦ �Է��ѻ��¶� �̴�� �Է½� Ŭ���̾�Ʈȭ���� ���������� �۰Եȴ�.
	AdjustWindowRect(&Winrt, WS_OVERLAPPEDWINDOW, FALSE);		//Winrt�� ������ �޾� �� ũ�⸦ ������ ��Ÿ�ϰ� �޴� ���η� �Ǵ��ϰ� Ŭ���̾�Ʈ ������ Winrt�� ���ϴ� ��ŭ ������ֱ����� �۾����ִ� �Լ�
	m_Hwnd = CreateWindowExW(WS_EX_APPWINDOW, WindowClass, WindowName, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 0, 0,
		Winrt.right - Winrt.left, Winrt.bottom - Winrt.top, NULL, NULL, hInstance, NULL);				//�۾��� Winrt�� ũ�� ������ �ް� �����츦 �����Ͽ� �ڵ鿡 �Է�
	GetClientRect(m_Hwnd, &Winrt);																		//������ �������� ũ�⸦ �ڵ��� ���� Winrt�� �Է¹޴´�.
	g_hwnd = m_Hwnd;																					//������ ������ �ڵ��� �ٸ� Ŭ������ �Բ� �������� �������� �޴���
	ShowWindow(m_Hwnd, SW_SHOW);																		//������� �����Ǿ��ٰ� �ƴϱ⿡ �����츦 ȣ���ؼ� ȭ�鿡 ���� �Լ�.
	return true;
}

KG_Window::KG_Window()
{
	g_KGWindow = this;																					//�����쿡 ����� �̺�Ʈ�� �����ϰ� �޼����� ó���ϱ� ���� WndProc�Լ��� ���̴µ�
																										//�� ���̺귯���� �پ��Ѱ����� �������� WndProc�� �����Լ��� ����Ͽ� ������������
																										//���� ������ �ϳ� �����Ͽ� �� Ŭ������ ����Ű�� �ϰ� ��� �����Լ��� �ҷ�����
																										//�޼��� ó���� ���ϴ� ������� ó���� �� �ְ� ���´�.
}


KG_Window::~KG_Window()
{
}
