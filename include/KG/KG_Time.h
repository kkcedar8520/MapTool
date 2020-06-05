#pragma once
#include "KG_Std.h"

class KG_Time
{
public:
	int CFPS = 0;
	float SecondTime;
	DWORD NowTime;		//DWORD->unsigned long¿Ã¥Ÿ
	float GameTime = 0;
	float TotalFrameTime = 0.0f;
	TCHAR Buffer[256];

public:
	bool Init();
	bool Render();
	bool Frame();
	bool Release();

public:
	KG_Time();
	~KG_Time();
};


