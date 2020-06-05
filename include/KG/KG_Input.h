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
	//static int num;						//프레임 계산
	int num;

public:

	DWORD KeyData[256];
	RECT rt{ 400,300,800,600 };
	float X;
	float Y;
	bool weaponbool = false;		//무기 발사 여부
	POINT m_Mouse;


public:
	bool GetKeyCheck(DWORD inkey);
	bool GetKeyCheckDouble(DWORD inkey, float CheckTime);
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	DWORD KeyCheck(DWORD inKey);			//키 입력시 그 키의 정보를 입력 받으며 FREE(이전에 눌리지않고 지금도 눌리지 않는다), UP(이전에 눌려있고 지금 떼어졌다)
											//HOLD(이전에 눌렸고 지금도 눌렸다),PUSH(이전에 눌리지 않고 지금은 눌렸다) 4가지의 키 정보를 넘기는 함수

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


