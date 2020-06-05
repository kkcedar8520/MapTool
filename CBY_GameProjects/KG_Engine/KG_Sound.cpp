#include "KG_Sound.h"

bool KG_Sound::Load(FMOD::System*  pSystem, const TCHAR* SoundFile)										//사운드 파일 인덱스 번호 지정하여 받기,인자로 받는 bool값은 플레이 여부
{
	std::string loadfile = wtm(SoundFile);
	if (pSystem->createSound(loadfile.c_str(), FMOD_DEFAULT, nullptr, &m_pSound) != FMOD_OK)
	{
		return false;
	}

	//pSystem->playSound(m_pSound, nullptr, false, &m_pChannel);
	return true;
}

bool KG_Sound::GetPlay()
{
	m_bPlay = false;
	if (m_pChannel != nullptr)
	{
		m_pChannel->isPlaying(&m_bPlay);
	}
	return m_bPlay;
}

bool KG_Sound::Play(FMOD::System*  pSystem)
{
	bool PlayCheck = false;
	if (m_pChannel != nullptr)
	{
		m_pChannel->isPlaying(&PlayCheck);
	}

	if (PlayCheck == false)
	{
		pSystem->playSound(m_pSound, nullptr, false, &m_pChannel);
		m_pChannel->setVolume(fVolume);
	}

	return true;
}

bool KG_Sound::EffectPlay(FMOD::System*  pSystem)
{
	FMOD::Channel* pChannel;
	pSystem->playSound(m_pSound, nullptr, false, &pChannel);
	m_pChannel = pChannel;
	m_pChannel->setVolume(fVolume);
	return true;
}

bool KG_Sound::Pause()
{
	bool play = false;
	bool pause = true;
	if (m_pChannel != nullptr)
	{
		m_pChannel->isPlaying(&play);
		if (play)
		{
			//m_pChannel->getPaused(&pause);
			m_pChannel->setPaused(&pause);
		}
	}
	return true;
}

void KG_Sound::Stop()
{
	if (m_pChannel != nullptr)
	{
		m_pChannel->stop();
	}
}

bool KG_Sound::Volume(float ChangeVolume, bool UP)
{
	float NowVolume;
	m_pChannel->getVolume(&NowVolume);
	if (m_pChannel == nullptr)
	{
		return false;
	}
	if (UP)
	{
		NowVolume += ChangeVolume;
		NowVolume = min(10.0f, NowVolume);
	}
	else
	{
		NowVolume -= ChangeVolume;
		NowVolume = max(0.0f, NowVolume);
	}
	m_pChannel->setVolume(NowVolume);

	return true;
}

bool KG_Sound::Init()
{
	return true;
}

bool KG_Sound::Frame()
{
	return true;
}

bool KG_Sound::Render()
{
	return true;
}

bool KG_Sound::Release()
{
	m_pSound->release();
	return true;
}


KG_Sound::KG_Sound()
{
	m_SoundList = 0;
}


KG_Sound::~KG_Sound()
{
}
