#include "KG_Time.h"

float g_gametime = 0;
int g_FPS = 0;
float g_SecondTime = 0;

bool KG_Time::Init()
{
	NowTime = timeGetTime();
	return true;
}

bool KG_Time::Frame()
{
	DWORD FrameInTime = timeGetTime();
	SecondTime = (float)(FrameInTime - NowTime) / 1000;
	g_gametime += SecondTime;
	g_SecondTime = SecondTime;
	static int fps;
	TotalFrameTime += SecondTime;

	if (TotalFrameTime >= 1.0f)
	{
		CFPS = fps;
		fps = 0;
		TotalFrameTime -= 1.0f;
	}
	g_FPS = CFPS;
	fps++;
	//= GameTime;
	NowTime = timeGetTime();
	return true;
}

bool KG_Time::Render()
{
	return true;
}

bool KG_Time::Release()
{
	return true;
}


KG_Time::KG_Time()
{
	GameTime = 0.0f;
}


KG_Time::~KG_Time()
{
}

