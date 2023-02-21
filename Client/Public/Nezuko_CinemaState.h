#pragma once

#include "NezukoState.h"


BEGIN(Client)
BEGIN(Nezuko)


class CNezuko_CinemaState : public CNezukoState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6, SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_START = 133,
		ANIM_SCENE_0 = 106,
		ANIM_SCENE_1 = 104,
		ANIM_SCENE_2 = 104,
		ANIM_SCENE_3 = 103,
		ANIM_SCENE_4 = 111,
		ANIM_SCENE_5 = 112,
		ANIM_SECNE_6 = 113,
	};



public:
	CNezuko_CinemaState(CINEMASCENE eScene);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;


private:
	CINEMASCENE m_eScene = SCENE_END;
	_bool		m_bAnimStop = false;
	_bool		m_bNextAnim = false;
	_bool		m_bIsHitMotion = false;
	_bool		m_bControlMotion = false;
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