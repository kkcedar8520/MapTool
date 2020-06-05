#pragma once
#include <map>
#include "KG_Sound.h"

const int g_iMaxSound = 32;

class KG_SoundMgr
{
public:
	std::map<int, KG_Sound*> list;
	std::wstring	m_szPath;
	int				m_iIndex;
	FMOD::System*  m_pSystem;
	HFONT   m_hDefaultFont;

private:
	int SoundChannel;

public:

	bool Release();
	static KG_SoundMgr& Get()
	{
		static KG_SoundMgr mgr;
		return mgr;
	}
	int Load(const TCHAR* filename, bool play, float fvol);
	bool Init();
	bool Frame();

private:
	KG_SoundMgr();
public:
	~KG_SoundMgr();
};

#define I_SOUND KG_SoundMgr::Get()

