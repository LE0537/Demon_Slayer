#pragma once

#include "TanjiroState.h"
#include "BaseAtk.h"
#include <queue>

BEGIN(Client)
BEGIN(Tanjiro)

class CTrain_CinemaState : public CTanjiroState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6, SCENE_7 , SCENE_END };
	enum SCENE_ANIM 
	{
		ANIM_SCENE_START = 66,

		ANIM_SCENE_0 = 67,

		ANIM_SCENE_1 = 68,

		ANIM_SCENE_2 = 66,

		ANIM_SCENE_3 = 67,

		ANIM_SCENE_4 = 68,

		ANIM_SCENE_5 = 104,

		ANIM_SCENE_6 = 105,

		ANIM_SCENE_7 = 112,
		

	};
public:
	CTrain_CinemaState(CINEMASCENE eScene);
	CTrain_CinemaState(CINEMASCENE eScene, _float fJumpHeight, _float fJumpTime, _float fSpeed, _float fJumpTimer);


	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;

	void Jump(CTanjiro* pTanjiro, _float fTimeDelta);
	void Initialize_JumpState(CTanjiro* pTanjiro, _fvector vTargetPosition);
	
private:
	CINEMASCENE m_eScene = SCENE_END;
	_bool m_bNextAnim = false;
	_bool m_bDebug = false;
	_bool m_bMaxHeight = false;

	_float3 m_vPlayerPosition; // 플레이어 위치
	_float3 m_vTargetPosition; // 점프하려는 위치

	_float m_fJumpHeight = 60.f; // 점프 높이
	_float m_fJumpTime = 1.7f; // 점프에 걸리는 시간
	_float m_fJumpTimer = 0.f; // 점프 타이머
	_float m_fJumpSpeed = 40.f; // 점프 속도

	_float m_fDelay = 0.f;

};

END
END