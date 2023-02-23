#pragma once

#include "KyoujuroState.h"

BEGIN(Client)
BEGIN(Kyoujuro)

class CKyoujuro_CinemaState : public CKyoujuroState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6, SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_START = 117,
		ANIM_SCENE_0 = 118,
		ANIM_SCENE_1 = 119,
		ANIM_SCENE_2 = 120,
		ANIM_SCENE_3 = 121,
		ANIM_SCENE_4 = 122,
		ANIM_SCENE_5 = 123,
		ANIM_SCENE_6 = 124,
		ANIM_SCENE_7 = 125,
		ANIM_SCENE_8 = 126,

		ANIM_SCENE_END = 100,
	};

public:
	CKyoujuro_CinemaState(CINEMASCENE eScene);
	

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;




private:
	CINEMASCENE m_eScene = SCENE_END;
	_bool		m_bAnimStop = false;
	_bool		m_bNextAnim = false;
	_bool		m_bIsHitMotion = false;
	_bool		m_bControlMotion = false;
	_bool		m_bMotionDelay = false;
	_vector		m_vMyPosition;
	_vector		m_vTargetPosition;
	_vector		m_vDirection;
	_float		m_fTime = 0.f;

	_bool       m_bOnGround = false;

	_float m_fPositionY;
	_float3 m_vPosition;
	_float3 m_vVelocity;
};

END
END