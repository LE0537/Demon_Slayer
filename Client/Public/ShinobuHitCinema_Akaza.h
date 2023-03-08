#pragma once

#include "ShinobuState.h"

BEGIN(Client)
BEGIN(Shinobu)

class CHitCinema_Akaza : public CShinobuState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6, SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_DMG_010 = 126,
		ANIM_SCENE_DMG_050 = 127,
		ANIM_SCENE_DMG_080 = 128,
		ANIM_SCENE_DMG_090 = 129,
		ANIM_SCENE_END,
	};

public:
	CHitCinema_Akaza(CINEMASCENE eScene);
	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;


private:
	CINEMASCENE m_eScene = SCENE_END;
	CCharacters::SKILL_TYPE m_eTargetSkill;
	_bool m_bAnimStop = false;
};

END
END