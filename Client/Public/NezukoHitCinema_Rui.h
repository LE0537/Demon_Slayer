#pragma once
#include "NezukoState.h"


BEGIN(Client)
BEGIN(Nezuko)

class CHitCinema_Rui : public CNezukoState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6, SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_DMG_010 = 132,
		ANIM_SCENE_DMG_030 = 133,
		ANIM_SCENE_DMG_040 = 134,
		ANIM_SCENE_DMG_060 = 135,
		ANIM_SCENE_DMG_070 = 136,
		ANIM_SCENE_END,
	};
public:
	CHitCinema_Rui(CINEMASCENE eScene);
	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;


private:
	CINEMASCENE m_eScene = SCENE_END;
	CCharacters::SKILL_TYPE m_eTargetSkill;
	_bool m_bAnimStop = false;


};

END
END