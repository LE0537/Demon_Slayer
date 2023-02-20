#pragma once
#include "RuiState.h"

BEGIN(Client)
BEGIN(Rui)


class CRui_CinemaState : public CRuiState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6, SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_START = 41,
		ANIM_SCENE_0 = 35,
		ANIM_SCENE_END,
	};


public:
	CRui_CinemaState(CINEMASCENE eScene);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;

	void Increase_Height(CRui* pRui, _float fTimeDelta);
	void Decrease_Height(CRui* pRui, _float fTimeDelta);
	void Gravity_Field(CRui* pRui, _float fTimeDelta);

	CRuiState* Scene_Start(CRui* pRui, _float fTimeDelta);
	CRuiState* Scene_0(CRui* pRui, _float fTimeDelta);


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

