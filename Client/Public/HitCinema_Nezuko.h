#pragma once

#include "ShinobuState.h"

BEGIN(Client)
BEGIN(Shinobu)

class CHitCinema_Nezuko : public CShinobuState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6,SCENE_7,  SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_DMG_010 = 113,
		ANIM_SCENE_DMG_030 = 114,
		ANIM_SCENE_DMG_040 = 115,
		ANIM_SCENE_DMG_050 = 116,
		ANIM_SCENE_DMG_060 = 117,
		ANIM_SCENE_DMG_080 = 118,
		ANIM_SCENE_DMG_100 = 119,
		ANIM_SCENE_DMG_110 = 120,
		ANIM_SCENE_END,
	};
public:
	CHitCinema_Nezuko(CINEMASCENE eScene);
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