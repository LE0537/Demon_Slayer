#pragma once

#include "TanjiroState.h"

BEGIN(Client)
BEGIN(Tanjiro)
class CUpperHitState : public CTanjiroState
{
public:
	CUpperHitState(CTanjiro::HIT_TYPE eHitType, STATE_TYPE eType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower, _float fJumpTime = 0.035f);

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;


private:
	void Increase_Height(CTanjiro * pTanjiro, _float fTimeDelta);
	void Fall_Height(CTanjiro * pTanjiro, _float fTimeDelta);

	void KnockBack_Player(CTanjiro* pTanjiro, _float fTimeDelta);
	void Bound_Player(CTanjiro* pTanjiro, _float fTimeDelta);
	void Upper_Player(CTanjiro* pTanjiro, _float fTimeDelta);

	CTanjiroState* KnockBackState(CTanjiro* pTanjiro, _float fTimeDelta);
	CTanjiroState* BoundState(CTanjiro* pTanjiro, _float fTimeDelta);
	CTanjiroState* UpperState(CTanjiro* pTanjiro, _float fTimeDelta);
	CTanjiroState* Upper2State(CTanjiro* pTanjiro, _float fTimeDelta);

private:
	CTanjiro::HIT_TYPE m_eHitType = CTanjiro::HIT_TYPE::HIT_END;
	_float m_fBoundPower = 0.f;
	_float m_fJumpPower = 0.f;
	_float m_fKnockBackPower = 0.f;

private:
	_bool m_bHitPlayerJump = false;
	_bool m_bNextAnim = false;
	_bool m_bReset = false;
	_bool m_bMaxHeight = false;

private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fCurrentPosY = 0.f;
	_float m_fJumpTime = 0.f;
	_float m_fDelay = 0.f;
	_bool  m_bStop = false;
	_bool  m_bTrue = false;
};
END
END