#pragma once
#include "TanjiroState.h"


BEGIN(Client)
BEGIN(Tanjiro)

class HitCinema_Rui : public CTanjiroState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6, SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_DMG_010 = 154,
		ANIM_SCENE_DMG_030 = 155,
		ANIM_SCENE_DMG_040 = 156,
		ANIM_SCENE_DMG_060 = 157,
		ANIM_SCENE_DMG_070 = 158,
		ANIM_SCENE_END,
	};
public:
	HitCinema_Rui(CINEMASCENE eScene);
	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;


private:
	CINEMASCENE m_eScene = SCENE_END;
	CCharacters::SKILL_TYPE m_eTargetSkill;
	_bool m_bAnimStop = false;


};

END
END