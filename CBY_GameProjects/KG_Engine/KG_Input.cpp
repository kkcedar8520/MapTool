#include "KG_Input.h"

DWORD KG_Input::KeyCheck(DWORD inKey)
{
	SHORT key = GetAsyncKeyState(inKey);		//GetAsyncKeyState�� Ű���� �޾� �� Ű�� ���ȴ��� ������ ���ȴ��� �Ǵ����ش�
												//������� �ֻ��� ��Ʈ�� 1�� ������ ���� ��� ������ ��Ʈ�� 1���ϸ� ��ȯ���� short�̹Ƿ� 2����Ʈ�̰�
												//16������ 2���� 4�ڸ��� ���ڸ��̹Ƿ� ������ ������ 1000 0000 0000 0000�̴� 16���� 0x8000�̵ǰ� ������ ������ 0x0001�ΰ�

	if (key & 0x8000)							//Ű�� �������� 0x8000�̰� �̰��� ��Ʈ���� ������ �� ��� ������ ���� ���� ��ȯ�Ѵ�.
	{
		if (KeyData[inKey] == KEY_FREE || KeyData[inKey] == KEY_UP)		//Ű�� ������ �ȴ��Ȱų� �������� ��Ȳ�̸� Ű�� ������ ��Ȳ
		{
			KeyData[inKey] = KEY_PUSH;
		}
		else
			KeyData[inKey] = KEY_HOLD;										//�� �ܿ� Ű�� ������ �ִ� �����̸� Ȧ���� ����
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
	if (KeyCheck(inkey))																//����Ű�� ĳ������ �̵��� ���� ����ϸ� ����� ���̿� �Ұ������� ������ �ʰ� �ð��� ����ȭ�� �̿�
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
	if (KeyCheck(inkey))																//����Ű�� ĳ������ �̵��� ���� ����ϸ� ����� ���̿� �Ұ������� ������ �ʰ� �ð��� ����ȭ�� �̿�
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
	ZeroMemory(&KeyData, sizeof(SHORT) * 256);											//Ű���尡 �Էµ� ������ �޸� 0���� �ʱ�ȭ
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
