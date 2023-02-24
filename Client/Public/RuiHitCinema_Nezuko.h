#pragma once

#include "RuiState.h"

BEGIN(Client)
BEGIN(Rui)

class CHitCinema_Nezuko : public CRuiState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6,SCENE_7,  SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_DMG_010 = 111,
		ANIM_SCENE_DMG_030 = 112,
		ANIM_SCENE_DMG_040 = 113,
		ANIM_SCENE_DMG_050 = 114,
		ANIM_SCENE_DMG_060 = 115,
		ANIM_SCENE_DMG_080 = 116,
		ANIM_SCENE_DMG_100 = 117,
		ANIM_SCENE_DMG_110 = 118,
		ANIM_SCENE_END,
	};
public:
	CHitCinema_Nezuko(CINEMASCENE eScene);
	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;


private:
	CINEMASCENE m_eScene = SCENE_END;
	CCharacters::SKILL_TYPE m_eTargetSkill;
	_bool m_bAnimStop = false;
};

END
END