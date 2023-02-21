#pragma once

#include "TanjiroState.h"
#include "BaseAtk.h"
#include <queue>

BEGIN(Client)
BEGIN(Tanjiro)

class CHinoCami_CinemaState : public CTanjiroState
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
	CHinoCami_CinemaState(CINEMASCENE eScene);

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;


	CTanjiroState* Scene_Start(CTanjiro* pTanjiro, _float fTimeDelta);
	CTanjiroState* Scene_0(CTanjiro* pTanjiro, _float fTimeDelta);
	CTanjiroState* Scene_1(CTanjiro* pTanjiro, _float fTimeDelta);
	CTanjiroState* Scene_2(CTanjiro* pTanjiro, _float fTimeDelta);
	CTanjiroState* Scene_3(CTanjiro* pTanjiro, _float fTimeDelta);
	CTanjiroState* Scene_4(CTanjiro* pTanjiro, _float fTimeDelta);
	CTanjiroState* Scene_5(CTanjiro* pTanjiro, _float fTimeDelta);
	CTanjiroState* Scene_6(CTanjiro* pTanjiro, _float fTimeDelta);


	void Increase_Height(CTanjiro* pTanjiro, _float fTimeDelta);
	void Fall_Height(CTanjiro* pTanjiro, _float fTimeDelta);
	void Fall_Height_Player(CTanjiro* pTanjiro, _float fTimeDelta);



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


	_bool       m_bFrameControl = false;
	_bool       m_bOnGround = false;

	_float m_fPositionY;
	_float3 m_vPosition;
	_float3 m_vVelocity;
};

END
END