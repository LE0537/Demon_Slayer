#pragma once
#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CSoundMgr
{
public:
	static CSoundMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	CSoundMgr();
	virtual~CSoundMgr();

public:
	void Initialize();


public:
	void PlayBGM(TCHAR* pSoundKey, float fVolume);
	void PlayEffect(TCHAR * pSoundKey, float fVolume);
	void PlayVoice(TCHAR * pSoundKey, float fVolume);
	void PlayDialog(TCHAR * pSoundKey, float fVolume);
	void BGM_Stop();
	void Effect_Stop(CHANNELID eType);
	void Free();
	void SetSoundVolume(CHANNELID  _Channel, float fVolume);
	void Mute_BGM();
	void Mute_Effect();
	void Mute_Voice();
	void Mute_Dialog();
	void Listen_BGM();
	void Listen_Effect();
	void Listen_Voice();
	void Listen_Dialog();
	_bool Dialog_End(_bool* bCheck);
private:
	void LoadBGMFile();
	void LoadEffectFile();
	void LoadVoiceFile();
	void LoadDialogFile();
private:
	static CSoundMgr* m_pInstance;

	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD::Sound*> m_mapSound;


	FMOD::System*    m_System = nullptr;
	FMOD::Channel*   m_Channel[MAXCHANNEL];
	FMOD_RESULT       Result;
	unsigned int      Version;
	void*             extradriverdata = nullptr;
};

END