#include "KG_SoundMgr.h"

int KG_SoundMgr::Load(const TCHAR* filename, bool play, float fvol)
{
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_tsplitpath_s(filename, Drive, Dir, name, ext);

	std::wstring nameExt = name;
	nameExt += ext;

	std::map<int, KG_Sound*>::iterator iter;
	for (iter = list.begin(); iter != list.end(); iter++)
	{
		if (iter->second->m_szName == nameExt)
		{
			return iter->first;
		}
	}

	std::wstring loadfile = Dir;
	loadfile = m_szPath + filename;

	KG_Sound* pData = new KG_Sound;
	pData->m_szName = nameExt;
	pData->fVolume = fvol;

	pData->Init();
	if (pData->Load(m_pSystem, loadfile.c_str()) >= 0)
	{
		list.insert(std::make_pair(++m_iIndex, pData));
		//pData->m_iIndex = m_iIndex;
		return m_iIndex;
	}
	delete pData;
	return m_iIndex;
}

bool KG_SoundMgr::Init()
{
	FMOD::System_Create(&m_pSystem);
	FMOD_RESULT ret = m_pSystem->init(g_iMaxSound, FMOD_INIT_NORMAL, 0);
	if (ret != FMOD_OK)
	{
		return false;
	}
	return true;
}

bool KG_SoundMgr::Frame()
{
	m_pSystem->update();

	return true;
}

bool KG_SoundMgr::Release()
{
	for (int i = 0; i < SoundChannel; i++)
	{
		KG_Sound* temp = list[i];
		temp->Release();
		delete temp;
	}
	list.clear();
	return true;
}

KG_SoundMgr::KG_SoundMgr()
{
	SoundChannel = -1;
	m_iIndex = 0;
	m_szPath = L"../../data/sound/";
}


KG_SoundMgr::~KG_SoundMgr()
{
	Release();
}

