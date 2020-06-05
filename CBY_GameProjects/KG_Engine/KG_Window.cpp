#include "KG_Window.h"

HWND g_hwnd;
HINSTANCE g_hInstance;
RECT Winrt;
KG_Window* g_KGWindow = nullptr;

LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)				//전역 함수로 선언하여 WndProc전달하는 방식으로 이 라이브러리를 불렀을때 다른곳에서
{																									//원하는 방식으로 메세지를 처리할 수 있게 하기 위해 이렇게 쓰인다.
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
	wn.cbSize = sizeof(WNDCLASSEX);				//윈도우 크기(윈도우 확장으로 만들것이므로 그 크기만큼 정해주면 된다 거의 고정)
	wn.style = CS_HREDRAW | CS_VREDRAW;				//윈도우 생성 스타일(위와 아래로 뿌려준다 정했다)
	wn.lpfnWndProc = &StaticWndProc;			//윈도우가 생성되고 이벤트 발생시 메세지를 보내서 처리할 곳 지정
	wn.cbClsExtra = 0;
	wn.cbWndExtra = 0;
	wn.hInstance = hInstance;					//윈도우 인스턴스 지정 인스턴스는 클래스 생성시 받는다
	wn.hIcon = NULL;								//윈도우 아이콘, NULL지정시 디폴트값으로 지정된 윈도우의 정해진걸로 받는다
	wn.hCursor = LoadCursor(NULL, IDC_ARROW);							//윈도우 마우스 커서, 아이콘과 같다
	wn.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//윈도우 배경색										//아이콘, 커서, 배경 전부 프로세서 입장에서는 전부 하나하나
	wn.lpszMenuName = NULL;																			//윈도우로 보기에 핸들이 필요하다.
	wn.lpszClassName = WindowClass;				//윈도우 클래스 이름

	g_hInstance = hInstance;					//윈도우에 부여된 인스턴스를 전역으로 두고 연결된 클래스들에서 이용

	if (!RegisterClassEx(&wn))					//RegisterClassEx함수는 위에 설정된 윈도우 구조체를 등록하는 함수이다
	{
		return false;
	}
	Winrt = { 0,0,1280,720 };					//윈도우생성을 위해 클라이언트 크기를 지정하는 RECT이다, 다만 아직 윈도우 전체의 크기를 입력한상태라 이대로 입력시 클라이언트화면은 지정값보다 작게된다.
	AdjustWindowRect(&Winrt, WS_OVERLAPPEDWINDOW, FALSE);		//Winrt의 정보를 받아 그 크기를 윈도우 스타일과 메뉴 여부로 판단하고 클라이언트 영역을 Winrt가 원하는 만큼 만들어주기위해 작업해주는 함수
	m_Hwnd = CreateWindowExW(WS_EX_APPWINDOW, WindowClass, WindowName, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 0, 0,
		Winrt.right - Winrt.left, Winrt.bottom - Winrt.top, NULL, NULL, hInstance, NULL);				//작업된 Winrt의 크기 정보를 받고 윈도우를 생성하여 핸들에 입력
	GetClientRect(m_Hwnd, &Winrt);																		//생성된 윈도우의 크기를 핸들을 통해 Winrt에 입력받는다.
	g_hwnd = m_Hwnd;																					//생성된 윈도우 핸들을 다른 클래스와 함께 쓰기위해 전역으로 받는중
	ShowWindow(m_Hwnd, SW_SHOW);																		//윈도우는 생성되었다고 아니기에 윈도우를 호출해서 화면에 띄우는 함수.
	return true;
}

KG_Window::KG_Window()
{
	g_KGWindow = this;																					//윈도우에 생기는 이벤트를 전달하고 메세지를 처리하기 위해 WndProc함수가 쓰이는데
																										//이 라이브러리를 다양한곳에서 쓰기위해 WndProc를 가상함수로 등록하여 쓰기위함으로
																										//전역 변수를 하나 선언하여 이 클래스를 가리키게 하고 어느 메인함수가 불러내든
																										//메세지 처리를 원하는 방식으로 처리할 수 있게 돕는다.
}


KG_Window::~KG_Window()
{
}
