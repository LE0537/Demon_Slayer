#pragma once

#include "NezukoState.h"

BEGIN(Client)
BEGIN(Nezuko)
class CUpperHitState : public CNezukoState
{
public:
	CUpperHitState(CNezuko::HIT_TYPE eHitType, STATE_TYPE eType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower, _float fJumpTime = 0.035f);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;


private:
	void Increase_Height(CNezuko * pNezuko, _float fTimeDelta);
	void Fall_Height(CNezuko * pNezuko, _float fTimeDelta);

	void KnockBack_Player(CNezuko* pNezuko, _float fTimeDelta);
	void Bound_Player(CNezuko* pNezuko, _float fTimeDelta);
	void Upper_Player(CNezuko* pNezuko, _float fTimeDelta);

	CNezukoState* KnockBackState(CNezuko* pNezuko, _float fTimeDelta);
	CNezukoState* BoundState(CNezuko* pNezuko, _float fTimeDelta);
	CNezukoState* UpperState(CNezuko* pNezuko, _float fTimeDelta);
	CNezukoState* Upper2State(CNezuko* pNezuko, _float fTimeDelta);
private:
	CNezuko::HIT_TYPE m_eHitType = CNezuko::HIT_TYPE::HIT_END;
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