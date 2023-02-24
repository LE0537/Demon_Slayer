#pragma once
#include "RuiState.h"

BEGIN(Client)
BEGIN(Rui)

class CHitCinema_Kyoujuro : public CRuiState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6, SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_DMG_010 = 122,
		ANIM_SCENE_DMG_060 = 123,
		ANIM_SCENE_DMG_070 = 124,
		ANIM_SCENE_DMG_075 = 125,
		ANIM_SCENE_END,
	};
public:
	CHitCinema_Kyoujuro(CINEMASCENE eScene);
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