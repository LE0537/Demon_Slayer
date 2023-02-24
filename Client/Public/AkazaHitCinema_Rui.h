#pragma once
#include "AkazaState.h"


BEGIN(Client)
BEGIN(Akaza)

class CHitCinema_Rui : public CAkazaState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6, SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_DMG_010 = 104,
		ANIM_SCENE_DMG_030 = 105,
		ANIM_SCENE_DMG_040 = 106,
		ANIM_SCENE_DMG_060 = 107,
		ANIM_SCENE_DMG_070 = 108,
		ANIM_SCENE_END,
	};
public:
	CHitCinema_Rui(CINEMASCENE eScene);
	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;


private:
	CINEMASCENE m_eScene = SCENE_END;
	CCharacters::SKILL_TYPE m_eTargetSkill;
	_bool m_bAnimStop = false;


};

END
END