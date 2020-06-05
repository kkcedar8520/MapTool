#pragma once
#include "KG_Std.h"

enum { KEY_FREE, KEY_UP, KEY_HOLD, KEY_PUSH };
enum
{
	NONE = 0,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	Z,
	X,
	G,
	SPACE,
	KEYCOUNT
};

class KG_Input
{
private:
	//static int num;						//������ ���
	int num;

public:

	DWORD KeyData[256];
	RECT rt{ 400,300,800,600 };
	float X;
	float Y;
	bool weaponbool = false;		//���� �߻� ����
	POINT m_Mouse;


public:
	bool GetKeyCheck(DWORD inkey);
	bool GetKeyCheckDouble(DWORD inkey, float CheckTime);
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	DWORD KeyCheck(DWORD inKey);			//Ű �Է½� �� Ű�� ������ �Է� ������ FREE(������ �������ʰ� ���ݵ� ������ �ʴ´�), UP(������ �����ְ� ���� ��������)
											//HOLD(������ ���Ȱ� ���ݵ� ���ȴ�),PUSH(������ ������ �ʰ� ������ ���ȴ�) 4������ Ű ������ �ѱ�� �Լ�

public:
	static KG_Input& Get()
	{
		static KG_Input mgr;
		return mgr;
	}
public:
	KG_Input();
	~KG_Input();
};

#define I_Input KG_Input::Get()


