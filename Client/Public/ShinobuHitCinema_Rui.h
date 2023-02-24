#pragma once
#include "ShinobuState.h"


BEGIN(Client)
BEGIN(Shinobu)

class CHitCinema_Rui : public CShinobuState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6, SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_DMG_010 = 121,
		ANIM_SCENE_DMG_030 = 122,
		ANIM_SCENE_DMG_040 = 123,
		ANIM_SCENE_DMG_060 = 124,
		ANIM_SCENE_DMG_070 = 125,
		ANIM_SCENE_END,
	};
public:
	CHitCinema_Rui(CINEMASCENE eScene);
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