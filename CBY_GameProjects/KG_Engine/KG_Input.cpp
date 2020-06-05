#include "KG_Input.h"

DWORD KG_Input::KeyCheck(DWORD inKey)
{
	SHORT key = GetAsyncKeyState(inKey);		//GetAsyncKeyState는 키값을 받아 그 키가 눌렸는지 이전에 눌렸는지 판단해준다
												//눌릴경우 최상위 비트를 1로 이전에 눌릴 경우 최하위 비트를 1로하며 반환값은 short이므로 2바이트이고
												//16진수는 2진수 4자리가 한자리이므로 눌릴때 이진수 1000 0000 0000 0000이니 16진수 0x8000이되고 이전에 눌릴때 0x0001인것

	if (key & 0x8000)							//키가 눌렸을때 0x8000이고 이것을 비트연산 논리곱을 쓸 경우 같을때 참의 값을 반환한다.
	{
		if (KeyData[inKey] == KEY_FREE || KeyData[inKey] == KEY_UP)		//키가 이전에 안눌렸거나 떼어지는 상황이면 키가 눌리는 상황
		{
			KeyData[inKey] = KEY_PUSH;
		}
		else
			KeyData[inKey] = KEY_HOLD;										//그 외에 키가 눌러져 있는 상태이면 홀드인 상태
	}
	else
	{
		if (KeyData[inKey] == KEY_PUSH || KeyData[inKey] == KEY_HOLD)
		{
			KeyData[inKey] = KEY_UP;
		}

		KeyData[inKey] = KEY_FREE;
	}
	return KeyData[inKey];
}


bool KG_Input::GetKeyCheck(DWORD inkey)
{
	if (KeyCheck(inkey))																//방향키로 캐릭터의 이동을 정의 계산하며 사양의 차이에 불공정함이 생기지 않게 시간의 동기화를 이용
	{
		if (KeyData[inkey] == KEY_PUSH || KeyData[inkey] == KEY_HOLD)					//enum { UP=0, DOWN, LEFT, RIGHT };
		{
			return true;
		}
	}
	return false;

}

bool KG_Input::GetKeyCheckDouble(DWORD inkey, float CheckTime)
{
	if (KeyCheck(inkey))																//방향키로 캐릭터의 이동을 정의 계산하며 사양의 차이에 불공정함이 생기지 않게 시간의 동기화를 이용
	{
		if (KeyData[inkey] == KEY_PUSH)					//enum { UP=0, DOWN, LEFT, RIGHT };
		{
			if (CheckTime > 0.0f)
			{
				return true;
			}
		}
	}
	return false;

}

bool KG_Input::Init()
{
	ZeroMemory(&KeyData, sizeof(SHORT) * 256);											//키보드가 입력될 변수를 메모리 0으로 초기화
	return true;
}
bool KG_Input::Frame()
{
	GetCursorPos(&m_Mouse);
	ScreenToClient(g_hwnd, &m_Mouse);
	return true;
}
bool KG_Input::Render()
{
	return true;
}
bool KG_Input::Release()
{
	return true;
}

KG_Input::KG_Input()
{
	num = 0;
}


KG_Input::~KG_Input()
{
}
