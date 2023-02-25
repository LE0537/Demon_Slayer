#pragma once

#include "EnmuState.h"

BEGIN(Client)
BEGIN(Enmu)
class CUpperHitState : public CEnmuState
{
public:
	CUpperHitState(CEnmu::HIT_TYPE eHitType, STATE_TYPE eType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower, _float fJumpTime = 0.035f);

	virtual CEnmuState* HandleInput(CEnmu* pEnmu) override;
	virtual CEnmuState* Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	virtual CEnmuState* Late_Tick(CEnmu* pEnmu, _float fTimeDelta) override;


	virtual void Enter(CEnmu* pEnmu) override;
	virtual void Exit(CEnmu* pEnmu) override;


private:
	void Increase_Height(CEnmu* pEnmu, _float fTimeDelta);
	void Fall_Height(CEnmu* pEnmu, _float fTimeDelta);

	void KnockBack_Player(CEnmu* pEnmu, _float fTimeDelta);
	void Bound_Player(CEnmu* pEnmu, _float fTimeDelta);
	void Upper_Player(CEnmu* pEnmu, _float fTimeDelta);

	CEnmuState* KnockBackState(CEnmu* pEnmu, _float fTimeDelta);
	CEnmuState* BoundState(CEnmu* pEnmu, _float fTimeDelta);
	CEnmuState* UpperState(CEnmu* pEnmu, _float fTimeDelta);
	CEnmuState* Upper2State(CEnmu* pEnmu, _float fTimeDelta);
private:
	CEnmu::HIT_TYPE m_eHitType = CEnmu::HIT_TYPE::HIT_END;
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