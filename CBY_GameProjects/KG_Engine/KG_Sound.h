#pragma once
#include "KG_Std.h"
#include "fmod.h"
#include "fmod.hpp"
//#define MAX_SOUNDLIST 32

class KG_Sound
{
public:
	std::wstring  m_szName;
	FMOD::Sound* m_pSound;
	FMOD::Channel* m_pChannel;
	int m_SoundList;
	float fVolume;
	bool m_bPlay;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	bool Load(FMOD::System*  pSystem, const TCHAR* SoundFile);
	bool Play(FMOD::System*  pSystem);
	bool EffectPlay(FMOD::System*  pSystem);
	bool Pause();
	bool Volume(float ChangeVolume, bool UP);
	bool GetPlay();
	void Stop();

public:
	KG_Sound();
	~KG_Sound();
};


