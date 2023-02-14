#pragma once

#include "AkazaState.h"

BEGIN(Client)
BEGIN(Akaza)
class CUpperHitState : public CAkazaState
{
public:
	CUpperHitState(CAkaza::HIT_TYPE eHitType, STATE_TYPE eType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower, _float fJumpTime = 0.035f);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;


private:
	void Increase_Height(CAkaza * pAkaza, _float fTimeDelta);
	void Fall_Height(CAkaza * pAkaza, _float fTimeDelta);

	void KnockBack_Player(CAkaza* pAkaza, _float fTimeDelta);
	void Bound_Player(CAkaza* pAkaza, _float fTimeDelta);
	void Upper_Player(CAkaza* pAkaza, _float fTimeDelta);

	CAkazaState* KnockBackState(CAkaza* pAkaza, _float fTimeDelta);
	CAkazaState* BoundState(CAkaza* pAkaza, _float fTimeDelta);
	CAkazaState* UpperState(CAkaza* pAkaza, _float fTimeDelta);
	CAkazaState* Upper2State(CAkaza* pAkaza, _float fTimeDelta);
private:
	CAkaza::HIT_TYPE m_eHitType = CAkaza::HIT_TYPE::HIT_END;
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

};
END
END