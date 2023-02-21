#pragma once
#include "ShinobuState.h"


BEGIN(Client)
BEGIN(Shinobu)


class CShinobu_CinemaState : public CShinobuState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6, SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_START = 30,
		ANIM_SCENE_0_START = 14,
		ANIM_SCENE_0_LOOP = 15,
		ANIM_SCENE_0_END = 16,

	};
public:
	CShinobu_CinemaState(CINEMASCENE eScene);
	
	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;

	CShinobuState* Scene_Start(CShinobu* pShinobu, _float fTimeDelta);
	CShinobuState* Scene_0(CShinobu* pShinobu, _float fTimeDelta);


	void Increase_Height(CShinobu* pShinobu, _float fTimeDelta);
	void Decrease_Height(CShinobu* pShinobu, _float fTimeDelta);
	void Initialize_Value(CShinobu* pShinobu, _float fTimeDelta);

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