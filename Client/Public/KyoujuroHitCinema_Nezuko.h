#pragma once

#include "KyoujuroState.h"

BEGIN(Client)
BEGIN(Kyoujuro)

class CHitCinema_Nezuko : public CKyoujuroState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6,SCENE_7,  SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_DMG_010 = 106,
		ANIM_SCENE_DMG_030 = 107,
		ANIM_SCENE_DMG_040 = 108,
		ANIM_SCENE_DMG_050 = 109,
		ANIM_SCENE_DMG_060 = 110,
		ANIM_SCENE_DMG_080 = 111,
		ANIM_SCENE_DMG_100 = 112,
		ANIM_SCENE_DMG_110 = 113,
		ANIM_SCENE_END,
	};
public:
	CHitCinema_Nezuko(CINEMASCENE eScene);
	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;


private:
	CINEMASCENE m_eScene = SCENE_END;
	CCharacters::SKILL_TYPE m_eTargetSkill;
	_bool m_bAnimStop = false;
};

END
END